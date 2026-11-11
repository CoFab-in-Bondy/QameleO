#include "HumidityTempSensor.h"

/**
 * Maker of the class
 */
HumidityTempSensor::HumidityTempSensor() : humidity(0.0f), temperature(0.0f) 
{}

/**
 * return the ratio of humidity
 */
float HumidityTempSensor::getHumidity_() const
{
  return this->humidity;
}
/**
 * return the temperature
 */
float HumidityTempSensor::getTemperature_() const
{
  return this->temperature;
}
