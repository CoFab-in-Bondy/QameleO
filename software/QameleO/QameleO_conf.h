#ifndef QAMELEO_CONF_H
#define QAMELEO_CONF_H

// ============================================================================
// Choix des capteurs
// Active UN SEUL capteur particules et UN SEUL capteur temperature/humidite.
// ============================================================================
// Temperature / humidite
#define SHT_SENSOR
// #define DHT_SENSOR

// Particules
#define NextPM_SENSOR
// #define PMS7003_SENSOR

#if defined(SHT_SENSOR) && defined(DHT_SENSOR)
  #error "Choisis un seul capteur temperature/humidite : SHT_SENSOR OU DHT_SENSOR"
#endif

#if defined(NextPM_SENSOR) && defined(PMS7003_SENSOR)
  #error "Choisis un seul capteur particules : NextPM_SENSOR OU PMS7003_SENSOR"
#endif

#endif // QAMELEO_CONF_H
