#include "TEL0177.h"

#include <string.h>
#include <stdio.h>


// ================================================================================================
// FONCTIONS PRIVÉES
// ================================================================================================

bool TEL0177::_setLastError(TEL_Status status) {
    this->_lastError = status;
    return (status == TEL_OK);
}

void TEL0177::_display(const char* expression, TEL_Status status) {
  if (!expression) expression = "unknown";

  switch (status) {
    case TEL_OK:
      break;

    case TEL_ERROR:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_ERROR", expression);
      break;

    case TEL_TIMEOUT:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_TIMEOUT", expression);
      break;

    case TEL_BUSY:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_BUSY", expression);
      break;

    case TEL_NO_RESPONSE:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_NO_RESPONSE", expression);
      break;

    case TEL_INVALID_PARAM:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_INVALID_PARAM", expression);
      break;

    case TEL_NOT_CONNECTED:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_NOT_CONNECTED", expression);
      break;

    case TEL_BUFFER_OVERFLOW:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_BUFFER_OVERFLOW", expression);
      break;

    case TEL_NOT_READY:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_NOT_READY", expression);
      break;

    case TEL_PARSE_ERROR:
      TEL_LOGLN_ERROR("[TEL0177] %s -> TEL_PARSE_ERROR", expression);
      break;

    default:
      TEL_LOGLN_ERROR("[TEL0177] %s -> UNKNOWN_ERROR", expression);
      break;
  }
}


// ================================================================================================
// INITIALISATION / MODEM
// ================================================================================================
/**
 * 
 */
bool TEL0177::begin(Stream& serial) {
    this->_lastError = TEL_OK;

    this->_at.begin(serial);
    this->_network.begin(this->_at);
    this->_mqtt.begin(this->_at);

    delay(TEL_TIMEOUT_BOOT);

    for (uint8_t i = 0; i < TEL_MAX_RETRIES; i++) {
        if (this->_at.isAlive()) {
            this->_lastError = TEL_OK;
            return true;
        }

        delay(TEL_RETRY_DELAY_MS);
    }

    this->_lastError = TEL_NO_RESPONSE;
    this->_display("begin", this->_lastError);
    return false;
}

/**
 * 
 */
void TEL0177::loop() {
    this->_mqtt.loop();
}

/**
 * 
 */
void TEL0177::loopFor(uint32_t durationMs) {
    this->_mqtt.loopFor(durationMs);
}

/**
 * 
 */
bool TEL0177::reset() {
    TEL_Status s = this->_at.send(TEL_RESET_COMMAND, TEL_TIMEOUT_NETWORK);

    if (s != TEL_OK) {
        this->_display("reset", s);
        return this->_setLastError(s);
    }

    delay(TEL_TIMEOUT_BOOT);

    for (uint8_t i = 0; i < TEL_MAX_RETRIES; i++) {
        if (this->_at.isAlive()) {
            this->_lastError = TEL_OK;
            return true;
        }

        delay(TEL_RETRY_DELAY_MS);
    }

    this->_lastError = TEL_NO_RESPONSE;
    this->_display("reset/isAlive", this->_lastError);
    return false;
}

/**
 * 
 */
bool TEL0177::isAlive() {
    bool ok = this->_at.isAlive();
    this->_lastError = ok ? TEL_OK : TEL_NO_RESPONSE;

    if (!ok) {
        this->_display("isAlive", this->_lastError);
    }

    return ok;
}

/**
 * 
 */
TEL_Status TEL0177::lastError() const {
    return this->_lastError;
}

/**
 * 
 */
const char* TEL0177::lastResponse() const {
    return this->_at.lastResponse();
}


// ================================================================================================
// SIM
// ================================================================================================
/**
 * 
 */
