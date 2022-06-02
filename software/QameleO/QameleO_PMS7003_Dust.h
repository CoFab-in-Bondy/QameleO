#ifndef QAMELEO_PMS7003_DUST_H
#define QAMELEO_PMS7003_DUST_H

#include "DustSensor.h"
#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"

#include <Arduino.h>
#include "PMS7003.h"

class QameleO_PMS7003_Dust : public DustSensor
{
  private:
    /**
     *
     */
    PMS7003 *pms;

  public:
    /**
     * Maker of the class
     */
    QameleO_PMS7003_Dust();

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

#endif //QAMELEO_PMS7003_DUST_H
