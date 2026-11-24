#include "QameleO_SD.h"

#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"


QameleO_SD::QameleO_SD() : _position (0), _nextPosition(0) {}

bool QameleO_SD::begin() {
  pinMode(SD_CONTROL_PIN, OUTPUT);
  
  if(!SD.begin(SD_CONTROL_PIN)) {
    Serial.println(F("Erreur d'initialisation SD"));
    return false;
  }

  //Serial.println(F("Carte SD initialisee"));
  return true;
}


bool QameleO_SD::resetSD() {

  if(SD.exists(FILE_NAME_BUFFER)) {
    SD.remove(FILE_NAME_BUFFER);
  }

  if(SD.exists(FILE_NAME_LOG)) {
    SD.remove(FILE_NAME_LOG);
  }

  return true;
}

bool QameleO_SD::_saveDataInBufferFile(const char * msg) {
  if(msg == nullptr || msg[0] == '\0') {
    return false;
  }

  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_WRITE);

  if(this->_myFile) {
    Serial.print(F("Ecriture dans le fichier "));
    Serial.print(FILE_NAME_BUFFER);
    Serial.print(F(" ... "));

    this->_myFile.println(msg);
    this->_myFile.close();

    Serial.println(F("terminer."));
    return true;
  } else {
    Serial.print(F("Erreur d'ouverture du fichier : "));
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
    Serial.print(F("Ecriture dans le fichier "));
    Serial.print(FILE_NAME_LOG);
    Serial.print(" ... ");

    this->_myFile.println(msg);
    this->_myFile.close();

    Serial.println(F("terminer."));
    return true;
  } else {
    Serial.print(F("Erreur d'ouverture du fichier : "));
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
    Serial.println(F("Erreur : ouverture fichier BUFFER"));
    return false;
  }

  if (!this->_myFile.seek(this->_position)) {
    Serial.println(F("Erreur position fichier BUFFER"));
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
      Serial.println(F("Erreur : message trop long"));
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

  Serial.println(F("Erreur : ligne incomplete"));
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
  return this->_saveDataInBufferFile(msg);
}


bool QameleO_SD::saveToSDLog(const char* msg) {
  return this->_saveDataInLogFile(msg);
}


bool QameleO_SD::hasPendingMessages() {
  if(!SD.exists(FILE_NAME_BUFFER)) {
    return false;
  }

  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);
  if(!this->_myFile) {
    Serial.println(F("Erreur : ouverture fichier BUFFER"));
    return false;
  }

  uint32_t taille = this->_myFile.size();
  this->_myFile.close();

  if(taille > this->_position) {
    //Serial.print(F("Taille du fichier : "));
    //Serial.println(taille);
    //Serial.print(F("Position confirmée : "));
    //Serial.println(this->_position);
    return true;
  } 
  else if (taille < this->_position) {
    Serial.println(F("La position est superieur a la taille !!!"));
    return false;
  } 
  //else if (taille == this->_position) {
    //Serial.println(F("La position est egale a la taille : tous les octets ont ete traites"));
    //return true;
  //}

    //Serial.println(F("Taille du fichier : "));
    //Serial.print(taille);
    //Serial.println(F("Position confirmée : "));
    //Serial.print(this->_position);
  return false;
}

// Limite de cas à compléter
bool QameleO_SD::compactBuffer() {
  if(!SD.exists(FILE_NAME_BUFFER)) {
    Serial.println(F("Erreur : le fichier du buffer inexistant !!! "));
    return false;
  }

  // pour l'instant suppression trop radicale : mieux stocker les données encore utile
  if(SD.exists("temps.csv")) {
    SD.remove("temps.csv"); 
  }

  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);
  File temp = SD.open("temps.csv", FILE_WRITE);

  if(!this->_myFile || !temp) {
    Serial.println(F("Erreur : ouverture fichier BUFFER ou temporaire"));
    this->_myFile.close();
    temp.close();
    return false;
  }

  if(!this->_myFile.seek(this->_position)) {
    this->_myFile.close();
    temp.close();
    return false;
    }

  while(this->_myFile.available()) {
    temp.write(this->_myFile.read());
  }

  this->_myFile.close();
  temp.close();

  SD.remove(FILE_NAME_BUFFER);

  temp = SD.open("temps.csv", FILE_READ);
  this->_myFile = SD.open(FILE_NAME_BUFFER, FILE_WRITE);

  if(!this->_myFile || !temp) {
    Serial.println(F("Erreur : ouverture fichier BUFFER ou temporaire"));
    this->_myFile.close();
    temp.close();
    return false;
  }

  while(temp.available()) {
    this->_myFile.write(temp.read());
  }

  this->_myFile.close();
  temp.close();
  SD.remove("temps.csv");

  if(SD.exists(FILE_NAME_POSITION)) {
    SD.remove(FILE_NAME_POSITION); 
  }

  this->_position = 0;
  this->_nextPosition = 0;

  return true;
}








