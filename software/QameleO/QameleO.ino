#include <MemoryUsage.h>
STACK_DECLARE
#include "QameleO_print.h"
#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"

#include "QameleO_Fan.h"
#include "QameleO_GSM.h"

#include "DustSensor.h"
#include "QameleO_NextPM_Dust.h"
// #include "QameleO_NextPM_Dust_RS485.h"

#include "HumidityTempSensor.h"
#include "QameleO_SHT35_HumidityTemp.h"

#include "QameleO_SD.h"
#include "QameleO_Boot_System.h"
#include "QameleO_struct.h"
#include "DataMessage.h"

#include "MyFile.h"
#include "MyFile.cpp"
#include "MyPile.cpp"

// ==========================================================================
// Objets principaux
// ==========================================================================

QameleO_GSM myGSM;
QameleO_Fan myFan;
QameleO_SD mySD;
QameleO_Boot_System timerSystem;

QameleO_measure sensorData;
DataMessage myDataMsg;

// ==========================================================================
// Capteurs
// ==========================================================================

HumidityTempSensor *myHumidityTempSensor = new QameleO_SHT35_HumidityTemp();
QameleO_NextPM_Dust *myDustSensor = new QameleO_NextPM_Dust();
//DustSensor *myDustSensor = new QameleO_NextPM_Dust();
// DustSensor *myDustSensor = new QameleO_NextPM_Dust_RS485();

// ==========================================================================
// Variables de temps
// ==========================================================================

unsigned long lastClockMs = 0;
unsigned long lastSendMs  = 0;

const unsigned long MEASURE_PERIOD_MS = (unsigned long)MEASURE_PERIOD * 60000UL;
const unsigned long CLOCK_PERIOD_MS   = (unsigned long)RECEIVE_UNIXTIME_PERIOD * 60000UL;
const unsigned long SEND_PERIOD_MS    = (unsigned long)SEND_PERIOD * 60000UL;

// false : ferme seulement MQTT après l’envoi.
// true  : ferme MQTT + data réseau. Plus économe, mais reconnexion plus lente.
const bool GSM_FULL_DISCONNECT = false;

// ==========================================================================
// Prototypes
// ==========================================================================

void printResult(const __FlashStringHelper *label, bool ok);
void acquireSensors();
void runGsmCycle(bool needClock, bool needSend);
void closeGsmAfterCycle();
void waitUntilNextMeasure(unsigned long startAcquire);

// ==========================================================================
// SETUP
// ==========================================================================

void setup() {
  Serial.begin(115200);
  SerialPMS.begin(115200, SERIAL_8E1);
  SerialGSM.begin(TRANSMISSION_RATE);

  while (!Serial);

  delay(3000);

  Serial.println(F("======================================================================"));
  Serial.println(F("                       QameleO : Arduino Mega                         "));
  Serial.println(F("======================================================================"));
//processRebootWatchdog();
  Serial.println(F("Initialisation des modules ..."));

  // ----------------------------------------------------------------------
  // Ventilateur
  // ----------------------------------------------------------------------
  myFan.setupFan();

  // ----------------------------------------------------------------------
  // GSM / TEL0177
  // ----------------------------------------------------------------------
  bool gsmOk = myGSM.begin();
  printResult(F("Init Modem GSM"), gsmOk);

  if (!gsmOk) {
    Serial.println(F("Erreur : modem GSM non detecte"));
  } else {
    printResult(F("Reset GSM"), myGSM.reset());

    // Après reset, le module a besoin de temps pour redémarrer.
    delay(10000);

    printResult(F("Re-init Modem GSM"), myGSM.begin());
  }

  // Important :
  // On ne fait PAS setupGSM_start() ici.
  // La connexion GSM/MQTT se fait uniquement dans runGsmCycle().

  // ----------------------------------------------------------------------
  // Capteur humidité / température
  // ----------------------------------------------------------------------
  if (myHumidityTempSensor != nullptr) {
    myHumidityTempSensor->setupSensor();
    delay(1000);
    Serial.println(F("Init capteur humidite/temperature --> OK"));
  }

  // ----------------------------------------------------------------------
  // Capteur particules
  // ----------------------------------------------------------------------
  if (myDustSensor != nullptr) {
    myDustSensor->setupDustSensor();
    Serial.println(F("Init capteur particules --> OK"));
  }

  // ----------------------------------------------------------------------
  // Carte SD
  // ----------------------------------------------------------------------
  mySD.setupSD();

  // Important :
  // Ne pas effacer automatiquement le buffer au démarrage,
  // sinon on perd les données non envoyées après reboot.
  //
  // Pour un test seulement, tu peux décommenter :
  // mySD.resetFileDataToSend();

  // ----------------------------------------------------------------------
  // Boot system
  // ----------------------------------------------------------------------
  timerSystem.setNbReboot();

  // Forcer lecture heure + envoi dès la première mesure.
  lastClockMs = millis() - CLOCK_PERIOD_MS;
  lastSendMs  = millis() - SEND_PERIOD_MS;

  Serial.println();
  Serial.println(F("Fin de l'initialisation !!!"));
  Serial.println(F("Debut des mesures"));
  Serial.println();
}

