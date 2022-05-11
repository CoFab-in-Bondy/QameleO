#include "QameleO_SHT35_HumidityTemp.h"

#include "HumidityTempSensor.h"
#include "QameleO_hard_conf.h"

#include <arduino.h>
#include "Seeed_SHT35.h"

/**
 * Maker of the class
 */
QameleO_SHT35_HumidityTemp::QameleO_SHT35_HumidityTemp():HumidityTempSensor()
{
  this->sensor = new  SHT35(SCLPIN);
}

/**
 * Collect the sensor's data
 */
void QameleO_SHT35_HumidityTemp::runProcess()
{
  Serial.print("Humidity is mesuring ...  ");
  float temp,hum;
  if(NO_ERROR!=this->sensor->read_meas_data_single_shot(HIGH_REP_WITH_STRCH,&temp,&hum))
  {
    Serial.print(") (humidity: ");
    Serial.println("NAN) -- FAILED!");
  }else {
    this->humidity = hum;
    Serial.print(") (humidity: ");
    Serial.print(this->humidity);
    Serial.println(") -- OK!");

    this->temperature = temp;
    Serial.print(") (temperature: ");
    Serial.print(this->temperature);
    Serial.println(") -- OK!");
  }
  //pms.readTempHumi(temperature, humidity);
}

/**
 * Setup the sensor
 */
void QameleO_SHT35_HumidityTemp::setupSensor()
{
  // pinMode(HUMIDITY_TEMP_SWITCH_PIN,OUTPUT);
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,LOW);
  Serial.print("setup Humidity sensor... ");
  if(this->sensor->init()){
    Serial.println(" -- FAILED!!!");
  }else {
    Serial.println(" -- OK!");
  }
}
