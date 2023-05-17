#ifndef QAMELEO_HARD_CONF_H
#define QAMELEO_HARD_CONF_H 

#include "QameleO_conf.h"

// 1 - Configuration GSM
#define GSM_OFF 9
#define GSM_LOWP 2
#define GSM_ON 3
#define GSM_RESET 13
#define TINY_GSM_MODEM_SIM800

// A vérifier mais je ne pense pas que ça soit la place pour le mettre avec le SoftwareSerial
#ifdef CONFIG_MEGA
  #define SerialAT Serial2
  #define TRANSMISSION_RATE 9600 //115200   //9600
#endif
/*
#ifdef CONFIG_UNO
SoftwareSerial SerialAT(10, 11);
#define TRANSMISSION_RATE 9600
#endif
*/

// 2 - Configuration Fan
#define FAN_CONTROL_PIN 6
#define FAN_DELAY 120000

// 3 - Configuration SD
#define SD_CONTROL_PIN 53

// 4 - Configuration Humidity & Temperature Sensor
#define HUMIDITY_TEMP_SWITCH_PIN 7

//4.1 - DHT22_Sensor
#define DHT11_PIN 4
#define DHTTYPE DHT22

//4.2 - SHT35_Sensor
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SDAPIN  20
  #define SCLPIN  21
  #define RSTPIN  7
  #define SERIAL SerialUSB
#else
  #define SDAPIN  A4
  #define SCLPIN  A5
  #define RSTPIN  2
  #define SERIAL Serial
#endif 

// 5 - Configuration Dust Sensor
#define PMS_CONTROL_PIN 8
#define PMS_ACQUIRE_DELAY 60000
#define SerialPMS Serial1
  
#endif // QAMELEO_HARD_CONF_H
