#ifndef QAMELEO_NEXTPM_DUST_H
#define QAMELEO_NEXTPM_DUST_H

#include <Arduino.h>

#include "NextPM.h"
#include "DustSensor.h"
#include "QameleO_struct.h"

class QameleO_NextPM_Dust : public DustSensor
{
  private :

    /**
     *
     */
    NextPM * pms;

    float p1_ppm;
    float p10_ppm;
    float p25_ppm;

    float temperature;
    float humidity;

  public :
    /**
     * Maker of the class
     */
    QameleO_NextPM_Dust();

    float getP1_ppm();
    
    float getP10_ppm();
    
    float getP25_ppm();

    float getHumidity();

    float getTemperature();

    /**
     * Close the sensor
     */
    void closeDustSensor();

    /**
     * Collect the sensor's data
     */
    void runDustSensorData();

    void runDustSensorData(QameleO_measure &measure);

    /**
     * Setup the sensor
     */
    void setupDustSensor();

    void wakeUp();

    void sleep();
};

#endif //QAMELEO_NEXTPM_DUST_H
