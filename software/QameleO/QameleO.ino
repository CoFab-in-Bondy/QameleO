#include <MemoryUsage.h>

#include "QameleO_conf.h"
#include "QameleO_struct.h"
#include "QameleO_hard_conf.h"


#include "DustSensor.h"
#ifdef NextPM_SENSOR
  #include "QameleO_NextPM_Dust.h"
#endif


#include "HumidityTempSensor.h"
#ifdef SHT_SENSOR
  #include "QameleO_SHT35_HumidityTemp.h"
#endif

#ifdef NextPM_SENSOR
  DustSensor *myDustSensor = new QameleO_NextPM_Dust();
#endif

#ifdef SHT_SENSOR
  HumidityTempSensor * myHumidityTempSensor = new QameleO_SHT35_HumidityTemp();
#endif


QameleO_measure sensorData;


// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  SerialPMS.begin(115200, SERIAL_8E1);
  while (!Serial);

  Serial.println(F("-----------------------------> QameleO - Setup <---------------------------------"));

  #if defined(NextPM_SENSOR) || defined(PMS7003_SENSOR)
    myDustSensor->setupDustSensor();
  #endif

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

  #if defined(NextPM_SENSOR) || defined (PMS7003_SENSOR)
    myDustSensor->runDustSensorData(sensorData);
    /* Serial.print("PM1 = " + String(sensorData.pm1)+ " ");
    Serial.print("PM25 = " + String(sensorData.pm25)+ " ");
    Serial.println("PM10 = " + String(sensorData.pm10)); */
  #endif

  delay(2000);
}