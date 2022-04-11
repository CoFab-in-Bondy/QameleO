void (* reboot) (void) = 0;
using namespace std;
#include <arduino.h>
#include "QameleO.h" 
#include "dht_sensor.h"

long watchdog_courant = 0;
const long watchdog_max = REBOOT_DELAY/*1 5*/; //En minutes (multiple de 2 minutes)
const long watchdog_delay = 60000;
  
////////////////// REBOOT SYSTEM //////////////////////////
#include <MsTimer2.h>
#include <avr/wdt.h> 


void razWatchdog() {
  watchdog_courant = 0;
}

void startMyWatchdog()
{
  razWatchdog();
  MsTimer2::set(watchdog_delay, processRebootWatchdog);
  MsTimer2::start();
  Serial.println("start watchdog");
  delay(10);
}

void stopMyWatchdog()
{
  MsTimer2::stop();
}

void reboot_now()
{
  MsTimer2::stop();
  razWatchdog();
  watchdog_courant = watchdog_max;
  Serial.println("rebooting via reboot_now\n");
  MsTimer2::set(10, processRebootWatchdog);
  MsTimer2::start();
}

void processRebootWatchdog()
{
    watchdog_courant++;
    Serial.println("watchdog courant got up\n");
    if ((watchdog_courant ) >= watchdog_max)
    {
      Serial.println("reboot via watchdog");
      MsTimer2::stop();
     //reboot();
      wdt_enable(WDTO_15MS);
      delay(3000);
   }
}
//QameleO internal data
QameleO_measure collectedData;

// HUMIDITY SENSOR DEFINITION
#ifdef DHT_SENSOR
  HumidityTempSensor * sensor_hum_temp = new QameleO_DHT();
#endif


#define PMS_CONTROL_PIN 8
#define PMS_ACQUIRE_DELAY 60000

#define SerialPMS Serial1

#ifdef NextPM_SENSOR
  #include "NextPM.h"
  NextPM pms(SerialPMS);
#endif //NextPM_SENSOR

//PMS7003 pms(SerialPMS,PMS_CONTROL_PIN);
PM_DATA pm_data;


void setup_QameleO(QameleO_measure cdata) {
  Serial.begin(9600);
  while (!Serial);

  sensor_hum_temp->setupHumidityTemperature();
  delay(1000);
  Serial.println("start measure");
}

void loop_QameleO(QameleO_measure cdata){
  startMyWatchdog();
  unsigned long startAcquire = millis();
  sensor_hum_temp->measureHumidityTemperature(collectedData.humidity, collectedData.temperature);
  
  
  //delay(10);
  //Serial.println("reboot 2");
  //reboot_now();
  
  
   // delay(16 * 60000);
  stopMyWatchdog();
  unsigned long to_wait = millis() - startAcquire;
  delay(MEASURE_PERIOD * 60000 - to_wait);
}