bool TEL0177::isSIMReady() {
    TEL_Status s = this->_network.isSIMReady();

    if (s != TEL_OK) {
        this->_display("isSIMReady", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::simUnlock(const char* pin) {
    TEL_Status s = this->_network.unlockSIM(pin);

    if (s != TEL_OK) {
        this->_display("simUnlock", s);
    }

    return this->_setLastError(s);
}


// ================================================================================================
// RÉSEAU / DATA
// ================================================================================================
/**
 * 
 */
bool TEL0177::isNetworkRegistered() {
    TEL_Status s = this->_network.isNetworkRegistered();

    if (s != TEL_OK) {
        this->_display("isNetworkRegistered", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::isDataNetworkRegistered() {
    TEL_Status s = this->_network.isDataNetworkRegistered();

    if (s != TEL_OK) {
        this->_display("isDataNetworkRegistered", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::isLTERegistered() {
    TEL_Status s = this->_network.isLTERegistered();

    if (s != TEL_OK) {
        this->_display("isLTERegistered", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::waitForNetwork(uint32_t timeout) {
    TEL_Status s = this->_network.waitForNetwork(timeout);

    if (s != TEL_OK) {
        this->_display("waitForNetwork", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::getSignalQuality(int& rssi, int& ber) {
    TEL_Status s = this->_network.getSignalQuality(rssi, ber);

    if (s != TEL_OK) {
        this->_display("getSignalQuality", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::isDataAttached() {
    TEL_Status s = this->_network.isDataAttached();

    if (s != TEL_OK) {
        this->_display("isDataAttached", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::attachData() {
    TEL_Status s = this->_network.attachData();

    if (s != TEL_OK) {
        this->_display("attachData", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::detachData() {
    TEL_Status s = this->_network.detachData();

    if (s != TEL_OK) {
        this->_display("detachData", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::isDataConnected() {
    TEL_Status s = this->_network.isDataConnected();

    if (s != TEL_OK) {
        this->_display("isDataConnected", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::activateDataContext() {
    TEL_Status s = this->_network.activateDataContext();

    if (s != TEL_OK) {
        this->_display("activateDataContext", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::deactivateDataContext() {
    TEL_Status s = this->_network.deactivateDataContext();

    if (s != TEL_OK) {
        this->_display("deactivateDataContext", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::networkConnect(
    const char* apn,
    const char* user,
    const char* pass,
    const char* pdpType
) {
    TEL_Status s = this->_network.connectData(apn, user, pass, pdpType);

    if (s != TEL_OK) {
        this->_display("networkConnect", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::networkDisconnect() {
    TEL_Status s = this->_network.disconnectData();

    if (s != TEL_OK) {
        this->_display("networkDisconnect", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
bool TEL0177::getIPAddress(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getIPAddress(buffer, size);

    if (s != TEL_OK) {
        this->_display("getIPAddress", s);
    }

    return this->_setLastError(s);
}

/**
 * 
 */
TEL_NetState TEL0177::networkState() const {
    return this->_network.getState();
}

/**
 * 
 */
const char* TEL0177::ipAddress() const {
    return this->_network.ipAddress();
}

/**
 * 
 */
const char* TEL0177::apn() const {
    return this->_network.apn();
}

/**
 * 
 */
const char* TEL0177::pdpType() const {
    return this->_network.pdpType();
}


// ================================================================================================
// INFORMATIONS MODULE / SIM / RÉSEAU
// ================================================================================================

bool TEL0177::getOperator(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getOperator(buffer, size);

    if (s != TEL_OK) {
        this->_display("getOperator", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::getIMEI(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getIMEI(buffer, size);

    if (s != TEL_OK) {
        this->_display("getIMEI", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::getICCID(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getICCID(buffer, size);

    if (s != TEL_OK) {
        this->_display("getICCID", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::getIMSI(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getIMSI(buffer, size);

    if (s != TEL_OK) {
        this->_display("getIMSI", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::getManufacturer(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getManufacturer(buffer, size);

    if (s != TEL_OK) {
        this->_display("getManufacturer", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::getModel(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getModel(buffer, size);

    if (s != TEL_OK) {
        this->_display("getModel", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::getFirmware(char* buffer, uint16_t size) {
    TEL_Status s = this->_network.getFirmware(buffer, size);

    if (s != TEL_OK) {
        this->_display("getFirmware", s);
    }

    return this->_setLastError(s);
}


// ================================================================================================
// MQTT
// ================================================================================================

bool TEL0177::mqttsetClient(
    const char* clientId,
    const char* user,
    const char* pass,
    uint16_t keepAlive
) {
    if (!clientId || clientId[0] == '\0') {
        this->_lastError = TEL_INVALID_PARAM;
        this->_display("mqttsetClient", this->_lastError);
        return false;
    }

    this->_mqtt.setClientConfig(clientId, user, pass, keepAlive);
    this->_lastError = TEL_OK;
    return true;
}

bool TEL0177::mqttsetServer(
    const char* clientId,
    const char* user,
    const char* pass,
    uint16_t keepAlive
) {
    return this->mqttsetClient(clientId, user, pass, keepAlive);
}

bool TEL0177::mqttconnect(const char* host, uint16_t port, uint32_t timeout) {
    TEL_Status s = this->_mqtt.connect(host, port, timeout);

    if (s != TEL_OK) {
        this->_display("mqttconnect", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::mqttreconnect() {
    TEL_Status s = this->_mqtt.reconnect();

    if (s != TEL_OK) {
        this->_display("mqttreconnect", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::mqttdisconnect() {
    TEL_Status s = this->_mqtt.disconnect();

    if (s != TEL_OK) {
        this->_display("mqttdisconnect", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::mqttisConnected() {
    bool ok = this->_mqtt.isConnected();
    this->_lastError = ok ? TEL_OK : TEL_NOT_CONNECTED;

    if (!ok) {
        //this->_display("mqttisConnected", this->_lastError);
    }

    return ok;
}

bool TEL0177::mqttpublish(
    const char* topic,
    const char* payload,
    uint8_t qos,
    bool retain,
    uint32_t timeout
) {
    TEL_Status s = this->_mqtt.publish(topic, payload, qos, retain, timeout);

    if (s != TEL_OK) {
        this->_display("mqttpublish", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::mqttsubscribe(const char* topic, uint8_t qos) {
    TEL_Status s = this->_mqtt.subscribe(topic, qos);

    if (s != TEL_OK) {
        this->_display("mqttsubscribe", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::mqttunsubscribe(const char* topic) {
    TEL_Status s = this->_mqtt.unsubscribe(topic);

    if (s != TEL_OK) {
        this->_display("mqttunsubscribe", s);
    }

    return this->_setLastError(s);
}

bool TEL0177::mqttsetCallback(TEL_MQTT_Callback callback) {
    if (!callback) {
        this->_lastError = TEL_INVALID_PARAM;
        this->_display("mqttsetCallback", this->_lastError);
        return false;
    }

    this->_mqtt.setCallback(callback);
    this->_lastError = TEL_OK;
    return true;
}

bool TEL0177::mqttsetRawCallback(TEL_MQTT_RawCallback callback) {
    if (!callback) {
        this->_lastError = TEL_INVALID_PARAM;
        this->_display("mqttsetRawCallback", this->_lastError);
        return false;
    }

    this->_mqtt.setRawCallback(callback);
    this->_lastError = TEL_OK;
    return true;
}