// ==========================================================================
// LOOP
// ==========================================================================

void loop() {
  myGSM.loop();

  Serial.println(F("-----------------------------> QameleO <---------------------------------"));

  timerSystem.startMyWatchdog();

  unsigned long startAcquire = millis();

  // ----------------------------------------------------------------------
  // 1. Lecture des capteurs
  // ----------------------------------------------------------------------
  acquireSensors();

  // ----------------------------------------------------------------------
  // 2. Création du message QameleO
  //
  // Format initial :
  // millis;nbReboot;SENSOR_NAME;messageID;pm1:pm25:pm10:humidity:temperature:humidityNextPM:temperatureNextPM
  // ----------------------------------------------------------------------
  String dte = myDataMsg.createMessage(
    sensorData,
    timerSystem.getNbReboot()
  );

  Serial.print(F("Message cree : "));
  Serial.println(dte);

  if (dte.length() <= 0) {
    Serial.println(F("Erreur : aucun message cree"));
    timerSystem.reboot_now();
    delay(6000);
    return;
  }

  // ----------------------------------------------------------------------
  // 3. Sauvegarde dans le buffer SD
  // ----------------------------------------------------------------------
  mySD.saveToSDBuffer(dte);

  // ----------------------------------------------------------------------
  // 4. Vérifier si on doit faire un cycle GSM
  // ----------------------------------------------------------------------
  unsigned long now = millis();

  bool needClock = false;
  bool needSend  = false;

  if (now - lastClockMs >= CLOCK_PERIOD_MS) {
    needClock = true;
  }

  if (now - lastSendMs >= SEND_PERIOD_MS) {
    needSend = true;
  }

  // ----------------------------------------------------------------------
  // 5. Cycle GSM unique : lecture heure + envoi
  // ----------------------------------------------------------------------
  if (needClock || needSend) {
    runGsmCycle(needClock, needSend);
  }

  // ----------------------------------------------------------------------
  // 6. Fin watchdog + attente prochaine mesure
  // ----------------------------------------------------------------------
  timerSystem.stopMyWatchdog();
STACK_COMPUTE;
SRamDisplay();

  waitUntilNextMeasure(startAcquire);
}

// ==========================================================================
// Affichage résultat
// ==========================================================================

void printResult(const __FlashStringHelper *label, bool ok) {
  Serial.print(label);
  Serial.print(F(" --> "));
  Serial.println(ok ? F("OK") : F("FAIL"));
}

// ==========================================================================
// Lecture des capteurs
// ==========================================================================

void acquireSensors() {
  Serial.println(F("Lecture des capteurs..."));
  myDustSensor -> wakeUp();
  // ----------------------------------------------------------------------
  // Ventilateur
  // ----------------------------------------------------------------------
  myFan.runFanProcess();

  // ----------------------------------------------------------------------
  // SHT35 : humidité / température
  // ----------------------------------------------------------------------
  if (myHumidityTempSensor != nullptr) {
    myHumidityTempSensor->runProcess();

    float h = myHumidityTempSensor->getHumidity_();
    float t = myHumidityTempSensor->getTemperature_();

    // Si ta classe retourne NAN en cas d'erreur, on protège ici.
    if (!isnan(h) && !isnan(t)) {
      sensorData.humidity = h;
      sensorData.temperature = t;
    } else {
      Serial.println(F("Erreur SHT35 : mesure NAN, anciennes valeurs conservees"));
    }

    Serial.print(F("Humidite = "));
    Serial.println(sensorData.humidity);

    Serial.print(F("Temperature = "));
    Serial.println(sensorData.temperature);
  }

  // ----------------------------------------------------------------------
  // NextPM : particules
  //
  // Important :
  // Ton capteur fonctionne avec runDustSensorData() sans paramètre,
  // puis récupération avec getP1(), getP25(), getP10().
  // ----------------------------------------------------------------------
  if (myDustSensor != nullptr) {
    myDustSensor->runDustSensorData();

    sensorData.pm1  = myDustSensor->getP1();
    sensorData.pm25 = myDustSensor->getP25();
    sensorData.pm10 = myDustSensor->getP10();
    sensorData.pm1_ppm = myDustSensor->getP1_ppm();
    sensorData.pm25_ppm = myDustSensor->getP25_ppm();
    sensorData.pm10_ppm = myDustSensor->getP10_ppm();
    sensorData.humidityNextPM = myDustSensor->getHumidity();
    sensorData.temperatureNextPM = myDustSensor->getTemperature();
    Serial.print(F("PM1 = "));
    Serial.println(sensorData.pm1);

    Serial.print(F("PM2.5 = "));
    Serial.println(sensorData.pm25);

    Serial.print(F("PM10 = "));
    Serial.println(sensorData.pm10);
    
  } 
   myDustSensor -> sleep();

}

