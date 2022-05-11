#include "DataMessage.h"

#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"
#include "QameleO_SD.h"

#include <arduino.h>
#include "RTClib.h"

/**
 * Maker of the class
 */
DataMessage::DataMessage()
{
  this->message="";
  this->messageID=0;
  #if defined(NextPM_SENSOR) || defined (PMS7003_SENSOR)
    this->dustSensor = true;
  #else 
    this->dustSensor = false;
  #endif 
  #if defined(DHT_SENSOR) || defined(SHT_SENSOR) 
    this->humTempSensor = true;
  #else 
    this->humTempSensor = false;
  #endif
  startTime = 0; // date de départ en seconde
  initOffset = 1000000000; // date de départ en seconde
  lastUpdate = 1000000000;
}

/**
 * Add a time on the string
 * 
 * @param data  - The string with the sensor data
 * @param nbReboot - The number of reboot
 */
String DataMessage::buildCaptureMessage(String data, int nbReboot)
{
  int retry = 10;
  while (lastUpdate  > millis()|| (lastUpdate + 7200000)  < millis() || startTime < 1561757324) {   // 7200000= 2h
    this->updateClock();
    retry = retry - 1;
    if(retry < 0)
      return "";
  }
  
  this->messageID = this->messageID + 1;
  String sid = String(this->messageID);
  //uint32_t cdate = (millis() - lastUpdate ) / 1000 + startTime;
  //String sdate = String(cdate);
  String sdate = String(millis());



  String msg = sdate + ";";
  msg += String(nbReboot) + ";"; 
  msg += SENSOR_NAME;
  msg += ";" + sid + ";";
  msg += data;
  return msg;
}

/**
 * Create a string with the data collected
 * 
 * @param data - The struct QameleO_measure, use to get the data
 * @param nbReboot - The number of reboot
 */
String DataMessage::createMessage(QameleO_measure data, int nbReboot)
{
  String m;
  if (this->dustSensor  && this->humTempSensor){
    m = String(data.pm1) + ":" + String(data.pm25) + ":" + String(data.pm10) + ":" + data.humidity + ":" + data.temperature;
  }else if (this->dustSensor){
    m = String(data.pm1) + ":" + String(data.pm25) + ":" + String(data.pm10);
  }else if (this->humTempSensor){
    m = String(data.humidity) + ":" + data.temperature;
  }else{
    m="You have define no any sensor. There are no data to send";
  }
  #ifdef NextPM_SENSOR
    m = m + ":" + String(data.humidityNextPM) + ":" + String(data.temperatureNextPM);
  #endif
  
  this->message =  buildCaptureMessage(m, nbReboot);
  return this->message;
}

/**
 * Replace the message clock with a negative clock
 * 
 * @param msg - The String where to put the negative clock
 * @param nbRebootMax - the number of program restarts
 * @return the changed message
 */
String DataMessage::haveNegativeClock(String msg, int nbRebootMax)
{
  String clockString="";
  String idRebootString="";
  byte check=0;
  long clockLong, idReboot;
  char buf[msg.length()+1];
  msg.toCharArray(buf,msg.length()+1);
  
  for(int i=0;i<sizeof(buf) && check!=2 ;i++){
    //Serial.print(buf[i]);
    if (buf[i]==';' || buf[i]=='$'){
      check++;
    }else {
      if (check==0)
        clockString += buf[i];
      if (check==1)
        idRebootString += buf[i];
    }
  }
  /*//Serial.println("L'horloge enlevée : " + clockString);
  int basilic = clockString.toInt();
  long canard = clockString.toInt();
  Serial.println("toInt() w/ int : " + String(basilic));
  Serial.println("toInt() w/ long : " + String(canard)); */
  QameleO_SD sd;
  clockLong = clockString.toInt() - sd.readBackup(idRebootString.toInt(), nbRebootMax); //mettre à la place de millis un appel qui renvois le tps à soustraire du msg
  /*Serial.println("Quasiment le tps utilisé " + String(millis()));
  Serial.println("Horloge négative : " + String(clockLong));*/
    
  /*//Serial.println("");
  //Serial.println(msg);
  Serial.println("Ancien message : " + msg);
  String msgSaved = msg.substring(msg.indexOf(";"));
  Serial.println("Nouveau message, prêt pour qu'on ajoute l'horloge négative : " + msgSaved);*/
  
  return String(clockLong)+msg.substring(msg.indexOf(";",msg.indexOf(";")+1));
}

/**
 * Update the clock
 */
void DataMessage::updateClock()
{
  SerialAT.print("AT+CCLK?\r\n");
  String input = SerialAT.readString();
  String year = input.substring(10,12);
  String month = input.substring(13,15);
  String day = input.substring(16,18);
  String hour = input.substring(19,21);
  String minute = input.substring(22,24);
  String second = input.substring(25,27);
  DateTime dt (year.toInt(),month.toInt(),day.toInt(),hour.toInt(),minute.toInt(),second.toInt());
  uint32_t mt = dt.unixtime();
  Serial.print("unix time (s)");
  Serial.println(mt);
  lastUpdate = millis();
  startTime = mt; // date de départ en seconde
}
