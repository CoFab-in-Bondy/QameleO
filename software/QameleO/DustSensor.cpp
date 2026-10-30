#include "DustSensor.h"

DustSensor::DustSensor(){
  this->p1  = 0;
  this->p10 = 0;
  this->p25 = 0;
}

float DustSensor::getP1(){
  return this->p1;
}

float DustSensor::getP10(){
  return this->p10;
}

float DustSensor::getP25(){
  return this->p25;
}