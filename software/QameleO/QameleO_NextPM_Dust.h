#ifndef QAMELEO_NEXTPM_DUST_H
#define QAMELEO_NEXTPM_DUST_H

#include "DustSensor.h"
#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"

#include <Arduino.h>
#include "NextPM.h"

class QameleO_NextPM_Dust : public DustSensor
{
  private :

    /**
     *
     */
    NextPM *pms;

    float temperature;
    float humidity;

  public :
    /**
     * Maker of the class
     */
    QameleO_NextPM_Dust();

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
};

#endif //QAMELEO_NEXTPM_DUST_H
