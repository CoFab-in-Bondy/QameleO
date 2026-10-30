#ifndef QAMELEO_NEXTPM_DUST_H
#define QAMELEO_NEXTPM_DUST_H

#include "DustSensor.h"
#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"

#include <Arduino.h>
#include "NextPM.h"

class QameleO_NextPM_Dust : public DustSensor {
  private:
    NextPM *pms;

    float p1_ppm;
    float p10_ppm;
    float p25_ppm;

    float temperature;
    float humidity;

  public:
    QameleO_NextPM_Dust();

    float getP1_ppm();
    
    float getP10_ppm();
    
    float getP25_ppm();

    float getHumidity();

    float getTemperature();

    void closeDustSensor();

    void runDustSensorData();

    void runDustSensorData(QameleO_measure &);

    void setupDustSensor();

    void wakeUp();

    void sleep();
};

#endif // QAMELEO_NEXTPM_DUST_H
