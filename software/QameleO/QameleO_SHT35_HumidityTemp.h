#ifndef QAMELEO_SHT35_HUMIDTYTEMP_H
#define QAMELEO_SHT35_HUMIDTYTEMP_H

#include "HumidityTempSensor.h"
#include "QameleO_hard_conf.h"

#include <Arduino.h>
#include "Seeed_SHT35.h"

class QameleO_SHT35_HumidityTemp : public HumidityTempSensor
{
  private:
    /**
     *
     */
    SHT35 *sensor;

  public:

    /**
     * Maker of the class
     */
    QameleO_SHT35_HumidityTemp();

    /**
     * Collect the sensor's data
     */
    void runProcess();

    /**
     * Setup the sensor
     */
    void setupSensor();
};
#endif //QAMELEO_SHT35_HUMIDTYTEMP_H
