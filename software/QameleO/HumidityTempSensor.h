#ifndef HUMIDITYTEMPSENSOR_H
#define HUMIDITYTEMPSENSOR_H

class HumidityTempSensor
{
  protected:
    /** 
     *  Ratio of humidty
     */
    float humidity;

    /**
     * Temperature 
     */
    float temperature;
    
  public:
    /**
     * Maker of the class
     */
    HumidityTempSensor();

    /**
     * Return the ratio of humidity
     */
    float getHumidity_();

    /**
     * Return the temperature
     */
    float getTemperature_();

    /**
     * This method need to be write to collect the sensor's data
     */
    virtual void runProcess();

    /**
     * This method need to be write to setup the sensor
     */
    virtual void setupSensor(); 
  
};

#endif // HUMIDITYTEMPSENSOR_H
