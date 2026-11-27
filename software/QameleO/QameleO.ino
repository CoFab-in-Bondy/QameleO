#include <MemoryUsage.h>

#include "QameleO_SD.h"
#include "QameleO_Fan.h"
#include "DataMessage.h"
#include "QameleO_GSM_Tel.h"
#include "QameleO_Boot_System.h"

#include "QameleO_conf.h"
#include "QameleO_struct.h"
#include "QameleO_hard_conf.h"


#include "DustSensor.h"
#ifdef NextPM_SENSOR
  #include "QameleO_NextPM_Dust.h"
#endif
#include "HumidityTempSensor.h"
#ifdef SHT_SENSOR
  #include "QameleO_SHT35_HumidityTemp.h"
#endif


#ifdef NextPM_SENSOR
  DustSensor *myDustSensor = new QameleO_NextPM_Dust();
#endif
#ifdef SHT_SENSOR
  HumidityTempSensor * myHumidityTempSensor = new QameleO_SHT35_HumidityTemp();
#endif


QameleO_SD mySD;
QameleO_Fan myFan;
QameleO_GSM_Tel myGSM;
DataMessage myDataMsg;
QameleO_measure sensorData;
QameleO_Boot_System timerSystem;


// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  SerialGSM.begin(115200);
  SerialPMS.begin(115200, SERIAL_8E1);
  while (!Serial);
  delay(3000);

  Serial.println(F("-----------------------------> QameleO - Setup <---------------------------------"));
  Serial.println(F("Initialisation des modules ... "));

  myFan.setupFan();

  myGSM.begin();
  myGSM.requestUnixTime();

  mySD.begin();
  mySD.resetSD();
  mySD.resetFileDataToSend();

  timerSystem.setNbReboot();
  
  Serial.println();
  Serial.println(F("Initialisation des Capteurs ... "));


  #if defined(NextPM_SENSOR) || defined(PMS7003_SENSOR)
    myDustSensor->setupDustSensor();
  #endif
  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->setupSensor();
  #endif

  delay(2000);
  Serial.println();
  Serial.println(F("start measure"));
}


// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  
  //myGSM.loop();
  Serial.println(F("-----------------------------> QameleO - Loop <---------------------------------"));

  timerSystem.startMyWatchdog();
  Serial.println(F("Start sensor ..."));
  unsigned long startAcquire = millis();

  //myFan.runFanProcess();

  char message[256];

// ============================================================================
// Acquisition des données des capteurs
// ============================================================================
  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->runProcess();
    sensorData.humidity = myHumidityTempSensor->getHumidity_();
    sensorData.temperature = myHumidityTempSensor->getTemperature_();
  #endif

  #if defined(NextPM_SENSOR) || defined (PMS7003_SENSOR)
    myDustSensor->runDustSensorData(sensorData);
    /* Serial.print("PM1 = " + String(sensorData.pm1)+ " ");
    Serial.print("PM25 = " + String(sensorData.pm25)+ " ");
    Serial.println("PM10 = " + String(sensorData.pm10)); */
  #endif

// ============================================================================
// Création du message de données à envoyer
// ============================================================================
  char * dte = myDataMsg.createMessage(sensorData, timerSystem.getNbReboot());
  Serial.print(F("Message crée : "));
  Serial.println(dte);

  if(dte[0] == '\O') {
    Serial.println(F("Error : not any message created."));
    timerSystem.reboot_now();
    delay(6000);    
  } else {
    mySD.saveToSDBuffer(dte);
  }


// ============================================================================
// Synchronisation de l'heure Unix avec le module GSM
// ============================================================================
  // = Calcul du temps écoulé depuis la dernière réception de l'heure Unix
  long tmpTime = millis() - myGSM.getNbUnixTimeReceive() * 60000UL * RECEIVE_UNIXTIME_PERIOD;

  // Si tmpTime est supérieur ou égal à 0, on demande l'heure Unix au module GSM
  if (tmpTime >= 0) {
    if (myGSM.requestUnixTime()) {
      myGSM.loopFor(10000);               // laisse 10 s pour recevoir le message MQTT
      if (myGSM.getNbUnixTimeReceive() > 0) {
        myDataMsg.setUnixTime(strtoul(myGSM.getUnixTime(), nullptr, 10));
      }
    } else {
      Serial.println("error : myGSM.requestUnixTime(), not any unix time receive");
      if (!myDataMsg.unixTimeIsSet())
        timerSystem.reboot_now();
      delay(6000);
    }
  }

// ============================================================================
// Gestion de l'envoi des mesures via GSM
// ============================================================================
  tmpTime = millis() - myGSM.getNbSendMeasure() * 60000UL * SEND_PERIOD;
  if (tmpTime >= 0) {
    myGSM.addOneSendMeasure();

    if (!mySD.hasPendingMessages()) {
      Serial.println(F("There are no data to send"));
    }

    while (mySD.hasPendingMessages()) {
      if (!mySD.readNextMessage(message, sizeof(message))) {
        break;
      }

      myDataMsg.haveUnixTime(message, sizeof(message));   // remplace millis() par la vraie date Unix

      Serial.print(F("Envoie MQTT : "));
      Serial.println(message);

      if (myGSM.sendData(message)) {
        Serial.println(F("The data has been sent."));
        mySD.confirmMessageSent();
        mySD.saveToSDLog(message);
      } else {
        Serial.println(F("Failed, the data hasn't been sent."));
        break;   // évite de boucler indéfiniment si le GSM ne répond plus
      }
    }

    mySD.compactBuffer();
  }

// ============================================================================
// Gestion de la temporisation entre les acquisitions de données
// ============================================================================
  timerSystem.stopMyWatchdog();
  unsigned long to_wait = millis() - startAcquire;
  long tmpBasilic = (long)MEASURE_PERIOD * 60000L - (long)to_wait;

  if (tmpBasilic > 0) {
    delay(tmpBasilic);
  }

  Serial.println();
}