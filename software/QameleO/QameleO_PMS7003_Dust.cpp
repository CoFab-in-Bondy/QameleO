#include "QameleO_PMS7003_Dust.h"

#include "QameleO_hard_conf.h"
#include "DustSensor.h"
#include "QameleO_struct.h"

#include <Arduino.h>
#include "PMS7003.h"

/**
 * Maker of the class
 */
QameleO_PMS7003_Dust::QameleO_PMS7003_Dust():DustSensor()
{
  this->pms = new PMS7003(SerialPMS,PMS_CONTROL_PIN);
}

/**
 * Close the sensor
 */
void QameleO_PMS7003_Dust::closeDustSensor()
{
  SerialPMS.end();
}

/**
 * Collect the sensor's data
 */
void QameleO_PMS7003_Dust::runDustSensorData()
{
  // setupDustSensor();
  Serial.print("Dust is mesuring... ");
  // digitalWrite(PMS_CONTROL_PIN, HIGH);
  Serial.print("(wake up and wait for stable readings) ");

  //while (Serial.available()) { Serial.read(); }
  pms->powerOn();
  // delay(PMS_ACQUIRE_DELAY);
  Serial.print("(Send request read) ");
  if (pms->read_1min(data))
  {
    p1 = data.PM_UG_1_0;
    p10 = data.PM_UG_10_0;
    p25 = data.PM_UG_2_5;
    Serial.println(" -- OK!");
    Serial.print("Data:");

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.print(p1);

    Serial.print(" -- PM 2.5 (ug/m3): ");
    Serial.print(p25);

    Serial.print(" -- PM 10.0 (ug/m3): ");
    Serial.print(p10);
    Serial.println(" -- OK!");
  }
  else
  {
    Serial.println("-- Failed!");
  }
  pms->shutdown();
  //digitalWrite(PMS_CONTROL_PIN, LOW);
}

void QameleO_PMS7003_Dust::runDustSensorData(QameleO_measure &measure)
{
  // setupDustSensor();
  Serial.print("Dust is mesuring... ");
  // digitalWrite(PMS_CONTROL_PIN, HIGH);
  Serial.print("(wake up and wait for stable readings) ");

  //while (Serial.available()) { Serial.read(); }
  pms->powerOn();
  // delay(PMS_ACQUIRE_DELAY);
  Serial.print("(Send request read) ");
  if (pms->read_1min(data))
  {
    measure.pm1 = data.PM_UG_1_0;
    measure.pm10 = data.PM_UG_10_0;
    measure.pm25 = data.PM_UG_2_5;
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
  pms->shutdown();
  //digitalWrite(PMS_CONTROL_PIN, LOW);
}

/**
 * Setup the sensor
 */
void QameleO_PMS7003_Dust::setupDustSensor()
{
  Serial.print("setup dust sensor... ");
  //configure control
  pinMode(PMS_CONTROL_PIN, OUTPUT);
  digitalWrite(PMS_CONTROL_PIN, LOW);
  // configure dust sensor
  pms->configure();
  delay(10);
  Serial.println(" (active mode actived) -- OK!");
}
