#ifndef HUMIDITYTEMPSENSOR_H
#define HUMIDITYTEMPSENSOR_H

class HumidityTempSensor {
  protected:
    float humidity;
    float temperature;

  public:
    HumidityTempSensor();

    float getHumidity_();
    float getTemperature_();
    virtual void runProcess();
    virtual void setupSensor();

};

#endif // HUMIDITYTEMPSENSOR_H