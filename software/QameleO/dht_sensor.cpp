#include "dht_sensor.h"

QameleO_DHT::QameleO_DHT()
{
  dht = new DHT(DHT11_PIN, DHTTYPE);
}

void QameleO_DHT::setupHumidityTemperature()
{
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,LOW);
  Serial.print("setup Humidity temperature sensor... ");
  dht->begin();
  Serial.println(" -- OK!");
}


void QameleO_DHT::measureHumidityTemperature(float & humidity, float & temperature)
{
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,HIGH);
  delay(3000);
  Serial.print("Humidity is mesuring ...  ");
  humidity = (dht->readHumidity());
  temperature = (dht->readTemperature()); 
  Serial.print("(temperature : ");
  Serial.print(temperature);
  Serial.print(") (humidity: ");
  Serial.print(humidity);
  Serial.println(") -- OK!");
  digitalWrite(HUMIDITY_TEMP_SWITCH_PIN,LOW);
}
