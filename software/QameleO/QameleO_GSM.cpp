#include "QameleO_GSM.h"

#include "QameleO_conf.h"
#include "QameleO_print.h"
#include "QameleO_hard_conf.h"

QameleO_GSM * QameleO_GSM::_instance = nullptr;

QameleO_GSM::QameleO_GSM()
  : _has_new_unix_time(false)
  , _last_unix_time(0)
  , _nbSendMeasure(0)
  , _nbUnixTimeReceive(0)
{}

bool QameleO_GSM::begin() {
  this->_instance = this;

  for (uint8_t i = 0; i < 3; i++) {
    if (this->_modem.begin(SerialGSM)) {
      return true;
    }
    delay(3000);
  }

  return false;
}

unsigned long QameleO_GSM::getNbOfSendMeasure() const {
  return this->_nbSendMeasure;
}

unsigned long QameleO_GSM::getNbOfUnixTimeReceive() const {
  return this->_nbUnixTimeReceive;
}

unsigned long QameleO_GSM::getLastUnixTime() const {
  return this->_last_unix_time;
}

bool QameleO_GSM::hasNewUnixTime() const {
  return this->_has_new_unix_time;
}

void QameleO_GSM::clearNewUnixTimeFlag(){
  this->_has_new_unix_time = false;
}

void QameleO_GSM::addOneSendMeasure() {
  this->_nbSendMeasure++;
}

void QameleO_GSM::addOneReceiveTime() {
  this->_nbUnixTimeReceive++;
}

void QameleO_GSM::_mqttCallbackRouter(const char * topic, const char * payload, uint16_t length){
  if(_instance){
    _instance->_onMqttMessage(topic, payload, length);
  }
}

void QameleO_GSM::_mqttRawCallbackRouter(const char * urc){
  if(_instance){
    _instance->_onMqttRaw(urc);
  }
}

void QameleO_GSM::_onMqttRaw(const char * urc) {
  LOG("URC : ");
  LOGLN(urc);
  LOGLN("");
}

bool QameleO_GSM::_extractUnixTime(const char* payload) {
  if (!payload || payload[0] == '\0') return false;

  char* end = nullptr;
  unsigned long ts = strtoul(payload, &end, 10);

  if (end == payload) return false;
  if (*end != '\0') return false;
  if (ts == 0) return false;

  this->_last_unix_time = ts;
  return true;
}

void QameleO_GSM::_onMqttMessage(const char* topic, const char* payload, uint16_t length) {
  if (!topic || !payload || length == 0) return;

  if (strcmp(topic, MQTT_TOPIC_TIME) != 0) return;

  char buffer[32];

  if (length >= sizeof(buffer)) {
    length = sizeof(buffer) - 1;
  }

  memcpy(buffer, payload, length);
  buffer[length] = '\0';

  if (this->_extractUnixTime(buffer)) {
    this->_has_new_unix_time = true;
    this->addOneReceiveTime();
  }

  LOG("Topic : ");
  LOGLN(topic);
  LOG("Length : ");
  LOGLN(length);
  LOG("Message : ");
  LOGLN(buffer);
  LOGLN("");
}

void QameleO_GSM::loop() {
  this->_modem.loop();
}

void QameleO_GSM::loopFor(uint32_t durationMs) {
  this->_modem.loopFor(durationMs);
}

bool QameleO_GSM::_setupNetwork() {
  if (!this->_modem.isAlive()) return false;

  this->_modem.simUnlock(GSM_PIN_CODE);
  delay(1000);

  if (!this->_modem.isSIMReady()) {
    if (!this->_modem.simUnlock(GSM_PIN_CODE)) return false;
  }

  if (!this->_modem.isSIMReady()) return false;

  if (!this->_modem.networkConnect(GSM_APN, GSM_USER, GSM_PASS, GSM_PDP_TYPE)) return false;

  return true;
}

bool QameleO_GSM::setupGSM_start(){
  return _setupNetwork();
}

bool QameleO_GSM::_setupMQTT(){
  if(!this->_modem.mqttsetCallback(QameleO_GSM::_mqttCallbackRouter)) return false;
  if(!this->_modem.mqttsetRawCallback(QameleO_GSM::_mqttRawCallbackRouter)) return false;

  if(!this->_modem.mqttsetClient(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) return false;

  if(!this->_modem.mqttconnect(MQTT_HOST, MQTT_PORT)) return false;

  return true;
}

bool QameleO_GSM::_subscribeClockTopic(){
  if(!this->_modem.mqttsubscribe(MQTT_TOPIC_TIME, MQTT_QOS)) return false;
  
  return true;
}

bool QameleO_GSM::setupMQTTAndSubscribe(){
  if(!_setupMQTT()) return false;
  if(!_subscribeClockTopic()) return false;

  return true;
}

bool QameleO_GSM::readTheClock(uint32_t listenMs){
  clearNewUnixTimeFlag();

  unsigned long start = millis();
  while (millis() - start < listenMs) {
      loop();
      if (hasNewUnixTime()) return true;
  }
  return false;
}

bool QameleO_GSM::sendData(const char* msg, bool is_last) {
  if (!msg || msg[0] == '\0') {
    return false;
  }

  LOG("sendData: publication MQTT...");
  LOGLN(msg);

  if (this->_modem.mqttpublish(MQTT_TOPIC, msg, MQTT_QOS, false)) {
    LOGLN("sendData: publish OK");
    return true;
  }

  LOGLN("sendData: publish echoue, tentative reconnexion MQTT...");

  if (!this->reconnectMQTT()) {
    LOGLN("sendData: reconnexion MQTT impossible");
    return false;
  }

  bool ok = this->_modem.mqttpublish(MQTT_TOPIC, msg, MQTT_QOS, false);

  LOGLN(ok ? "sendData: publish OK apres reconnexion" : "sendData: publish encore en erreur");

  return ok;
}


bool QameleO_GSM::sendData(const String& msg, bool is_last){
  return sendData(msg.c_str(), is_last);
}


bool QameleO_GSM::resubscribeTopics() {
  return this->_subscribeClockTopic();
}

bool QameleO_GSM::reconnectMQTT() {
  if (this->_modem.mqttisConnected()) {
    return true;
  }

  if (!this->_modem.mqttreconnect()) {
    return false;
  }

  return this->resubscribeTopics();
}



bool QameleO_GSM::stopGSM() {
  bool mqttOk = this->_modem.mqttdisconnect();
  bool netOk  = this->_modem.networkDisconnect();

  return mqttOk && netOk;
}

bool QameleO_GSM::reset(){
  return this->_modem.reset();
}

void QameleO_GSM::closeGSM() {
  if (this->_modem.mqttisConnected()) {
    this->_modem.mqttdisconnect();
  }

  this->_modem.networkDisconnect();

  this->_has_new_unix_time = false;
  this->_last_unix_time    = 0;
  this->_nbSendMeasure     = 0;
  this->_nbUnixTimeReceive = 0;
}

bool QameleO_GSM::connectGSMAndMQTT() {
  if (!this->_setupNetwork()) {
    return false;
  }

  if (!this->_setupMQTT()) {
    this->_modem.networkDisconnect();
    return false;
  }

  if (!this->_subscribeClockTopic()) {
    this->_modem.mqttdisconnect();
    this->_modem.networkDisconnect();
    return false;
  }

  return true;
}

bool QameleO_GSM::stopMQTTOnly(){
  return this->_modem.mqttdisconnect();
}
