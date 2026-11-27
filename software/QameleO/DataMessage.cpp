#include "DataMessage.h"

#include <limits.h>
#include <stdint.h>

#include "QameleO_conf.h"
#include "QameleO_SD.h"

DataMessage::DataMessage() {
  this->_message[0] = '\0';
  this->_messageID = 0;

  #if defined(NextPM_SENSOR) || defined (PMS7003_SENSOR)
    this->_dustSensor = true;
  #else
    this->_dustSensor = false;
  #endif
  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    this->_humTempSensor = true;
  #else
    this->_humTempSensor = false;
  #endif
}

unsigned long DataMessage::unixTime = 0;
uint32_t DataMessage::lastUpdateTimeClock = 0;


void DataMessage::buildCapteurMessage(const char *data, int nbReboot) {
  this->_messageID = this->_messageID + 1;
}


const char * DataMessage::_buildCapteurMessage(const char * data, int nbReboot) {
  this->_message[0] = '\0';
  this->_messageID = this->_messageID + 1;

  snprintf(
    this->_message, 
    sizeof(this->_message), 
    "%lu;%d;%s;%d;%s", millis(),nbReboot,SENSOR_NAME, this->_messageID, data);

  //char sid [10];
  //char sdate[20];
  //char sreboot[10];
  //const char * ssensorname = SENSOR_NAME;

  //snprintf(sid, sizeof(sid), "%d", _messageID);
  //snprintf(sdate, sizeof(sdate), "%lu", millis());
  //snprintf(sreboot, sizeof(sreboot), "%d", nbReboot);

  //strcat(this->_message, sdate); strcat(this->_message, ";");
  //strcat(this->_message, sreboot); strcat(this->_message, ";");
  //strcat(this->_message, ssensorname); strcat(this->_message, ";");
  //strcat(this->_message, sid); strcat(this->_message, ";");
  //strcat(this->_message, data);

  return this->_message;
}

/**
 * @brief Construit un message contenant les mesures des capteurs.
 *
 * Format du message :
 * temps;reboot;station;messageID;mesures
 *
 * Exemple :
 * 107664;0;QameleO_2026_test;15;2.80:3.60:5.40:44.96:25.00:41.63:24.24:42.00:42.00:42.00
 *
 * Informations générales (séparées par ';') :
 *  1. temps       : Temps écoulé depuis le démarrage (millis(), en ms)
 *  2. reboot      : Nombre de redémarrages
 *  3. station     : Identifiant de la station (SENSOR_NAME)
 *  4. messageID   : Numéro du message
 *
 * Mesures (séparées par ':') :
 *  1. pm1               : Concentration PM1 (µg/m3)
 *  2. pm25              : Concentration PM2.5 (µg/m3)
 *  3. pm10              : Concentration PM10 (µg/m3)
 *  4. humidity          : Humidité SHT35 (%)
 *  5. temperature       : Température SHT35 (°C)
 *  6. humidityNextPM    : Humidité NextPM (%)
 *  7. temperatureNextPM : Température NextPM (°C)
 *  8. pm1_ppm           : Mesure PM1 supplémentaire (unité à vérifier)
 *  9. pm25_ppm          : Mesure PM2.5 supplémentaire (unité à vérifier)
 * 10. pm10_ppm          : Mesure PM10 supplémentaire (unité à vérifier)
 *
 * Les mesures présentes dépendent des capteurs activés.
 *
 * @param data     Structure contenant les mesures (QameleO_measure).
 * @param nbReboot Nombre de redémarrages.
 * @return Pointeur vers le message construit, stocké dans _message.
 */
