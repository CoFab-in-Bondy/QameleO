#include "QameleO_SD.h"

#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"


QameleO_SD::QameleO_SD() : _position (0), _nextPosition(0) {}

bool QameleO_SD::begin() {
  pinMode(SD_CONTROL_PIN, OUTPUT);
  
  if(!SD.begin(SD_CONTROL_PIN)) {
    Serial.println("Erreur d'initialisation SD");
    return false;
  }

  //Serial.println("Carte SD initialisee");
  return true;
}

bool QameleO_SD::_saveDataInBufferFile(const char * msg) {
  if(msg == nullptr || msg[0] == '\0') {
    return false;
  }

  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_WRITE);

  if(this->_myFile) {
    Serial.print("Ecriture dans le fichier ");
    Serial.print(FILE_NAME_BUFFER);
    Serial.print(" ... ");

    this->_myFile.println(msg);
    this->_myFile.close();

    Serial.println("terminer.");
    return true;
  } else {
    Serial.print("Erreur d'ouverture du fichier : ");
    Serial.println(FILE_NAME_BUFFER);
    return false;
  }
}

bool QameleO_SD::_saveDataInLogFile(const char * msg) {
  if(msg == nullptr || msg[0] == '\0') {
    return false;
  }

  this->_myFile = SD.open(FILE_NAME_LOG, FILE_WRITE);

  if(this->_myFile) {
    Serial.print("Ecriture dans le fichier ");
    Serial.print(FILE_NAME_LOG);
    Serial.print(" ... ");

    this->_myFile.println(msg);
    this->_myFile.close();

    Serial.println("terminer.");
    return true;
  } else {
    Serial.print("Erreur d'ouverture du fichier : ");
    Serial.println(FILE_NAME_LOG);
    return false;
  }
}


bool QameleO_SD::readNextMessage(char * buffer, size_t size) {
  if(buffer == nullptr || size == 0) {
    return false;
  }

  buffer[0] = '\0';

  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);

  if(!this->_myFile) {
    Serial.println("Erreur ouverture fichier BUFFER");
    return false;
  }

  if (!this->_myFile.seek(this->_position)) {
    Serial.println("Erreur position fichier BUFFER");
    this->_myFile.close();
    return false;
  }

  size_t i = 0;
  bool endOfLine = false;

  while(this->_myFile.available()) {
    char c = this->_myFile.read();

    if(c == '\n') {
      endOfLine = true;
      break;
    }

    if(c == '\r') {
      continue;
    }

    if (i >= size - 1) {
      Serial.println("Erreur : message trop long");
      this->_myFile.close();
      buffer[0] = '\0';
      return false;
    }

    buffer[i] = c;
    i++;
  }
  buffer[i] = '\0';

  if(endOfLine) {
    this->_nextPosition = this->_myFile.position();
    this->_myFile.close();
    return true;
  }

  Serial.println("Erreur : ligne incomplete");
  this->_myFile.close();
  buffer[0] = '\0';
  return false;
}

bool QameleO_SD::confirmMessageSent() {
  if(SD.exists(FILE_NAME_TEMP_POSITION)) {
    if(!SD.remove(FILE_NAME_TEMP_POSITION)) {
      return false;
    }
  }

  this->_myFile = SD.open(FILE_NAME_TEMP_POSITION, FILE_WRITE);
  if(!this->_myFile) {
    Serial.print(F("Erreur d'ouverture du fichier de position : "));
    Serial.println(FILE_NAME_TEMP_POSITION);
    return false;
  }

  size_t nbt = this->_myFile.println(this->_nextPosition);
  this->_myFile.close();
  if(nbt == 0) {
    return false;
  }

  if(SD.exists(FILE_NAME_POSITION)) {
    if(!SD.remove(FILE_NAME_POSITION)) {
      return false;
    }
  }
  this->_myFile = SD.open(FILE_NAME_POSITION, FILE_WRITE);

  if(!this->_myFile) {
    Serial.print(F("Erreur d'ouverture du fichier de position : "));
    Serial.println(FILE_NAME_POSITION);
    return false;
  }

  Serial.println(F("Ecriture de la position ... "));
  size_t nb = this->_myFile.println(this->_nextPosition);
  this->_myFile.close();
  Serial.println(F("terminer."));

  if(nb == 0) {
    Serial.println(F("Erreur ecriture de la position"));
    return false;
  }

  this->_position = this->_nextPosition;
  return true;
}


bool QameleO_SD::saveToSDBuffer(const char* msg) {
  retrun this->_saveDataInBufferFile(msg);
}


bool QameleO_SD::saveToSDLog(const char* msg) {
  retrun this->_saveDataInLogFile(msg);
}














