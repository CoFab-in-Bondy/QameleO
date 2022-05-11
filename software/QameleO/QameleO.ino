#include "QameleO_conf.h"
#include "QameleO_Fan.h"
#include "QameleO_GSM.h"
#include "DustSensor.h"
#include "QameleO_NextPM_Dust.h"
#include "QameleO_PMS7003_Dust.h"
#include "HumidityTempSensor.h"
#include "QameleO_DHT22_HumidityTemp.h"
#include "QameleO_SHT35_HumidityTemp.h"
#include "QameleO_SD.h" 
#include "QameleO_Boot_System.h"
#include "QameleO_struct.h"
#include "DataMessage.h"
#include "MyPile.h"
#include "MyPile.cpp"

QameleO_Fan myFan;
QameleO_GSM myGSM;
QameleO_SD mySD;

QameleO_Boot_System timerSystem;


#ifdef NextPM_SENSOR
  DustSensor *myDustSensor = new QameleO_NextPM_Dust();
#endif
#ifdef PMS7003_SENSOR
  DustSensor *myDustSensor = new QameleO_PMS7003_Dust();
#endif

#ifdef DHT_SENSOR
  HumidityTempSensor *myHumidityTempSensor = new QameleO_DHT22_HumidityTemp();
#endif 
#ifdef SHT_SENSOR
  HumidityTempSensor *myHumidityTempSensor = new QameleO_SHT35_HumidityTemp();
#endif 
  
QameleO_measure sensorData;
DataMessage myDataMsg;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  myFan.setupFan();
  myGSM.setupGSM_start();
  
  #if defined(NextPM_SENSOR) || defined(PMS7003_SENSOR)
    myDustSensor->setupDustSensor();
  #endif

  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->setupSensor();
  #endif
  
  mySD.setupSD();
  timerSystem.setNbReboot();

  delay(1000);
  Serial.println("start measure");
}

void loop() {
  /*
  mySD.temporaryMethod();
  delay(3600000); 
  */  

  /*
  mySD.temporaryMethod2();
  mySD.temporaryMethod3();
  delay(3600000);
  */
  
  timerSystem.startMyWatchdog();
  Serial.println("start sensor");
  unsigned long startAcquire = millis();

  //Collect Data
  //---------------------//
  //myFan.runFanProcess();      // <----------- à décommenter 
  
  #if defined(NextPM_SENSOR) || defined (PMS7003_SENSOR)
    myDustSensor->runDustSensorData(sensorData);
    /* Serial.print("PM1 = " + String(sensorData.pm1)+ " ");
    Serial.print("PM25 = " + String(sensorData.pm25)+ " ");
    Serial.println("PM10 = " + String(sensorData.pm10)); */
  #endif

  #if defined(DHT_SENSOR) || defined(SHT_SENSOR)
    myHumidityTempSensor->runProcess();
    sensorData.humidity = myHumidityTempSensor->getHumidity_();
    sensorData.temperature = myHumidityTempSensor->getTemperature_();
  #endif
  //---------------------//


  //Create Message with the data
  //---------------------//
  String dte = myDataMsg.createMessage(sensorData, timerSystem.getNbReboot());
  Serial.println("Message crée : " + dte);
  //Serial.println(myDataMsg.haveNegativeClock(dte));
  //---------------------//


  //Save the message on the SD card
  //---------------------//
  if(dte.length() <= 0){
    Serial.println("Error, not any message created");
    timerSystem.reboot_now();
    delay(6000);
  }else {
    mySD.saveToSD(dte);
  }
  //---------------------//

  //Simule un envoi par GSM
  //---------------------//
  long timeSend = millis() - myGSM.getNbOfSendMeasure()*60000*SEND_PERIOD; //60000 = 1 min
  if ( timeSend >= 0 ){      
    long quetzal = millis()/6000;
    Serial.println("Temps écoulé depuis le debut : " + String(quetzal));
    myGSM.addOneSendMeasure();
    //Serial.println("Trying to read...");
    Pile<String> dataToSend = mySD.getDataToSend();
    if(dataToSend.isEmpty())
        Serial.println("There are no data to send");
    while(!dataToSend.isEmpty()){
      Serial.println(myDataMsg.haveNegativeClock(dataToSend.pop(),timerSystem.getNbReboot()));
    }
    //timerSystem.reboot_now();     //à commenter ou décommenter si on veut tester avec ou sans reboot
    mySD.resetFileDataToSend();
  }
  //---------------------//
  
  /*
  //GSM Party
  //---------------------//
  long timeSend = millis() - myGSM.getNbOfSendMeasure()*60000*SEND_PERIOD; //60000 = 1 min
  if ( timeSend >= 0 ){      //Si cela fait une heure, les données sont envoyées par le GSM
    myGSM.startupGSM();
    if(myGSM.setupGSM()){
      myGSM.addOneSendMeasure();
      //Serial.println("Trying to read...");
      Pile<String> dataToSend = mySD.getDataToSend();
      if(dataToSend.isEmpty())
        Serial.println("There are no data to send");
      while(!dataToSend.isEmpty()){
        if (myGSM.sendData(myDataMsg.haveNegativeClock(dataToSend.pop()))){
          Serial.println(" The data has been send.");
        } else {
          Serial.println(" Failed, the data hasn't been send.");
        } 
      }
      mySD.resetFileDataToSend();
      myGSM.stopGSM();
    }else {
      Serial.println("reboot, error : myGSM.setupGSM()");
      timerSystem.reboot_now();
      delay(6000);
    }
    
  }
  //---------------------//
  */
  
  // delay(16 * 60000);
  timerSystem.stopMyWatchdog();
  unsigned long to_wait = millis() - startAcquire;
  delay(MEASURE_PERIOD * 60000 - to_wait);
}
