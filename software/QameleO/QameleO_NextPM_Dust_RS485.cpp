#include "QameleO_NextPM_Dust_RS485.h"


QameleO_NextPM_Dust_RS485::QameleO_NextPM_Dust_RS485():DustSensor()
{
  this->pms = new NextPM_RS485(SerialPMS, SlaveId_RS485, reDePin_RS485);   // Serial1, slaveId=1, RE/DE sur D8
  this->temperature = 0;
  this->humidity    = 0;
}



void QameleO_NextPM_Dust_RS485::closeDustSensor(){
  SerialPMS.end();
}

void QameleO_NextPM_Dust_RS485::runDustSensorData()
{
  LOGLN("Next PM is mesuring --->  ");

  LOGLN("(wake up and wait for stable readings) ");

  LOGLN("(Send request read) ");

  if(this->pms->read_1min_rs485(this->data)){
    this->p1  = this->data.PM_UG_1_0;
    this->p10 = this->data.PM_UG_10_0;
    this->p25  = this->data.PM_UG_2_5;

    LOGLN("   -- OK!");
    LOG("Data:");

    LOGLN("PM 1.0 (ug/m3): ");
    LOGLN(this->p1);
    LOGLN(" -- PM 2.5 (ug/m3): ");
    LOGLN(this->p25);
    LOGLN(" -- PM 10.0 (ug/m3): ");
    LOGLN(this->p10);
    LOGLN(" -- OK!");
  }else{
    LOGLN("  -- Failed!");
  }
}

void QameleO_NextPM_Dust_RS485::runDustSensorData(QameleO_measure &measure){
  LOGLN("Next PM is mesuring --->  ");

  LOGLN("(wake up and wait for stable readings) ");

  LOGLN("(Send request read) ");

  if(this->pms->read_1min_rs485(this->data)){
    measure.pm1 = this->data.PM_UG_1_0;
    measure.pm10 = this->data.PM_UG_10_0;
    measure.pm25 = this->data.PM_UG_2_5;
  
    LOGLN("   -- OK!");
    LOG("Data:");

    LOG("PM 1.0 (ug/m3): ");
    LOGLN(this->p1);
    LOG(" -- PM 2.5 (ug/m3): ");
    LOGLN(this->p25);
    LOG(" -- PM 10.0 (ug/m3): ");
    LOGLN(this->p10);
    LOGLN(" -- OK!");
  } else {
    LOGLN("  -- Failed!");
  }
  /*
    if (this->pms->readTempHumi( temperature, humidity)){

    measure.temperatureNextPM = temperature;
    measure.humidityNextPM = humidity;
    LOG("Measure Temperature nextPM : ");
    LOG(measure.temperatureNextPM);
    LOG(" -- ");
    LOG("Measure Humidity nextPM : ");
    LOG(measure.humidityNextPM);
    LOGLN(" -- OK!");

  } else {
    LOGLN("-- QameleO_NextPM:readTempHumi() Failed!");
  }
  */
}

void QameleO_NextPM_Dust_RS485::setupDustSensor(){
  LOGLN("setup dust sensor... ");
  pinMode(reDePin_RS485, OUTPUT);
  digitalWrite(reDePin_RS485, LOW);

  //Configure control 
  this->pms->configure();
  delay(10);
  LOGLN(" (active mode actived) --OK!");
}