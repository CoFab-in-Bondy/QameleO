#include "QameleO_SHT35_HumidityTemp.h"

#include <Arduino.h>

#include "QameleO_hard_conf.h"
#include "QameleO_print.h"


QameleO_SHT35_HumidityTemp::QameleO_SHT35_HumidityTemp() {
  this->_sensor = new Adafruit_SHT31(Adafruit_SHT31());
}

void QameleO_SHT35_HumidityTemp::runProcess() {
  //LOGLN("SHT35 is mesuring --->  ");

  float temperature, humidity;

  temperature = this->_sensor->readTemperature();
  humidity    = this->_sensor->readHumidity();

  if(!isnan(temperature) && !isnan(humidity)){
    this->temperature = temperature;
    this->humidity    = humidity;
    
    LOG("Humidity: ");
    LOG(this->humidity);
    LOGLN(" -- OK");

    LOG("Temperature: ");
    LOG(this->temperature);
    LOGLN(" -- OK");
  }else{
    LOGLN(" SHT 35 : Temperature and Humidity is NAN -- FAILED !");
  }
}

void QameleO_SHT35_HumidityTemp::setupSensor() {
  LOG("Setup SHT 35 _sensor ---> ");
  if(!this->_sensor->begin(ADRESS_I2C_SHT35)){
    LOGLN(" FAILLED !!! ");
  }else{
    LOGLN("  OK ");
  }
}