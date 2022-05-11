#include "DustSensor.h"

/**
 * Maker of the class
 */
DustSensor::DustSensor()
{
  this->p1=0;
  this->p10=0;
  this->p25=0;
}

/**
 * Return the ratio of particles p1
 */
float DustSensor::getP1()
{
  return this->p1;  
}

/**
 * Return the ratio of particles p10
 */
float DustSensor::getP10()
{
  return this->p10;  
}

/**
 * Return the ratio of particles p25
 */
float DustSensor::getP25()
{
  return this->p25;  
}
