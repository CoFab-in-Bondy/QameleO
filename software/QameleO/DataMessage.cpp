#include "DataMessage.h"

#include "QameleO_conf.h"

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















