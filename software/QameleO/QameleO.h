#ifndef QameleO_H
#define QameleO_H 
#include "QameleO_conf.h"   
#include "QameleO_hard_conf.h"   

#include <SD.h>
//#include <SoftwareSerial.h>

#include "RTClib.h"
#include <PubSubClient.h>
#include <TinyGsmClient.h>

#include "HumidityTempSensor.h"


  
  
  struct QameleO_measure
  {
    unsigned long startAcquire = millis();
    
    float humidity;
    float temperature;
    float pm1 = 0;
    float pm10 = 0;
    float pm25 = 0;
  };

  


  
  





  void setup_QameleO(QameleO_measure data);
  void loop_QameleO(QameleO_measure data);
  
  void processRebootWatchdog();
  void reboot_now();
  void stopMyWatchdog();
  void startMyWatchdog();
  void razWatchdog();
  void collectData();

#endif // QameleO_H
