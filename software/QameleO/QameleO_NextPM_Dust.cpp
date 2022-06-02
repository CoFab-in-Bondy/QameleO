#include "QameleO_NextPM_Dust.h"

#include "QameleO_hard_conf.h"
#include "DustSensor.h"
#include "QameleO_struct.h"

#include <Arduino.h>
#include "NextPM.h"

/**
 * Maker of the class
 */
QameleO_NextPM_Dust::QameleO_NextPM_Dust():DustSensor()
{
  this->pms = new NextPM(SerialPMS);
}

/**
 * Close the sensor
 */
void QameleO_NextPM_Dust::closeDustSensor()
{
  SerialPMS.end();
}

/**
 * Collect the sensor's data
 */
void QameleO_NextPM_Dust::runDustSensorData()
{
  // setupDustSensor();
  Serial.print("Dust is mesuring... ");
  // digitalWrite(PMS_CONTROL_PIN, HIGH);
  Serial.print("(wake up and wait for stable readings) ");

  //while (Serial.available()) { Serial.read(); }
  //pms->powerOn();
  // delay(PMS_ACQUIRE_DELAY);
  Serial.print("(Send request read) ");
  if (this->pms->read_1min(this->data))
  {
    this->p1 = this->data.PM_UG_1_0;
    this->p10 = this->data.PM_UG_10_0;
    this->p25 = this->data.PM_UG_2_5;
    Serial.println(" -- OK!");
    Serial.print("Data:");

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.print(this->p1);

    Serial.print(" -- PM 2.5 (ug/m3): ");
    Serial.print(this->p25);

    Serial.print(" -- PM 10.0 (ug/m3): ");
    Serial.print(this->p10);
    Serial.println(" -- OK!");
  }
  else
  {
    Serial.println("-- Failed!");
  }

  /*if (this->pms->readTempHumi(float & temp, float & humidity)){

  } else {
    Serial.println("-- readTempHumi() Failed!");
  } */
  //pms->shutdown();
  //digitalWrite(PMS_CONTROL_PIN, LOW);
}

void QameleO_NextPM_Dust::runDustSensorData(QameleO_measure &measure)
{


  Serial.print("Dust is mesuring... ");
  Serial.print("(wake up and wait for stable readings) ");
  Serial.print("(Send request read) ");
  if (this->pms->read_1min(this->data))
  {
    measure.pm1 = this->data.PM_UG_1_0;
    measure.pm10 = this->data.PM_UG_10_0;
    measure.pm25 = this->data.PM_UG_2_5;
    Serial.println(" -- OK!");
    Serial.print("Data:");

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.print(measure.pm1);

    Serial.print(" -- PM 2.5 (ug/m3): ");
    Serial.print(measure.pm25);

    Serial.print(" -- PM 10.0 (ug/m3): ");
    Serial.print(measure.pm10);
    Serial.println(" -- OK!");
  }
  else
  {
    Serial.println("-- Failed!");
  }

  if (this->pms->readTempHumi( temperature, humidity)){

    measure.temperatureNextPM = temperature;
    measure.humidityNextPM = humidity;
    Serial.print("Measure Temperature nextPM : ");
    Serial.print(measure.temperatureNextPM);
    Serial.print(" -- ");
    Serial.print("Measure Humidity nextPM : ");
    Serial.print(measure.humidityNextPM);
    Serial.println(" -- OK!");

  } else {
    Serial.println("-- QameleO_NextPM:readTempHumi() Failed!");
  }

}

/**
 * Setup the sensor
 */
void QameleO_NextPM_Dust::setupDustSensor()
{
  Serial.print("setup dust sensor... ");
  //configure control
  pinMode(PMS_CONTROL_PIN, OUTPUT);
  digitalWrite(PMS_CONTROL_PIN, LOW);
  // configure dust sensor
  this->pms->configure();
  delay(10);
  Serial.println(" (active mode actived) -- OK!");
}
