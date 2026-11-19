#include "TEL0177_Network.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

/**
 * 
 */
void TEL0177_Network::begin(TEL0177_AT_Core& at){
  this->_at = &at;
  this->_state = NET_DETACHED;

  this->_simReady = false;
  this->_networkReady = false;
  this->_dataReady = false;

  memset(this->_ip, 0, sizeof(this->_ip));
  memset(this->_apn, 0, sizeof(this->_apn));
  memset(this->_pdpType, 0, sizeof(this->_pdpType));

  strncpy(this->_pdpType, TEL_PDP_TYPE_DEFAULT, sizeof(this->_pdpType) - 1);
}

/**
 * 
 */
TEL_Status TEL0177_Network::isSIMReady() {
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CPIN?",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) {
    this->_simReady = false;
    return s;
  }

  if (strstr(buffer, "+CPIN: READY")) {
    this->_simReady = true;
    return TEL_OK;
  }

  this->_simReady = false;

  if (strstr(buffer, "SIM PIN")) {
    return TEL_NOT_READY;
  }

  if (strstr(buffer, "+CME ERROR") ||
      strstr(buffer, "NOT READY") ||
      strstr(buffer, "BUSY")) {
    return TEL_NOT_READY;
  }

  return TEL_PARSE_ERROR;
}

/**
 * 
 */
TEL_Status TEL0177_Network::unlockSIM(const char* pin) {
  if (!this->_at || !pin || pin[0] == '\0') {
    return TEL_INVALID_PARAM;
  }

  uint32_t start = millis();
  bool pinSent = false;

  while (millis() - start < TEL_TIMEOUT_NETWORK) {
    char buffer[TEL_RX_BUFFER_SIZE] = {0};

    TEL_Status s = this->_at->sendAndReadRaw(
      "AT+CPIN?",
      buffer,
      sizeof(buffer),
      TEL_TIMEOUT_DEFAULT_AT
    );

    if (s == TEL_OK) {
      if (strstr(buffer, "+CPIN: READY")) {
        this->_simReady = true;
        return TEL_OK;
      }

      if (strstr(buffer, "SIM PIN")) {
        if (!pinSent) {
          char command[TEL_CMD_BUFFER_SIZE] = {0};
          int n = snprintf(command, sizeof(command), "AT+CPIN=\"%s\"", pin);

          if (n < 0 || n >= (int)sizeof(command)) {
            return TEL_BUFFER_OVERFLOW;
          }

          s = this->_at->send(command, TEL_TIMEOUT_DEFAULT_AT);
          if (s != TEL_OK) {
            this->_simReady = false;
            return s;
          }

          pinSent = true;
          delay(TEL_SIM_UNLOCK_DELAY_MS);
        } else {
          delay(TEL_RETRY_DELAY_MS);
        }

        continue;
      }

      if (strstr(buffer, "+CME ERROR") ||
          strstr(buffer, "NOT READY") ||
          strstr(buffer, "BUSY")) {
        this->_simReady = false;
        delay(TEL_RETRY_DELAY_MS);
        continue;
      }
    }

    delay(TEL_RETRY_DELAY_MS);
  }

  this->_simReady = false;
  return TEL_TIMEOUT;
}

/**
 * 
 */
TEL_Status TEL0177_Network::isNetworkRegistered() {
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CREG?",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );
  if (s != TEL_OK) {
    this->_state = NET_DETACHED;
    this->_networkReady = false;
    return s;
  }

  char* p = strstr(buffer, "+CREG:");
  if (!p) {
    this->_networkReady = false;
    this->_state = NET_DETACHED;
    return TEL_PARSE_ERROR;
  }

  int n = 0;
  int stat = 0;

  if (sscanf(p, "+CREG: %d,%d", &n, &stat) != 2) {
    this->_networkReady = false;
    this->_state = NET_DETACHED;
    return TEL_PARSE_ERROR;
  }

  if (stat == 1 || stat == 5) {
    this->_networkReady = true;
    this->_state = NET_REGISTERED;
    return TEL_OK;
  }

  this->_networkReady = false;
  this->_state = NET_DETACHED;
  return TEL_NOT_READY;
}

