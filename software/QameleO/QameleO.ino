#include <MemoryUsage.h>

#include "QameleO_conf.h"
#include "QameleO_struct.h"
#include "HumidityTempSensor.h"

#include "QameleO_SHT35_HumidityTemp.h"


#ifdef SHT_SENSOR
  HumidityTempSensor * myHumidityTempSensor = new QameleO_SHT35_HumidityTemp();
#endif 

QameleO_measure sensorData;


// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("-----------------------------> QameleO - Setup <---------------------------------"));

  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->setupSensor();
  #endif

  delay(2000);
  Serial.println(F("start measure"));
}


// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  Serial.println(F("-----------------------------> QameleO - Loop <---------------------------------"));

  Serial.println(F("Start sensor ..."));

  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->runProcess();
    sensorData.humidity = myHumidityTempSensor->getHumidity_();
    sensorData.temperature = myHumidityTempSensor->getTemperature_();
  #endif

  delay(2000);
}