#include "QameleO_GSM.h"

#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"

#include <arduino.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include "RTClib.h"

/**
 * Maker of the class
 */
QameleO_GSM::QameleO_GSM()
{
  this->modem = new TinyGsm(SerialAT);
  this->client = new TinyGsmClient(*this->modem);
  this->mqtt = new PubSubClient(*this->client);

  this->nbSendMeasure = 1;
}

void QameleO_GSM::addOneSendMeasure()
{
  this->nbSendMeasure++;
}

/**
 * Close the connection
 */
void QameleO_GSM::closeGSM()
{
  SerialAT.end();
}

unsigned long QameleO_GSM::getNbOfSendMeasure()
{
  return this->nbSendMeasure;
}

/**
 * Send the message create
 * 
 * @param msg - The message to send
 * @return true if it's a sucess, false if there are a problem
 */
bool QameleO_GSM::sendData(String msg)
{
  //setupGSM();
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.print("data is sending... " );
  int retry = 3;
  while (!this->modem->restart()) {
   if(retry < 0)
   {
    Serial.println(" bool QameleO_GSM::sendData(String msg), modem->restart() : fail");
    return false;
   }
    else
    {
          Serial.print(" retry ");
          delay(10000);
    }
    retry = retry - 1;
  }
  Serial.print("(modem initialized) " );
  // Unlock your SIM card with a PIN
  this->modem->simUnlock(GSM_PIN_CODE);

   retry = 3;
  while (!this->modem->waitForNetwork(NETWORK_REGISTER_TIMEOUT)) {
   if(retry < 0)
   {
    Serial.println(" bool QameleO_GSM::sendData(String msg), modem->waitForNetwork(NETWORK_REGISTER_TIMEOUT) : fail");
    return false;
   }
    else
    {
          Serial.print(" retry ");
    }
    retry = retry - 1;
  } 
  //String gsmTime = modem.getGSMDateTime(DATE_TIME);
  retry = 3;  
  while(!this->modem->gprsConnect(GSM_APN, "wap", "wapwap")) {
   if(retry < 0)
   {
    Serial.println(" bool QameleO_GSM::sendData(String msg), modem->gprsConnect(GSM_APN, \"wap\", \"wapwap\") : fail");
    return false;
   }
    else
    {
          Serial.print(" retry ");
    }
    retry = retry - 1;
  }
  Serial.print("(connected to network) " );
  this->mqtt->setServer(GSM_MQTT_BROKER, 1935);
  Serial.print("Connecting to ");
  Serial.print(GSM_MQTT_BROKER);
  retry = 3;
  while (0 == this->mqtt->connect(SENSOR_NAME, "gamasenseit", "gamasenseit")) {
    Serial.print(mqtt->state());
    if(retry < 0)
    {
      Serial.println(" bool QameleO_GSM::sendData(String msg), mqtt->connect(SENSOR_NAME, \"gamasenseit\", \"gamasenseit\") : fail");
      return false;
    }
    else
    {
      Serial.print(" retry ");
    }

    retry = retry - 1;
  }
  Serial.println(" OK");
  
  if(msg.length() <= 0)
  {
      Serial.println(" Le message est ne contient pas de caratère, il n'y a rien à envoyer");
      return false;
  }
  unsigned int bufSize = msg.length() + 1; 
  char cmessage[bufSize];
  msg.toCharArray(cmessage, bufSize);
  Serial.print("to send");
  Serial.println(cmessage);
  //char cmessage[50];
  //msg.toCharArray(cmessage,50);
  retry = 3;
  bool notstop = true;
  while (!this->mqtt->publish("gamasenseit", cmessage) && notstop)
  {
    if(retry < 0)
    {
      notstop = false;
      Serial.println(" fail to send");
    }
    else{
      Serial.print(" retry ");
      retry = retry - 1; 
    }
  }
  if(retry >= 0)
  {
    Serial.println("sent OK");
    this->mqtt->disconnect();
    return true;
  }
  this->mqtt->disconnect();
  return false;
  
}

/**
 * Try the connection and if is a success, start it
 */
bool QameleO_GSM::setupGSM()
{
  Serial.print("Setup GSM...");
  long rate = TRANSMISSION_RATE;
  //long rate = 115200;
  int retry = 3 ;
  bool notConnected = true;
 
  while(notConnected&&retry>0)
  {
    SerialAT.begin(rate);
    delay(3000);
    int nbloop = 20;
    while (notConnected && nbloop > 0) {
      SerialAT.print("AT\r\n");
      Serial.print("(wait) ");
      String input = SerialAT.readString();
      if (input.indexOf("OK") >= 0) {
        Serial.print(" (responded at rate ");
        Serial.print(rate);
        Serial.print(") ");
        notConnected = false;
      }
       delay(1000);

      nbloop = nbloop -1;
    }
    Serial.println("");
    Serial.println("retry");
    retry = retry -1;
  }
  delay(3000);
  if(!notConnected)
  {
    Serial.println(" -- OK!");
    return true;
  }
  else
  {
    Serial.println(" -- FAILED!");
    delay(500);
    return false;
    
  }
}

/**
 * Setup the pin of the GSM system
 */
void QameleO_GSM::setupGSM_start()
{
  pinMode(GSM_OFF, OUTPUT);
  pinMode(GSM_LOWP, OUTPUT);
  pinMode(GSM_ON, OUTPUT);
  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, LOW);
  digitalWrite(GSM_OFF, LOW);
  digitalWrite(GSM_LOWP, HIGH);   
  digitalWrite(GSM_ON, LOW);
}

/**
 * 
 */
void QameleO_GSM::startupGSM()
{
  digitalWrite(GSM_OFF, LOW);
  digitalWrite(GSM_LOWP, LOW);  
  delay(1000);
  digitalWrite(GSM_LOWP, HIGH);   
  digitalWrite(GSM_ON, HIGH);
  delay(3000);
  digitalWrite(GSM_ON, LOW);
  delay(2000);
}

/**
 * 
 */
void QameleO_GSM::stopGSM()
{
  SerialAT.end();
  digitalWrite(GSM_LOWP, HIGH);   
  digitalWrite(GSM_ON, LOW);
  digitalWrite(GSM_OFF, HIGH);
  digitalWrite(GSM_LOWP, LOW);   
  delay(1000);
  digitalWrite(GSM_LOWP, HIGH);   
 
  digitalWrite(GSM_OFF, LOW);
}
