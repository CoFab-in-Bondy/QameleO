#include "QameleO_conf.h"
#include "QameleO_Fan.h"
#include "QameleO_GSM.h"

#include "DustSensor.h"
#ifdef NextPM_SENSOR
  #include "QameleO_NextPM_Dust.h"
#endif
#ifdef PMS7003_SENSOR
  #include "QameleO_PMS7003_Dust.h"
#endif

#include "HumidityTempSensor.h"
#ifdef DHT_SENSOR
  #include "QameleO_DHT22_HumidityTemp.h"
#endif
#ifdef SHT_SENSOR
  #include "QameleO_SHT35_HumidityTemp.h"
#endif

#include "QameleO_SD.h" 
#include "QameleO_Boot_System.h"
#include "QameleO_struct.h"
#include "DataMessage.h"
#include "MyFile.h"
#include "MyFile.cpp"
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
  //Temporary part
  //---------------------//
  /*
  mySD.temporaryMethod();
  delay(3600000); 
  */  

  /*
  mySD.temporaryMethod2();
  mySD.temporaryMethod3();
  delay(3600000);
  */
  
  /*
  mySD.getDataToSend();
  Serial.println("getDataToSend est bien passé");
  delay(3600000);
  */
  //---------------------//
    
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
    mySD.saveToSDBuffer(dte);
  }
  //---------------------//

  /*
  //Simule un envoi par GSM
  //---------------------//
  long timeSend = millis() - myGSM.getNbOfSendMeasure()*60000*SEND_PERIOD; //60000 = 1 min
  if ( timeSend >= 0 ){      
    long quetzal = millis()/60000;
    Serial.println("Temps écoulé depuis le debut en minutes : " + String(quetzal));
    myGSM.addOneSendMeasure();
    //Serial.println("Trying to read...");
    Queue<String> dataToSend = mySD.getDataToSend();
    if(dataToSend.isEmpty())
        Serial.println("There are no data to send");
    while(!dataToSend.isEmpty()){
      String tmp = dataToSend.dequeue();
      Serial.println(myDataMsg.haveNegativeClock(tmp,timerSystem.getNbReboot()));
    }
    //timerSystem.reboot_now();     //à commenter ou décommenter si on veut tester avec ou sans reboot
    mySD.resetFileDataToSend();
  }
  //---------------------//
  */

  //Set/Get Unix time
  //---------------------//
  long tmpTime = millis() - myGSM.getNbOfUnixTimeReceive()*60000*RECEIVE_UNIXTIME_PERIOD ; //60000 = 1 min
  if (tmpTime >= 0){
    myGSM.startupGSM();
    if(myGSM.setupGSM()){
      myGSM.addOneReceiveTime();
      myGSM.readTheClock();
      myGSM.stopGSM();
    }else {
      Serial.println("error : myGSM.setupGSM(), not any unix time receive");
      if (!myDataMsg.unixTimeIsSet())
        timerSystem.reboot_now();
      delay(6000);
    }
  }
  //---------------------//
  
  
  //Send Party with GSM
  //---------------------//
  tmpTime = millis() - myGSM.getNbOfSendMeasure()*60000*SEND_PERIOD; //60000 = 1 min
  if ( tmpTime >= 0 ){      //Si cela fait une heure, les données sont envoyées par le GSM
    myGSM.startupGSM();
    if(myGSM.setupGSM()){
      myGSM.addOneSendMeasure();
      //Serial.println("Trying to read...");
      Queue<String> dataToSend = mySD.getDataToSend();
      if(dataToSend.isEmpty())
        Serial.println("There are no data to send");
      while(!dataToSend.isEmpty()){
        String tmpString = dataToSend.dequeue();
        mySD.saveToSDLog(myDataMsg.haveUnixTime(tmpString,timerSystem.getNbReboot()));
        if (myGSM.sendData(myDataMsg.haveNegativeClock(tmpString,timerSystem.getNbReboot()))){
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
  
  
  // delay(16 * 60000);
  timerSystem.stopMyWatchdog();
  unsigned long to_wait = millis() - startAcquire;
  delay(MEASURE_PERIOD * 60000 - to_wait);
}
