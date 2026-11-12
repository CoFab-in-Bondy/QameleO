#include "QameleO_NextPM_Dust.h"
#include "QameleO_hard_conf.h"

/**
 * 
 */
QameleO_NextPM_Dust::QameleO_NextPM_Dust():DustSensor(), humidity(0.0f), temperature(0.0f) {
  this->pms = new NextPM(SerialPMS);
}

/**
 * 
 */
void QameleO_NextPM_Dust::closeDustSensor(){
  SerialPMS.end();
}

/**
 * 
 */
void QameleO_NextPM_Dust::runDustSensorData(){
  //Serial.println(F("Next PM is mesuring --->  "));

  //Serial.println(F("(wake up and wait for stable readings) "));

  //Serial.println(F("(Send request read) "));

  if(this->pms->read_1min(this->data)){
    this->p1  = this->data.PM_UG_1_0;
    this->p10 = this->data.PM_UG_10_0;
    this->p25  = this->data.PM_UG_2_5;

    Serial.println(F("   -- OK!"));
    Serial.print(F("Data:"));

    Serial.println(F("PM 1.0 (ug/m3): "));
    Serial.print(this->p1);
    Serial.println(F(" -- PM 2.5 (ug/m3): "));
    Serial.print(this->p25);
    Serial.println(F(" -- PM 10.0 (ug/m3): "));
    Serial.print(this->p10);
    Serial.println(F(" -- OK!"));
  }else{
    Serial.println(F("  -- Failed!"));
  }
}

/**
 * 
 */
void QameleO_NextPM_Dust::runDustSensorData(QameleO_measure & measure){
  Serial.println(F("Next PM is mesuring --->  "));

  //Serial.println(F("(wake up and wait for stable readings) "));

  //Serial.println(F("(Send request read) "));

  if(this->pms->read_1min(this->data)){
    this->p1  = measure.pm1  = this->data.PM_UG_1_0;
    this->p10 = measure.pm10 = this->data.PM_UG_10_0;
    this->p25 = measure.pm25 = this->data.PM_UG_2_5;
  
    //Serial.println(F("   -- OK!"));
    //Serial.print(F("Data:"));

    Serial.print(F(" -- PM 1.0 (ug/m3): "));
    Serial.println(this->p1);
    Serial.print(F(" -- PM 2.5 (ug/m3): "));
    Serial.println(this->p25);
    Serial.print(F(" -- PM 10.0 (ug/m3): "));
    Serial.println(this->p10);
    Serial.println(F(" -- OK!"));
  } else {
    Serial.println(F("  -- Failed!"));
  }
    if (this->pms->readTempHumi( temperature, humidity)){

    measure.temperatureNextPM = temperature;
    measure.humidityNextPM = humidity;
    Serial.print(F("Measure Temperature nextPM : "));
    Serial.print(measure.temperatureNextPM);
    Serial.print(F(" -- "));
    Serial.print(F("Measure Humidity nextPM : "));
    Serial.print(measure.humidityNextPM);
    Serial.println(F(" -- OK!"));

  } else {
    Serial.println(F("-- QameleO_NextPM:readTempHumi() Failed!"));
  }
}

/**
 * Setup the sensor
 */
void QameleO_NextPM_Dust::setupDustSensor(){
  Serial.println(F("setup dust sensor... "));
  pinMode(PMS_CONTROL_PIN, OUTPUT);
  digitalWrite(PMS_CONTROL_PIN, LOW);

  //Configure control 
  this->pms->configure();
  delay(10);
  Serial.println(F(" (active mode actived) --OK!"));
}