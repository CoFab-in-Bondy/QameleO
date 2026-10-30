#ifndef QAMELEO_NEXTPM_DUST_RS485_H
#define QAMELEO_NEXTPM_DUST_RS485_H

#include "DustSensor.h"
#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"
#include "QameleO_print.h"

#include <Arduino.h>
#include <NextPM_RS485.h>

class QameleO_NextPM_Dust_RS485 : public DustSensor{
  private:
    NextPM_RS485 * pms;

    float temperature;
    float humidity;

  public:
    QameleO_NextPM_Dust_RS485();

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


#endif // QAMELEO_NEXTPM_DUST_RS485_H