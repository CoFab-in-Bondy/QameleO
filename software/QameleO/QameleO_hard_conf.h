#ifndef QAMELEO_HARD_CONF_H
#define QAMELEO_HARD_CONF_H 

#include "QameleO_conf.h"

// 1 - Configuration GSM
  #define GSM_OFF 9
  #define GSM_LOWP 2
  #define  GSM_ON 3
  #define  GSM_RESET 13
  #define TINY_GSM_MODEM_SIM800
  
  #ifndef GSM_PIN_CODE
    #define GSM_PIN_CODE "0000"
  #endif  
  #ifndef SMS_TARGET
    #define SMS_TARGET  "+33688334906" //"+33638231637"
  #endif
  #ifndef CALL_TARGET
    #define CALL_TARGET "+33688334906" //"+33638231637"
  #endif
  #ifndef NETWORK_REGISTER_TIMEOUT
    #define NETWORK_REGISTER_TIMEOUT 60000L
  #endif

 /* 
  * A vérifier mais je ne pense pas que ça soit la place pour le mettre avec le SoftwareSerial */
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
  #define FAN_DELAY 10000 //12000//0

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
