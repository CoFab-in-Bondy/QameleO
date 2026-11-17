#include "QameleO_GSM_Tel.h"

#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"

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










