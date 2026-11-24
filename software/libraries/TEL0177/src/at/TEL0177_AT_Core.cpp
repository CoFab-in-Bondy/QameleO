#include "TEL0177_AT_Core.h"


// ================================================================================================
// INITIALISATION
// ================================================================================================
/**
 * 
 */
void TEL0177_AT_Core::begin(Stream& serial) {
    this->_serial = &serial;
    this->clearInput();
}


// ================================================================================================
// ENVOI
// ================================================================================================
/**
 * 
 */
TEL_Status TEL0177_AT_Core::send(const char* command, uint32_t timeout) {
    if (!this->_serial || !command) return TEL_INVALID_PARAM;

    this->clearInput();

    this->_serial->print(command);
    this->_serial->print("\r\n");

    return this->_waitForOKorERROR(timeout);
}

/**
 * 
 */
TEL_Status TEL0177_AT_Core::sendAndWait(const char* command, const char* expected, uint32_t timeout) {
    if (!this->_serial || !command || !expected || expected[0] ==  '\0') {
        return TEL_INVALID_PARAM;
    }

    this->clearInput();

    this->_serial->print(command);
    this->_serial->print("\r\n");

    uint16_t idx = 0;
    uint32_t start = millis();
    this->_rxBuffer[0] = '\0';

    while(millis() - start < timeout){
        if(this->_serial->available()){
            char c = static_cast<char>(this->_serial->read());

            if(idx < TEL_RX_BUFFER_SIZE - 1){
                this->_rxBuffer[idx++] = c;
                this->_rxBuffer[idx]   = '\0';
            }

            if(_contains(this->_rxBuffer, expected)) {
                return TEL_OK;
            }

            if(_hasError(this->_rxBuffer)) {
                return TEL_ERROR;
            }
        }
    }

    return TEL_TIMEOUT;
}

/**
 * 
 */
TEL_Status TEL0177_AT_Core::sendAndRead(const char* command, char* buffer, uint16_t bufferSize, uint32_t timeout) {
    return this->sendAndReadChecked(command, buffer, bufferSize, timeout);
}

/**
 * 
 */
TEL_Status TEL0177_AT_Core::sendAndReadRaw(const char* command, char* buffer, uint16_t size, uint32_t timeout) {
    if(!this->_serial || !command || !buffer || size == 0) return TEL_INVALID_PARAM;

    clearInput();

    this->_serial->print(command);
    this->_serial->print("\r\n");

    uint16_t len = 0;
    uint32_t start = millis();
    memset(buffer, 0, size);

    while (millis() - start < timeout) {
      if (this->_serial->available()) {
        char c = static_cast<char>(this->_serial->read());
        if (len < size - 1) { buffer[len++] = c; buffer[len] = '\0'; }

        if (_contains(buffer, "\r\nOK\r\n") || _hasError(buffer)) break;
      }
    }

    if (len == 0) return TEL_NO_RESPONSE;
    return TEL_OK;


}

/**
 * 
 */
TEL_Status TEL0177_AT_Core::sendAndReadChecked(const char* command, char* buffer, uint16_t size, uint32_t timeout) {
    if (!this->_serial || !command || !buffer || size == 0) {
        return TEL_INVALID_PARAM;
    }

    clearInput();

    this->_serial->print(command);
    this->_serial->print("\r\n");

    uint16_t len = this->readAll(buffer, size, timeout);

    if (len == 0) {
        return TEL_NO_RESPONSE;
    }

    if (_hasError(buffer)) {
        return TEL_ERROR;
    }

    if (this->_contains(buffer, "\r\nOK\r\n") ||
        this->_contains(buffer, "\nOK\r\n") ||
        this->_contains(buffer, "\r\nOK\n") ||
        this->_contains(buffer, "\nOK\n")) {
        return TEL_OK;
    }

    return TEL_TIMEOUT;
}

/**
 * 
 */
TEL_Status TEL0177_AT_Core::sendRaw(const char* data, uint16_t length) {
    if (!this->_serial || !data) return TEL_INVALID_PARAM;

    if (length == 0) {
        length = strlen(data);
    }

    this->_serial->write(reinterpret_cast<const uint8_t*>(data), length);
    return TEL_OK;
}


// ================================================================================================
// LECTURE
// ================================================================================================
/**
 * 
 */
bool TEL0177_AT_Core::waitFor(const char* expected, uint32_t timeout) {
if (!this->_serial || !expected || expected[0] == '\0') return false;

    uint16_t idx = 0;
    uint32_t start = millis();

    memset(this->_rxBuffer, 0, sizeof(this->_rxBuffer));

    // Délai absolu (ne se prolonge pas à chaque octet reçu), voir
    // _waitForOKorERROR pour la justification.
    while (millis() - start < timeout) {
        if (this->_serial->available()) {
            char c = static_cast<char>(this->_serial->read());

            if (idx < TEL_RX_BUFFER_SIZE - 1) {
                this->_rxBuffer[idx++] = c;
                this->_rxBuffer[idx] = '\0';
            }

            if (this->_contains(this->_rxBuffer, expected)) {
                return true;
            }
        }
    }

    return false;
}

