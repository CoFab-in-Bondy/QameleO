#ifndef QAMELEO_STRUCT_H
#define QAMELEO_STRUCT_H

struct QameleO_measure
  {
    //unsigned long startAcquire = millis();
    
    float humidity;
    float temperature;
    float pm1;
    float pm10;
    float pm25;
    float pm1_ppm;
    float pm10_ppm;
    float pm25_ppm;
    float humidityNextPM;
    float temperatureNextPM;
  };

#endif //QAMELEO_STRUCT_H