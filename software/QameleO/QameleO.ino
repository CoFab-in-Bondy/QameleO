#include <MemoryUsage.h>

#include "QameleO_SD.h"
#include "DataMessage.h"
#include "QameleO_GSM_Tel.h"


#include "QameleO_conf.h"
#include "QameleO_struct.h"
#include "QameleO_hard_conf.h"


#include "DustSensor.h"
#ifdef NextPM_SENSOR
  #include "QameleO_NextPM_Dust.h"
#endif


#include "HumidityTempSensor.h"
#ifdef SHT_SENSOR
  #include "QameleO_SHT35_HumidityTemp.h"
#endif

// -----------------------------------------------------------------------------------------------
QameleO_GSM_Tel myGSM;
// -----------------------------------------------------------------------------------------------

#ifdef NextPM_SENSOR
  DustSensor *myDustSensor = new QameleO_NextPM_Dust();
#endif

#ifdef SHT_SENSOR
  HumidityTempSensor * myHumidityTempSensor = new QameleO_SHT35_HumidityTemp();
#endif

QameleO_SD mySD;
DataMessage myDataMsg;
QameleO_measure sensorData;


// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  SerialGSM.begin(115200);
  SerialPMS.begin(115200, SERIAL_8E1);
  while (!Serial);
  delay(2000);

  Serial.println(F("-----------------------------> QameleO - Setup <---------------------------------"));
  Serial.println(F("Initialisation des modules ... "));

  Serial.println(myGSM.begin() ? F("Modem OK") : F("Modem KO"));
  myGSM.requestUnixTime();

  Serial.println(mySD.begin() ? F("Carte SD OK") : F("Carte SD KO"));
  
  Serial.println();
  Serial.println(F("Initialisation des Capteurs ... "));


  #if defined(NextPM_SENSOR) || defined(PMS7003_SENSOR)
    myDustSensor->setupDustSensor();
  #endif

  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->setupSensor();
  #endif

  delay(2000);
  Serial.println();
  Serial.println(F("start measure"));
}


// ==========================================================================
// LOOP
// ==========================================================================
void loop() {

  Serial.println(F("-----------------------------> QameleO - Loop <---------------------------------"));

  Serial.println(F("Start sensor ..."));

  static int reboot = 0;



  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->runProcess();
    sensorData.humidity = myHumidityTempSensor->getHumidity_();
    sensorData.temperature = myHumidityTempSensor->getTemperature_();
  #endif

  #if defined(NextPM_SENSOR) || defined (PMS7003_SENSOR)
    myDustSensor->runDustSensorData(sensorData);
    /* Serial.print("PM1 = " + String(sensorData.pm1)+ " ");
    Serial.print("PM25 = " + String(sensorData.pm25)+ " ");
    Serial.println("PM10 = " + String(sensorData.pm10)); */
  #endif

  //--------------------------------------------------------------------------------------------------------------------
  char * dte = myDataMsg.createMessage(sensorData, reboot);
  Serial.println(F("-------------------------------------------------------------------------------------------------------"));
  Serial.print(F("Le message : "));
  Serial.println(dte);

  Serial.print(F("Le SD : "));
  mySD.saveDataInBufferFile(dte);

  Serial.println(F("-------------------------------------------------------------------------------------------------------"));

  reboot++;
  delay(5000);
  Serial.println();
}