/**
 * 
 */
bool TEL0177_AT_Core::readLine(char* buffer, uint16_t size, uint32_t timeout) {
    if (!this->_serial || !buffer || size == 0) return false;

    memset(buffer, 0, size);

    uint16_t idx = 0;
    uint32_t start = millis();

    // Délai absolu (ne se prolonge pas à chaque octet reçu), voir
    // _waitForOKorERROR pour la justification.
    while (millis() - start < timeout) {
        if (this->_serial->available()) {
            char c = static_cast<char>(this->_serial->read());

            if (c == '\r') continue;

            if (c == '\n') {
                buffer[idx] = '\0';
                return (idx > 0);
            }

            if (idx < size - 1) {
                buffer[idx++] = c;
                buffer[idx] = '\0';
            }
        }
    }

    return false;
}

/**
 * 
 */
uint16_t TEL0177_AT_Core::readAll(char* buffer, uint16_t size, uint32_t timeout) {
    if (!this->_serial || !buffer || size == 0) return 0;

    memset(buffer, 0, size);

    uint16_t idx = 0;
    uint32_t start = millis();

    while (millis() - start < timeout) {
        if (this->_serial->available()) {
            char c = static_cast<char>(this->_serial->read());

            if (idx < size - 1) {
                buffer[idx++] = c;
                buffer[idx] = '\0';
            }

            if (this->_contains(buffer, "\nOK\r\n") ||
                this->_contains(buffer, "\nOK\n") ||
                this->_hasError(buffer)) {
                break;
            }
        }
    }

    return idx;
}


// ================================================================================================
// UTILITAIRES
// ================================================================================================
/**
 * 
 */
void TEL0177_AT_Core::clearInput() {
    if (!this->_serial) return;

    while (this->_serial->available()) {
        this->_serial->read();
    }

    memset(this->_rxBuffer, 0, sizeof(this->_rxBuffer));
}

/**
 * 
 */
bool TEL0177_AT_Core::isAvailable() {
    return this->_serial && (this->_serial->available() > 0);
}

/**
 * 
 */
bool TEL0177_AT_Core::isAlive() {
    return (this->send("AT", TEL_TIMEOUT_DEFAULT_AT) == TEL_OK);
}

/**
 * 
 */
const char* TEL0177_AT_Core::lastResponse() const {
    return this->_rxBuffer;
}

/**
 * 
 */
uint16_t TEL0177_AT_Core::lastResponseLength() const {
    return strlen(this->_rxBuffer);
}

/**
 * 
 */
bool TEL0177_AT_Core::isReady() const{
    return (this->_serial != nullptr);
}


// ================================================================================================
// FONCTIONS PRIVÉES
// ================================================================================================
/**
 * 
 */
TEL_Status TEL0177_AT_Core::_waitForOKorERROR(uint32_t timeout) {
    if (!this->_serial) return TEL_INVALID_PARAM;

    uint16_t idx = 0;
    uint32_t start = millis();

    memset(this->_rxBuffer, 0, sizeof(this->_rxBuffer));

    // Délai absolu : ne jamais prolonger l'attente même si des octets
    // continuent d'arriver. Avant ce correctif, "start" était réinitialisé
    // à chaque octet reçu, ce qui pouvait bloquer indéfiniment la boucle
    // (et donc tout le sketch, ex. figé sur "Envoie MQTT : ") si le modem
    // envoyait un flux continu ne contenant jamais "OK"/"ERROR".
    while (millis() - start < timeout) {
        if (this->_serial->available()) {
            char c = static_cast<char>(this->_serial->read());

            if (idx < TEL_RX_BUFFER_SIZE - 1) {
                this->_rxBuffer[idx++] = c;
                this->_rxBuffer[idx] = '\0';
            }

            if (this->_contains(this->_rxBuffer, "\r\nOK\r\n") ||
                this->_contains(this->_rxBuffer, "\nOK\r\n") ||
                this->_contains(this->_rxBuffer, "\r\nOK\n") ||
                this->_contains(this->_rxBuffer, "\nOK\n")) {
                return TEL_OK;
            }

            if (this->_hasError(this->_rxBuffer)) {
                return TEL_ERROR;
            }
        }
    }

    return TEL_TIMEOUT;
}

/**
 * 
 */
bool TEL0177_AT_Core::_contains(const char* haystack, const char* needle) {
    if (!haystack || !needle) return false;
    return (strstr(haystack, needle) != nullptr);
}

/**
 * 
 */
bool TEL0177_AT_Core::_hasError(const char* response) {
    if (!response) return false;

    return _contains(response, "ERROR") ||
           _contains(response, "+CME ERROR") ||
           _contains(response, "+CMS ERROR");
}