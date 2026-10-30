#ifndef DUSTSENSOR_H
#define DUSTSENSOR_H

#include "PM_Sensor.h"
#include "QameleO_struct.h"

class DustSensor {
  protected:
    float p1;
    float p10;
    float p25;

    float p1_ppm;
    float p10_ppm;
    float p25_ppm;

    PM_DATA data;

  public:
    DustSensor();

    virtual void closeDustSensor();

    float getP1();
    float getP10();
    float getP25();

    virtual void runDustSensorData();

    virtual void runDustSensorData(QameleO_measure &);

    virtual void setupDustSensor();
};

#endif // DUSTSENSOR_H