const char * DataMessage::createMessage(QameleO_measure data, int nbReboot) {
  char chaine [150];
  chaine[0] = '\0';
  int written;

  char pm1[16];
  char pm25[16];
  char pm10[16];
  char humidity[16];
  char temperature[16];

  char pm1_ppm[16];
  char pm25_ppm[16];
  char pm10_ppm[16];
  char humidityNextPM[16];
  char temperatureNextPM[16];

  dtostrf(data.pm1, 1, 2, pm1);
  dtostrf(data.pm25, 1, 2, pm25);
  dtostrf(data.pm10, 1, 2, pm10);
  dtostrf(data.humidity, 1, 2, humidity);
  dtostrf(data.temperature, 1, 2, temperature);

  dtostrf(data.pm1_ppm, 1, 2, pm1_ppm);
  dtostrf(data.pm25_ppm, 1, 2, pm25_ppm);
  dtostrf(data.pm10_ppm, 1, 2, pm10_ppm);
  dtostrf(data.humidityNextPM, 1, 2, humidityNextPM);
  dtostrf(data.temperatureNextPM, 1, 2, temperatureNextPM);

  if(this->_dustSensor && this->_humTempSensor) {
    written = snprintf(
            chaine, 
            sizeof(chaine), 
            "%s:%s:%s:%s:%s",
            pm1, pm25, pm10, humidity, temperature
            );
  } else if(this->_dustSensor) {
    written = snprintf(chaine, 
            sizeof(chaine), 
            "%s:%s:%s",
            pm1, pm25, pm10
            );
  } else if(this->_humTempSensor) {
    written = snprintf(chaine, 
            sizeof(chaine), 
            "%s:%s",
            humidity, temperature
            );
  } else {
    written = snprintf(chaine, 
            sizeof(chaine), 
            "%s,",
            "You have define no any sensor. There are no data to send"
            );
  }

  #ifdef NextPM_SENSOR 
    size_t size = strlen(chaine);
    size_t remaining = sizeof(chaine) - size;

    written = snprintf(chaine + size, 
            sizeof(chaine) - size, 
            ":%s:%s:%s:%s:%s",
            humidityNextPM, temperatureNextPM, pm1_ppm, pm25_ppm, pm10_ppm
            );
  #endif

  if (written < 0 || (size_t)written >= remaining) {
    return nullptr;
  }

  this->_buildCapteurMessage(chaine, nbReboot);
  return this->_message;
}


const char* DataMessage::haveNegativeClock(char* msg, int nbRebootMax) {
    if (msg == nullptr)
        return nullptr;

    char* first = strchr(msg, ';');

    if (first == nullptr)
        return nullptr;

    char* second = strchr(first + 1, ';');

    if (second == nullptr)
        return nullptr;

    char* endNumber = nullptr;

    unsigned long clock = strtoul(msg, &endNumber, 10);

    if (endNumber != first || endNumber == msg)
        return nullptr;

    long idReboot = strtol(first + 1, &endNumber, 10);

    if (endNumber != second ||
        endNumber == first + 1 ||
        idReboot < 0 ||
        idReboot > INT_MAX)
        return nullptr;

    QameleO_SD sd;

    int64_t negativeClock =
        (int64_t)clock -
        (int64_t)sd.readBackup((int)idReboot, nbRebootMax);

    if (negativeClock < LONG_MIN || negativeClock > LONG_MAX)
        return nullptr;

    char clockStr[12];

    int len = snprintf(
        clockStr,
        sizeof(clockStr),
        "%ld",
        (long)negativeClock
    );

    if (len <= 0 || len >= (int)sizeof(clockStr))
        return nullptr;

    size_t oldLength = second - msg;

    if ((size_t)len > oldLength)
        return nullptr;

    memmove(msg + len, second, strlen(second) + 1);

    memcpy(msg, clockStr, len);

    return msg;
}


const char* DataMessage::haveUnixTime(char* msg, size_t capacity)
{
  if (msg == nullptr)
    return nullptr;

  char* first = strchr(msg, ';');
  if (first == nullptr)
    return nullptr;
  char* second = strchr(first + 1, ';');
  if (second == nullptr)
    return nullptr;

  char* endNumber = nullptr;
  unsigned long clockMs = strtoul(msg, &endNumber, 10);
  if (endNumber != first || endNumber == msg)
    return nullptr;

  char clockStr[12];
  int len;

  if (unixTimeIsSet()) {
    unsigned long unixNow = getUnixTime();
    Serial.println(F("Unix Time est set"));
    Serial.println(unixNow);

    unsigned long age = (millis() - clockMs) / 1000;  
    len = snprintf(clockStr, sizeof(clockStr), "%lu", unixNow - age);
  } else {
    Serial.println(F("Unix Time n'est pas set"));
    len = snprintf(clockStr, sizeof(clockStr), "-1");
  }

  if (len <= 0 || len >= (int)sizeof(clockStr))
    return nullptr;

  size_t restLen = strlen(second);
  if ((size_t)len + restLen + 1 > capacity)
    return nullptr;

  memmove(msg + len, second, restLen + 1);
  memcpy(msg, clockStr, len);
  return msg;
}

static void DataMessage::setUnixTime(unsigned long n)
{
  unixTime = n;
  lastUpdateTimeClock = millis();
  Serial.println(unixTime);
}


bool DataMessage::unixTimeIsSet()
{
  if(unixTime > 0){
    return true;
  }else {
    return false;
  }
}


static unsigned long DataMessage::getUnixTime(){
  return unixTime + (millis() - lastUpdateTimeClock)/1000;
}



