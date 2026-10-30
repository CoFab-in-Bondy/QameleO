#include "QameleO_NextPM_Dust.h"
#include "QameleO_print.h"

/**
 * 
 */
QameleO_NextPM_Dust::QameleO_NextPM_Dust():DustSensor(){
  this->pms = new NextPM(SerialPMS);
  this->humidity = 0;
  this->temperature = 0;
}

float QameleO_NextPM_Dust::getHumidity(){
  return this->humidity;
}

float QameleO_NextPM_Dust::getTemperature(){
  return this->temperature;
}

float QameleO_NextPM_Dust::getP1_ppm(){
  return this->p1_ppm;
}
    
float QameleO_NextPM_Dust::getP10_ppm(){
  return this->p10_ppm;
}
    
float QameleO_NextPM_Dust::getP25_ppm(){
  return this->p25_ppm;
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
void QameleO_NextPM_Dust::wakeUp(){
  this->pms->powerOn();
}

/**
 * 
 */
void QameleO_NextPM_Dust::sleep(){
  this->pms->shutdown();
}


/**
 * 
 */
void QameleO_NextPM_Dust::runDustSensorData(){
  //LOGLN("Next PM is mesuring --->  ");

  //LOGLN("(wake up and wait for stable readings) ");

  //LOGLN("(Send request read) ");

  if(this->pms->read_1min(this->data)){
    this->p1  = this->data.PM_UG_1_0;
    this->p10 = this->data.PM_UG_10_0;
    this->p25  = this->data.PM_UG_2_5;

    this->p1_ppm  = this->data.PM_PPM_1_0;
    this->p10_ppm = this->data.PM_PPM_10_0;
    this->p25_ppm  = this->data.PM_PPM_2_5;

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

    if (this->pms->readTempHumi( temperature, humidity)){

    this->temperature = temperature;
    this->humidity = humidity;
    LOG("Measure Temperature nextPM : ");
    LOG(this->temperature);
    LOG(" -- ");
    LOG("Measure Humidity nextPM : ");
    LOG(this->humidity);
    LOGLN(" -- OK!");

  } else {
    LOGLN("-- QameleO_NextPM:readTempHumi() Failed!");
  }
}

/**
 * 
 */
void QameleO_NextPM_Dust::runDustSensorData(QameleO_measure & measure){
  LOGLN("Next PM is mesuring --->  ");

  LOGLN("(wake up and wait for stable readings) ");

  LOGLN("(Send request read) ");

  if(this->pms->read_1min(this->data)){
    measure.pm1 = this->data.PM_UG_1_0;
    measure.pm10 = this->data.PM_UG_10_0;
    measure.pm25 = this->data.PM_UG_2_5;
  
    measure.pm1_ppm  = this->data.PM_PPM_1_0;
    measure.pm10_ppm = this->data.PM_PPM_10_0;
    measure.pm25_ppm  = this->data.PM_PPM_2_5;
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
}

/**
 * Setup the sensor
 */
void QameleO_NextPM_Dust::setupDustSensor(){
  LOGLN("setup dust sensor... ");
  pinMode(PMS_CONTROL_PIN, OUTPUT);
  digitalWrite(PMS_CONTROL_PIN, LOW);

  //Configure control 
  this->pms->configure();
  delay(10);
  LOGLN(" (active mode actived) --OK!");
}