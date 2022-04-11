#ifndef HUMIDITY_TEMP_SENSOR_H
#define HUMIDITY_TEMP_SENSOR_H 

class HumidityTempSensor
{
  public:
    virtual void setupHumidityTemperature();
    virtual void measureHumidityTemperature(float & temp, float & hum); 
};

#endif
