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
    float humidityNextPM;
    float temperatureNextPM;
  };

#endif //QAMELEO_STRUCT_H
