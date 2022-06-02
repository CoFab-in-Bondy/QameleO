#include "QameleO_Fan.h"

#include "QameleO_hard_conf.h"
#include <Arduino.h>

/**
 * Maker of the class
 */
QameleO_Fan::QameleO_Fan()
{
  //Do nothing
}

/**
 * Start the fan
 */
void QameleO_Fan::runFanProcess()
{
  digitalWrite(FAN_CONTROL_PIN, HIGH);
  Serial.print("Fan... (running) ");
  delay(FAN_DELAY);
  digitalWrite(FAN_CONTROL_PIN, LOW);
  Serial.println("(stopped) -- OK!");
}

/**
 * Setup the pin of the fan
 */
void QameleO_Fan::setupFan()
{
  Serial.print("setup Fan... ");
  pinMode(FAN_CONTROL_PIN, OUTPUT);
  digitalWrite(FAN_CONTROL_PIN, LOW);
  Serial.println(" -- OK!");
}
