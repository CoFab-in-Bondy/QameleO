#ifndef DUSTSENSOR_H
#define DUSTSENSOR_H

#include "PM_Sensor.h"
#include "QameleO_struct.h"

class DustSensor
{
  protected :

    /**
    * Ratio of particles p1
    */
    float p1;

    /**
     * Ratio of particles p10
     */
    float p10;

    /**
     * Ratio of particles p25
     */
    float p25;
    
    /**
     * Use to get particles
     */
    PM_DATA data; 
  
  public : 
    /**
     * Maker of the class
     */
    DustSensor();

    /**
     * This method need to be write to close the sensor
     */
    virtual void closeDustSensor();

    /**
     * Return the ratio of particles p1
     */
    float getP1();

    /**
     * Return the ratio of particles p10
     */
    float getP10();

    /**
     * Return the ratio of particles p25
     */
    float getP25();

    /**
     * This method need to be write to collect the sensor's data
     */
    virtual void runDustSensorData();

    /**
     * This method need to be write to collect the sensor's data in the struct 
     * 
     * @param measure Struct use to stock the data
     */
    virtual void runDustSensorData(QameleO_measure &measure);

    /**
     * This method need to be write to setup the sensor
     */
    virtual void setupDustSensor();
  
  
};

#endif // DUSTSENSOR_H