/**
 * 
 */
TEL_Status TEL0177_Network::isDataNetworkRegistered(){
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGREG?",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );
  if (s != TEL_OK) {
    return s;
  }

  char* p = strstr(buffer, "+CGREG:");
  if (!p) return TEL_PARSE_ERROR;

  int n = 0;
  int stat = 0;

  if (sscanf(p, "+CGREG: %d,%d", &n, &stat) != 2) {
    return TEL_PARSE_ERROR;
  }

  if (stat == 1 || stat == 5) {
    this->_networkReady = true;
    if (this->_state == NET_DETACHED) {
      this->_state = NET_REGISTERED;
    }
    return TEL_OK;
  }

  return TEL_NOT_READY;
}

/**
 * 
 */
TEL_Status TEL0177_Network::isLTERegistered(){
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CEREG?",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );
  if (s != TEL_OK) {
    return s;
  }

  char* p = strstr(buffer, "+CEREG:");
  if (!p) return TEL_PARSE_ERROR;

  int n = 0;
  int stat = 0;

  if (sscanf(p, "+CEREG: %d,%d", &n, &stat) != 2) {
    return TEL_PARSE_ERROR;
  }

  if (stat == 1 || stat == 5) {
    this->_networkReady = true;
    if (this->_state == NET_DETACHED) {
      this->_state = NET_REGISTERED;
    }
    return TEL_OK;
  }

  return TEL_NOT_READY;
}

/**
 * 
 */
TEL_Status TEL0177_Network::_waitForNetworkReg(uint32_t timeout) {
  if (!this->_at) return TEL_INVALID_PARAM;

  uint32_t start = millis();

  while (millis() - start < timeout) {
    TEL_Status s1 = this->isLTERegistered();
    if (s1 == TEL_OK) return TEL_OK;

    TEL_Status s2 = this->isDataNetworkRegistered();
    if (s2 == TEL_OK) return TEL_OK;

    TEL_Status s3 = this->isNetworkRegistered();
    if (s3 == TEL_OK) return TEL_OK;

    delay(TEL_RETRY_DELAY_MS);
  }

  this->_networkReady = false;
  this->_state = NET_DETACHED;
  return TEL_TIMEOUT;
}

/**
 * 
 */
TEL_Status TEL0177_Network::waitForNetwork(uint32_t timeout) {
  return this->_waitForNetworkReg(timeout);
}

/**
 * 
 */
TEL_Status TEL0177_Network::getSignalQuality(int& rssi, int& ber) {
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CSQ",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );
  if (s != TEL_OK) return s;

  char* p = strstr(buffer, "+CSQ:");
  if (!p) return TEL_PARSE_ERROR;

  p += strlen("+CSQ:");

  if (sscanf(p, "%d,%d", &rssi, &ber) != 2) {
    return TEL_PARSE_ERROR;
  }

  if (rssi == 99) {
    return TEL_NOT_READY;
  }

  return TEL_OK;
}

/**
 * 
 */
TEL_Status TEL0177_Network::isDataAttached() {
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGATT?",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  if (strstr(buffer, "+CGATT: 1") || strstr(buffer, "+CGATT:1")) {
    this->_state = NET_DATA_ATTACHED;
    return TEL_OK;
  }

  if (strstr(buffer, "+CGATT: 0") || strstr(buffer, "+CGATT:0")) {
    this->_dataReady = false;

    if (this->_networkReady) {
      this->_state = NET_REGISTERED;
    } else {
      this->_state = NET_DETACHED;
    }

    return TEL_NOT_CONNECTED;
  }

  return TEL_PARSE_ERROR;
}

/**
 * 
 */
