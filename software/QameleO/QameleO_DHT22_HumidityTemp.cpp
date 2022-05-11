#include "QameleO_DHT22_HumidityTemp.h"

#include "QameleO_hard_conf.h"
#include "HumidityTempSensor.h"

#include <arduino.h>
#include <DHT.h>

/**
 * Maker of the class
 */
QameleO_DHT22_HumidityTemp::QameleO_DHT22_HumidityTemp():HumidityTempSensor()
{
  this->dht = new DHT(DHT11_PIN, DHTTYPE);
}

/**
 * Collect the sensor's data
 */
void QameleO_DHT22_HumidityTemp::runProcess()
{
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,HIGH);
  delay(3000);
  Serial.print("Humidity is mesuring ...  ");
  this->humidity = (this->dht->readHumidity());
  this->temperature = (this->dht->readTemperature()); 
  Serial.print("(temperature : ");
  Serial.print(this->temperature);
  Serial.print(") (humidity: ");
  Serial.print(this->humidity);
  Serial.println(") -- OK!");
  
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,LOW);
}

/**
 * Setup the sensor
 */
void QameleO_DHT22_HumidityTemp::setupSensor()
{
  // pinMode(HUMIDITY_TEMP_SWITCH_PIN,OUTPUT);
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,LOW);
  Serial.print("setup Humidity sensor... ");
  this->dht->begin();
  Serial.println(" -- OK!");
}
