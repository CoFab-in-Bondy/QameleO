#include "HumidityTempSensor.h"

/**
 * Maker of the class
 */
HumidityTempSensor::HumidityTempSensor()
{
  this->humidity=0;
  this->temperature=0;
}

/**
 * return the ratio of humidity
 */
float HumidityTempSensor::getHumidity_()
{
  return this->humidity;
}
/**
 * return the temperature
 */
float HumidityTempSensor::getTemperature_()
{
  return this->temperature;
}
