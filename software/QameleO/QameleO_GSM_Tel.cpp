#include "QameleO_GSM_Tel.h"

#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"

char QameleO_GSM_Tel::_unix_time [11] = "";

QameleO_GSM_Tel::QameleO_GSM_Tel() {}

/**
 * 
 */
bool QameleO_GSM_Tel::begin() {
  if (!this->_modem.begin(SerialGSM)) {
    return false;
  }

  if(!this->_modem.mqttsetClient(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) {
    return false;
  }

  if(!this->_modem.mqttsetCallback(QameleO_GSM_Tel::_mqttCallbackRouter)) {
    return false;
  }
  
  return true;
}

bool QameleO_GSM_Tel::_ensureData() {
  if(this->_modem.isDataConnected()) {
    return true;
  }

  if(!this->_modem.isSIMReady()) {
    if(!this->_modem.simUnlock(GSM_PIN_CODE)){ 
      return false;
    }
    delay(2000);
  }

  for(uint8_t i = 0; i < 3; i++) {
    if(this->_modem.networkConnect(GSM_APN)) {
      return true;
    } 
    delay(3000);
  } 

  return false;
}


bool QameleO_GSM_Tel::_ensureMqtt() {

  if(this->_modem.mqttisConnected()) {
    return true;
  }

  if(!this->_ensureData()) {
    return false;
  }

  return this->_modem.mqttconnect(MQTT_HOST, MQTT_PORT);
}


bool QameleO_GSM_Tel::sendData(const char * msg) {
  if(msg == nullptr || msg[0] == '\0') {
    return false;
  }

  if(!this->_ensureMqtt()) {
    return false;
  }

  return this->_modem.mqttpublish(MQTT_TOPIC, msg);
}

bool QameleO_GSM_Tel::_subscribeTimer() {
  if(!this->_ensureMqtt()) {
    return false;
  }

  return this->_modem.mqttsubscribe(MQTT_PASS);
}


void QameleO_GSM_Tel::_mqttCallbackRouter(const char* topic, const char* payload, uint16_t length) {
  Serial.println("=============================================================");
  Serial.print("Topic   = "); Serial.println(topic);
  Serial.print("Contenu = "); Serial.println(payload);
  Serial.print("Taille  = "); Serial.println(length);
  Serial.println("=============================================================");

  if(topic != nullptr && strcmp(topic, MQTT_TOPIC_TIME) == 0) {
    _saveUnixTime(payload, length);
  }
}

bool QameleO_GSM_Tel::_saveUnixTime(const char * payload, uint16_t length) {
  if(!length == 10) {
    return false;
  }

  memcpy(_unix_time, payload, length);
  _unix_time[length] = '\0';

  return true;
}

const char * QameleO_GSM_Tel::getUnixTime() const {
  return this->_unix_time; 
}


bool QameleO_GSM_Tel::requestUnixTime() {
  return this->_subscribeTimer();
}

void QameleO_GSM_Tel::loop() {
  this->_modem.loop();
}

void QameleO_GSM_Tel::loopFor(int timer) {
  this->_modem.loopFor(timer);
}
