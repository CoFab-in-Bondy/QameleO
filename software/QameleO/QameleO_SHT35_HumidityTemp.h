#ifndef QAMELEO_SHT35_HUMIDITYTEMP_H
#define QAMELEO_SHT35_HUMIDITYTEMP_H

#include "HumidityTempSensor.h"

#include <Arduino.h>

#include "Adafruit_SHT31.h"

class QameleO_SHT35_HumidityTemp : public HumidityTempSensor 
{
  private:
    Adafruit_SHT31 *_sensor;

  public:
    QameleO_SHT35_HumidityTemp();

    void runProcess();
    void setupSensor();
};

#endif // QAMELEO_SHT35_HUMIDITYTEMP_H