// ==========================================================================
// Cycle GSM / MQTT complet
// ==========================================================================

void runGsmCycle(bool needClock, bool needSend) {
  Serial.println();
  Serial.println(F("=== CYCLE GSM / MQTT ==="));

  if (!myGSM.connectGSMAndMQTT()) {
    Serial.println(F("Erreur : connexion GSM/MQTT impossible"));
    return;
  }

  Serial.println(F("Connexion GSM/MQTT OK"));

  // ----------------------------------------------------------------------
  // 1. Lecture de l'heure Unix
  // ----------------------------------------------------------------------
  if (needClock) {
    Serial.println(F("Lecture heure UNIX..."));

    if (myGSM.readTheClock(15000)) {
      unsigned long unixTime = myGSM.getLastUnixTime();

      Serial.print(F("Heure Unix recue : "));
      Serial.println(unixTime);

      // Important :
      // DataMessage utilise sa propre horloge interne.
      // On lui donne donc l'heure reçue par le GSM.
      DataMessage::setUnixTime(unixTime);

      lastClockMs = millis();
    } else {
      Serial.println(F("Aucune heure Unix recue"));
    }

    // Petite pause pour laisser passer les URC MQTT.
    myGSM.loopFor(300);
    delay(200);
  }

  // ----------------------------------------------------------------------
  // 2. Envoi des messages stockés sur SD
  // ----------------------------------------------------------------------
  if (needSend) {
    Serial.println(F("Envoi des donnees en attente..."));

    Queue<String> dataToSend = mySD.getDataToSend();

    if (dataToSend.isEmpty()) {
      Serial.println(F("Aucune donnee a envoyer"));
      lastSendMs = millis();
    } else {
      bool allSent = true;

      // On compte un cycle d'envoi, pas chaque message.
      myGSM.addOneSendMeasure();

      while (!dataToSend.isEmpty()) {
        String rawMessage = dataToSend.dequeue();

        String mqttMessage = myDataMsg.haveUnixTime(
          rawMessage,
          timerSystem.getNbReboot()
        );

        Serial.print(F("Message MQTT : "));
        Serial.println(mqttMessage);

        bool isLast = dataToSend.isEmpty();

        if (myGSM.sendData(mqttMessage, isLast)) {
          Serial.println(F("Message envoye"));
          mySD.saveToSDLog(mqttMessage);
        } else {
          Serial.println(F("Erreur envoi message"));
          allSent = false;
          break;
        }

        myGSM.loopFor(200);
      }

      if (allSent) {
        Serial.println(F("Tous les messages ont ete envoyes"));
        mySD.resetFileDataToSend();
        lastSendMs = millis();
      } else {
        Serial.println(F("Tous les messages n'ont pas ete envoyes"));
        Serial.println(F("Le buffer SD est conserve pour le prochain cycle"));
      }
    }
  }

  // ----------------------------------------------------------------------
  // 3. Fermeture MQTT / GSM
  // ----------------------------------------------------------------------
  closeGsmAfterCycle();

  Serial.println(F("Fin cycle GSM / MQTT"));
  Serial.println();
}

// ==========================================================================
// Fermeture GSM / MQTT
// ==========================================================================

void closeGsmAfterCycle() {
  Serial.println(F("Deconnexion GSM/MQTT..."));

  bool ok = false;

  if (GSM_FULL_DISCONNECT) {
    // Ferme MQTT + data réseau.
    // Reconnexion plus lente au prochain cycle.
    ok = myGSM.stopGSM();
  } else {
    // Ferme seulement MQTT.
    // Recommandé pour tes tests et pour éviter networkConnect -> TEL_TIMEOUT.
    ok = myGSM.stopMQTTOnly();
  }

  if (ok) {
    Serial.println(F("Deconnexion OK"));
  } else {
    Serial.println(F("Erreur pendant la deconnexion"));
  }
}

// ==========================================================================
// Attente jusqu'à la prochaine mesure
// ==========================================================================

void waitUntilNextMeasure(unsigned long startAcquire) {
  unsigned long elapsed = millis() - startAcquire;

  long waitTime = (long)MEASURE_PERIOD_MS - (long)elapsed;

  if (waitTime > 0) {
    Serial.print(F("Attente avant prochaine mesure : "));
    Serial.print(waitTime / 1000);
    Serial.println(F(" secondes"));

    delay(waitTime);
  } else {
    Serial.println(F("Pas d'attente : la mesure a depasse la periode prevue"));
  }
}