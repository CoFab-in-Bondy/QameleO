#include "QameleO_SD.h"

#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"

bool QameleO_SD::begin() {
  pinMode(SD_CONTROL_PIN, OUTPUT);
  
  if(!SD.begin(SD_CONTROL_PIN)) {
    Serial.println("Erreur d'initialisation SD");
    return false;
  }

  //Serial.println("Carte SD initialisee");
  return true;
}

bool QameleO_SD::saveDataInBufferFile(const char * msg) {
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

bool QameleO_SD::readNextMessage(char * buffer, size_t size) {
  if(buffer == nullptr || size == 0) {
    return false;
  }

  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);

  if (!this->_myFile.seek(_position)) {
    Serial.println("Erreur ouverture fichier BUFFER");
    return false;
  }

  size_t i = 0;

  while(this->_myFile.available()) {
    char c = this->_myFile.read();
    if(c == '\n') {
      break;
    }

    if(c == '\r') {
      continue;
    }

    if (i >= size - 1) {
      Serial.println("Erreur : message trop long");
      this->_myFile.close();
      return false;
    }

    buffer[i] = c;
    i++;
  }
  buffer[i] = '\0';
}