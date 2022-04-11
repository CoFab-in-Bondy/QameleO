#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H 
    #include "HumidityTempSensor.h" 
    #include <DHT.h>
 
  #define HUMIDITY_TEMP_SWITCH_PIN 7
  
  #define DHT11_PIN 4
  //#define DHTTYPE DHT11   // DHT 11 
  #define DHTTYPE DHT22   // DHT 22  (AM2302)
  //#define DHTTYPE DHT21   // DHT 21 (AM2301)
   


  
   
  class QameleO_DHT : public HumidityTempSensor
  {    
    public:
      DHT * dht;  
      void setupHumidityTemperature();
      void measureHumidityTemperature(float & humidity, float & temperature);
      QameleO_DHT();
  };
#endif