TEL_Status TEL0177_Network::attachData() {
  if (!this->_at) return TEL_INVALID_PARAM;

  TEL_Status s = this->isDataAttached();
  if (s == TEL_OK) {
    this->_state = NET_DATA_ATTACHED;
    return TEL_OK;
  }

  s = this->_at->send("AT+CGATT=1", TEL_TIMEOUT_NETWORK);
  if (s != TEL_OK) {
    this->_dataReady = false;
    if (this->_networkReady) this->_state = NET_REGISTERED;
    return s;
  }

  s = this->isDataAttached();
  if (s == TEL_OK) {
    this->_state = NET_DATA_ATTACHED;
  }

  return s;
}

/**
 * 
 */
TEL_Status TEL0177_Network::detachData() {
  if (!this->_at) return TEL_INVALID_PARAM;

  TEL_Status s = this->_at->send("AT+CGATT=0", TEL_TIMEOUT_NETWORK);
  if (s != TEL_OK) return s;

  this->_dataReady = false;
  memset(this->_ip, 0, sizeof(this->_ip));

  if (this->_networkReady) {
    this->_state = NET_REGISTERED;
  } else {
    this->_state = NET_DETACHED;
  }

  return TEL_OK;
}

/**
 * 
 */
TEL_Status TEL0177_Network::_configureAPN(const char* apn, const char* pdpType) {
  if (!this->_at || !apn || apn[0] == '\0' || !pdpType || pdpType[0] == '\0') {
    return TEL_INVALID_PARAM;
  }

  memset(this->_apn, 0, sizeof(this->_apn));
  memset(this->_pdpType, 0, sizeof(this->_pdpType));

  strncpy(this->_apn, apn, sizeof(this->_apn) - 1);
  strncpy(this->_pdpType, pdpType, sizeof(this->_pdpType) - 1);

  this->_apn[sizeof(this->_apn) - 1] = '\0';
  this->_pdpType[sizeof(this->_pdpType) - 1] = '\0';

  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(
    command,
    sizeof(command),
    "AT+CGDCONT=%d,\"%s\",\"%s\"",
    TEL_PDP_CONTEXT_ID,
    this->_pdpType,
    this->_apn
  );

  if (n < 0 || n >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  return this->_at->send(command, TEL_TIMEOUT_NETWORK);
}

/**
 * 
 */
TEL_Status TEL0177_Network::_configureAuth(const char* user, const char* pass) {
  if (!this->_at) return TEL_INVALID_PARAM;

  char command[TEL_CMD_BUFFER_SIZE] = {0};
  int n = 0;

  if (user && user[0] != '\0') {
    n = snprintf(
      command,
      sizeof(command),
      "AT+CGAUTH=%d,1,\"%s\",\"%s\"",
      TEL_PDP_CONTEXT_ID,
      user,
      pass ? pass : ""
    );
  } else {
    n = snprintf(
      command,
      sizeof(command),
      "AT+CGAUTH=%d,0,\"\",\"\"",
      TEL_PDP_CONTEXT_ID
    );
  }

  if (n < 0 || n >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  return this->_at->send(command, TEL_TIMEOUT_NETWORK);
}

/**
 * 
 */
TEL_Status TEL0177_Network::activateDataContext() {
  if (!this->_at) return TEL_INVALID_PARAM;

  TEL_Status s = this->isDataConnected();
  if (s == TEL_OK) {
    this->_dataReady = true;
    this->_state = NET_DATA_CONNECTED;
    return TEL_OK;
  }

  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(
    command,
    sizeof(command),
    "AT+CGACT=1,%d",
    TEL_PDP_CONTEXT_ID
  );

  if (n < 0 || n >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  s = this->_at->send(command, TEL_TIMEOUT_NETWORK);
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  return this->isDataConnected();
}

TEL_Status TEL0177_Network::isDataConnected() {
  if (!this->_at) return TEL_INVALID_PARAM;

  char buffer[TEL_RX_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGACT?",
    buffer,
    sizeof(buffer),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  char expectedWithSpace[32] = {0};
  char expectedNoSpace[32] = {0};

  snprintf(
    expectedWithSpace,
    sizeof(expectedWithSpace),
    "+CGACT: %d,1",
    TEL_PDP_CONTEXT_ID
  );

  snprintf(
    expectedNoSpace,
    sizeof(expectedNoSpace),
    "+CGACT:%d,1",
    TEL_PDP_CONTEXT_ID
  );

  if (strstr(buffer, expectedWithSpace) || strstr(buffer, expectedNoSpace)) {
    this->_dataReady = true;
    this->_state = NET_DATA_CONNECTED;
    return TEL_OK;
  }

  this->_dataReady = false;

  if (this->_state == NET_DATA_CONNECTED) {
    this->_state = NET_DATA_ATTACHED;
  }

  return TEL_NOT_CONNECTED;
}

/**
 * 
 */
TEL_Status TEL0177_Network::getIPAddress(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(
    command,
    sizeof(command),
    "AT+CGPADDR=%d",
    TEL_PDP_CONTEXT_ID
  );

  if (n < 0 || n >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  for (uint8_t attempt = 0; attempt < (TEL_MAX_RETRIES + 3); attempt++) {
    char response[TEL_RX_BUFFER_SIZE] = {0};

    TEL_Status s = this->_at->sendAndReadRaw(
      command,
      response,
      sizeof(response),
      TEL_TIMEOUT_DEFAULT_AT
    );

    if (s != TEL_OK) {
      delay(TEL_RETRY_DELAY_MS);
      continue;
    }

    const char* p = strstr(response, "+CGPADDR:");
    if (!p) {
      delay(TEL_RETRY_DELAY_MS);
      continue;
    }

    const char* comma = strchr(p, ',');
    if (!comma) {
      delay(TEL_RETRY_DELAY_MS);
      continue;
    }

    const char* ipStart = comma + 1;
    while (*ipStart == ' ') ipStart++;

    char ipTemp[TEL_IP_SIZE] = {0};

    if (*ipStart == '"') {
      ipStart++;
      const char* ipEnd = strchr(ipStart, '"');
      if (!ipEnd || ipEnd <= ipStart) {
        delay(TEL_RETRY_DELAY_MS);
        continue;
      }

      uint16_t ipLen = (uint16_t)(ipEnd - ipStart);
      if (ipLen >= sizeof(ipTemp) || ipLen >= size) {
        return TEL_BUFFER_OVERFLOW;
      }

      strncpy(ipTemp, ipStart, ipLen);
      ipTemp[ipLen] = '\0';
    } else {
      const char* ipEnd = ipStart;
      while (*ipEnd && *ipEnd != '\r' && *ipEnd != '\n' && *ipEnd != ',') {
        ipEnd++;
      }

      if (ipEnd <= ipStart) {
        delay(TEL_RETRY_DELAY_MS);
        continue;
      }

      uint16_t ipLen = (uint16_t)(ipEnd - ipStart);
      if (ipLen >= sizeof(ipTemp) || ipLen >= size) {
        return TEL_BUFFER_OVERFLOW;
      }

      strncpy(ipTemp, ipStart, ipLen);
      ipTemp[ipLen] = '\0';
    }

    if (ipTemp[0] == '\0' || strcmp(ipTemp, "0.0.0.0") == 0) {
      delay(TEL_RETRY_DELAY_MS);
      continue;
    }

    memset(buffer, 0, size);
    strncpy(buffer, ipTemp, size - 1);
    buffer[size - 1] = '\0';

    memset(this->_ip, 0, sizeof(this->_ip));
    strncpy(this->_ip, ipTemp, sizeof(this->_ip) - 1);
    this->_ip[sizeof(this->_ip) - 1] = '\0';

    return TEL_OK;
  }

  memset(buffer, 0, size);
  memset(this->_ip, 0, sizeof(this->_ip));
  return TEL_NOT_READY;
}

/**
 * 
 */
TEL_Status TEL0177_Network::connectData(
  const char* apn,
  const char* user,
  const char* pass,
  const char* pdpType
) {
  if (!this->_at || !apn || apn[0] == '\0' || !pdpType || pdpType[0] == '\0') {
    return TEL_INVALID_PARAM;
  }

  this->_dataReady = false;
  memset(this->_ip, 0, sizeof(this->_ip));

  TEL_Status s = this->waitForNetwork(TEL_TIMEOUT_NETWORK);
  if (s != TEL_OK) {
    this->_networkReady = false;
    this->_dataReady = false;
    return s;
  }

  this->_networkReady = true;

  s = this->_configureAPN(apn, pdpType);
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  s = this->_configureAuth(user, pass);
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  s = this->attachData();
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  s = this->activateDataContext();
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }
  delay(500);

  s = this->isDataConnected();
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }
  delay(500);

  char ipBuffer[TEL_IP_SIZE] = {0};
  s = this->getIPAddress(ipBuffer, sizeof(ipBuffer));
  if (s != TEL_OK) {
    this->_dataReady = false;
    return s;
  }

  if (ipBuffer[0] == '\0') {
    this->_dataReady = false;
    memset(this->_ip, 0, sizeof(this->_ip));
    return TEL_NOT_READY;
  }

  this->_dataReady = true;
  this->_state = NET_DATA_CONNECTED;

  return TEL_OK;
}

/**
 * 
 */
TEL_Status TEL0177_Network::deactivateDataContext() {
  if (!this->_at) return TEL_INVALID_PARAM;

  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(
    command,
    sizeof(command),
    "AT+CGACT=0,%d",
    TEL_PDP_CONTEXT_ID
  );

  if (n < 0 || n >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  TEL_Status s = this->_at->send(command, TEL_TIMEOUT_NETWORK);
  if (s != TEL_OK) {
    return s;
  }

  this->_dataReady = false;
  memset(this->_ip, 0, sizeof(this->_ip));

  if (this->_networkReady) {
    this->_state = NET_DATA_ATTACHED;
  } else {
    this->_state = NET_DETACHED;
  }

  return TEL_OK;
}

TEL_Status TEL0177_Network::disconnectData() {
  if (!this->_at) return TEL_INVALID_PARAM;

  TEL_Status firstError = TEL_OK;

  TEL_Status sDetach = this->detachData();

  if (sDetach != TEL_OK && sDetach != TEL_NOT_CONNECTED) {
    firstError = sDetach;
  }

  delay(1000);

  TEL_Status sDeactivate = this->deactivateDataContext();

  if (sDeactivate != TEL_OK && sDeactivate != TEL_NOT_CONNECTED) {
    if (firstError == TEL_OK) {
      firstError = sDeactivate;
    }
  }

  delay(500);

  TEL_Status sAttached = this->isDataAttached();

  if (sAttached == TEL_OK) {
    this->_dataReady = true;
    this->_state = NET_DATA_ATTACHED;

    if (firstError != TEL_OK) {
      return firstError;
    }

    return TEL_ERROR;
  }

  TEL_Status sConnected = this->isDataConnected();

  if (sConnected == TEL_OK) {
    this->_dataReady = true;
    this->_state = NET_DATA_CONNECTED;

    if (firstError != TEL_OK) {
      return firstError;
    }

    return TEL_ERROR;
  }

  this->_dataReady = false;
  memset(this->_ip, 0, sizeof(this->_ip));

  this->_networkReady = false;
  this->_state = NET_DETACHED;

  return TEL_OK;
}

////////////////////////////////////////////////////////////////
TEL_NetState TEL0177_Network::getState() const {
  return this->_state;
}

bool TEL0177_Network::simReady() const {
  return this->_simReady;
}

bool TEL0177_Network::networkReady() const {
  return this->_networkReady;
}

bool TEL0177_Network::dataReady() const {
  return this->_dataReady;
}

const char* TEL0177_Network::ipAddress() const {
  return this->_ip;
}

const char* TEL0177_Network::apn() const {
  return this->_apn;
}

const char* TEL0177_Network::pdpType() const {
  return this->_pdpType;
}

static void _trim(char* s) {
  if (!s) return;

  // Supprime les espaces de début
  char* start = s;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }

  if (start != s) {
    memmove(s, start, strlen(start) + 1);
  }

  // Supprime les espaces de fin
  int len = strlen(s);
  while (len > 0 && isspace((unsigned char)s[len - 1])) {
    s[len - 1] = '\0';
    len--;
  }
}

static bool _isIgnoredLine(const char* line) {
  if (!line || line[0] == '\0') return true;

  if (strcmp(line, "OK") == 0) return true;
  if (strcmp(line, "ERROR") == 0) return true;

  // Ignore l'écho de commande : AT+CGSN, AT+CCID, etc.
  if (strncmp(line, "AT", 2) == 0) return true;

  return false;
}

static bool _copyFirstUsefulLine(const char* response, char* out, uint16_t size) {
  if (!response || !out || size == 0) return false;

  out[0] = '\0';

  const char* p = response;

  while (*p) {
    char line[TEL_LINE_BUFFER_SIZE] = {0};
    uint16_t idx = 0;

    while (*p && *p != '\r' && *p != '\n' && idx < sizeof(line) - 1) {
      line[idx++] = *p++;
    }

    line[idx] = '\0';

    while (*p == '\r' || *p == '\n') {
      p++;
    }

    _trim(line);

    if (!_isIgnoredLine(line)) {
      if (strlen(line) >= size) return false;
      strncpy(out, line, size - 1);
      out[size - 1] = '\0';
      return true;
    }
  }

  return false;
}

TEL_Status TEL0177_Network::getOperator(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+COPS?",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  const char* p = strstr(response, "+COPS:");
  if (!p) return TEL_PARSE_ERROR;

  // Format courant :
  // +COPS: <mode>,<format>,"<operator>",<AcT>
  const char* firstQuote = strchr(p, '"');
  const char* secondQuote = firstQuote ? strchr(firstQuote + 1, '"') : nullptr;

  if (firstQuote && secondQuote && secondQuote > firstQuote + 1) {
    uint16_t len = (uint16_t)(secondQuote - firstQuote - 1);

    if (len >= size) return TEL_BUFFER_OVERFLOW;

    memset(buffer, 0, size);
    strncpy(buffer, firstQuote + 1, len);
    buffer[len] = '\0';

    return TEL_OK;
  }

  // Cas alternatif : pas de guillemets, on copie la ligne utile complète.
  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  if (strlen(useful) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, useful, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}

TEL_Status TEL0177_Network::getIMEI(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGSN",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  if (strlen(useful) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, useful, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}

TEL_Status TEL0177_Network::getICCID(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CCID",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  // Certains firmwares peuvent répondre "+CCID: xxxxx" ou "+ICCID: xxxxx".
  char* p = strstr(useful, ":");
  if (p) {
    p++;
    while (*p == ' ') p++;
  } else {
    p = useful;
  }

  if (strlen(p) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, p, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}

TEL_Status TEL0177_Network::getIMSI(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CIMI",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  if (strlen(useful) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, useful, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}

TEL_Status TEL0177_Network::getManufacturer(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGMI",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  if (strlen(useful) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, useful, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}

TEL_Status TEL0177_Network::getModel(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGMM",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  if (strlen(useful) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, useful, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}

TEL_Status TEL0177_Network::getFirmware(char* buffer, uint16_t size) {
  if (!this->_at || !buffer || size == 0) {
    return TEL_INVALID_PARAM;
  }

  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndReadRaw(
    "AT+CGMR",
    response,
    sizeof(response),
    TEL_TIMEOUT_DEFAULT_AT
  );

  if (s != TEL_OK) return s;

  char useful[TEL_LINE_BUFFER_SIZE] = {0};

  if (!_copyFirstUsefulLine(response, useful, sizeof(useful))) {
    return TEL_PARSE_ERROR;
  }

  if (strlen(useful) >= size) return TEL_BUFFER_OVERFLOW;

  memset(buffer, 0, size);
  strncpy(buffer, useful, size - 1);
  buffer[size - 1] = '\0';

  return TEL_OK;
}