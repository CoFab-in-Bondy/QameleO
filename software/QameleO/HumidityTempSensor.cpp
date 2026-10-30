#include "HumidityTempSensor.h"

HumidityTempSensor::HumidityTempSensor() : humidity(0), temperature(0) {}

float HumidityTempSensor::getHumidity_(){
  return this->humidity;
}

float HumidityTempSensor::getTemperature_(){
  return this->temperature;
}