#ifndef QAMELEO_CONF_H
#define QAMELEO_CONF_H

// ============================================================================
// Identification du capteur
// ============================================================================

#ifndef SENSOR_NAME
  #define SENSOR_NAME "QameleO_2026_"
#endif

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

// ============================================================================
// Fichiers SD
// ============================================================================

#ifndef FILE_NAME_BUFFER
  #define FILE_NAME_BUFFER "notsend.csv"
#endif

#ifndef FILE_NAME_SAVE
  #define FILE_NAME_SAVE "timesave.csv"
#endif

#ifndef FILE_NAME_LOG
  #define FILE_NAME_LOG "datalog.csv"
#endif

// ============================================================================
// Plateforme
// ============================================================================

#ifndef CONFIG_MEGA
  #define CONFIG_MEGA
#endif

// ============================================================================
// Timers en minutes
// ============================================================================

#ifndef MEASURE_PERIOD
  #define MEASURE_PERIOD 1UL  //15UL
#endif

#ifndef SEND_PERIOD
  #define SEND_PERIOD 5UL //60UL
#endif

#ifndef RECEIVE_UNIXTIME_PERIOD
  #define RECEIVE_UNIXTIME_PERIOD 240 //240UL
#endif

// ============================================================================
// Watchdog / reboot
// ============================================================================

#ifndef REBOOT_DELAY
  #define REBOOT_DELAY 5UL
#endif

// ============================================================================
// GSM / TEL0177
// ============================================================================

#ifndef GSM_PIN_CODE
  #define GSM_PIN_CODE "0000"
#endif

#ifndef GSM_APN
  #define GSM_APN "mmsbouygtel.com"
#endif

#ifndef GSM_USER
  #define GSM_USER ""
#endif

#ifndef GSM_PASS
  #define GSM_PASS ""
#endif

#ifndef GSM_PDP_TYPE
  #define GSM_PDP_TYPE "IP"
#endif

// Ancien nom conserve si du vieux code l'utilise encore.
// Dans ton nouveau code, utilise plutot MQTT_HOST.
#ifndef GSM_MQTT_BROKER
  #define GSM_MQTT_BROKER MQTT_HOST
#endif

// ============================================================================
// MQTT
// ============================================================================

#ifndef MQTT_HOST
  #define MQTT_HOST "XXXXXX"
#endif

#ifndef MQTT_PORT
  #define MQTT_PORT 1935
#endif

#ifndef MQTT_CLIENT_ID
  #define MQTT_CLIENT_ID SENSOR_NAME
#endif

#ifndef MQTT_USER
  #define MQTT_USER "XXXXXXX"
#endif

#ifndef MQTT_PASS
  #define MQTT_PASS "XXXXXXX"
#endif

#ifndef MQTT_TOPIC
  #define MQTT_TOPIC "gamasenseit"
#endif

#ifndef MQTT_TOPIC_TIME
  #define MQTT_TOPIC_TIME "myTimer"
#endif

#ifndef MQTT_QOS
  #define MQTT_QOS 0
#endif

#ifndef MQTT_RETAIN
  #define MQTT_RETAIN false
#endif

#endif // QAMELEO_CONF_H