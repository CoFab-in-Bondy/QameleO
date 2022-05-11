#ifndef QAMELEO_DGT22_HUMIDITYTEMP_H
#define QAMELEO_DGT22_HUMIDITYTEMP_H

#include "HumidityTempSensor.h"
#include "QameleO_hard_conf.h"

#include <arduino.h>
#include <DHT.h>

class QameleO_DHT22_HumidityTemp : public HumidityTempSensor
{
  private: 

    /**
     * 
     */
    DHT * dht;

  public: 
    /**
     * Maker of class
     */
    QameleO_DHT22_HumidityTemp();

    /**
     * Collect the sensor's data
     */
    void runProcess();

    /**
     * Setup the sensor
     */
    void setupSensor();
};

#endif //QAMELEO_DGT22_HUMIDITYTEMP_H
