#ifndef QAMELEO_HARD_CONF_H
#define QAMELEO_HARD_CONF_H

#include <Arduino.h>

// ============================================================================
// Configuration carte
// ============================================================================

#ifndef CONFIG_MEGA
  #define CONFIG_MEGA
#endif

// ============================================================================
// Ports serie
// Arduino Mega :
// Serial  = USB debug
// Serial1 = capteur particules
// Serial2 = modem TEL0177 / GSM
// ============================================================================

#ifndef SerialPMS
  #define SerialPMS Serial1
#endif

#ifndef SerialSensor
  #define SerialSensor Serial1
#endif

#ifndef SerialGSM
  #define SerialGSM Serial2
#endif

// Alias conserve pour compatibilite avec ancien code TinyGSM.
#ifndef SerialAT
  #define SerialAT SerialGSM
#endif

#ifndef TRANSMISSION_RATE
  #define TRANSMISSION_RATE 115200
#endif

#ifndef PMS_SERIAL_RATE
  #define PMS_SERIAL_RATE 115200
#endif

// ============================================================================
// Capteur humidite / temperature SHT35
// ============================================================================

#ifndef ADRESS_I2C_SHT35
  #define ADRESS_I2C_SHT35 0x45
#endif

#ifndef HUMIDITY_TEMP_SWITCH_PIN
  #define HUMIDITY_TEMP_SWITCH_PIN 7
#endif

// ============================================================================
// DHT22
// ============================================================================

#ifndef DHT11_PIN
  #define DHT11_PIN 4
#endif

#ifndef DHTTYPE
  #define DHTTYPE DHT22
#endif

// ============================================================================
// Capteur particules PMS / NextPM
// ============================================================================

// Attention : ne pas utiliser la meme broche que le ventilateur.
// Avant tu avais PMS_CONTROL_PIN = 8 et FAN_CONTROL_PIN = 8.
// Si tu utilises NextPM, PMS_CONTROL_PIN peut ne pas servir.
// Si tu utilises PMS7003, choisis une vraie broche libre.
#ifndef PMS_CONTROL_PIN
  #define PMS_CONTROL_PIN 8
#endif

#ifndef PMS_ACQUIRE_DELAY
  #define PMS_ACQUIRE_DELAY 60000UL
#endif

// ============================================================================
// RS485 NextPM
// ============================================================================

#ifndef SlaveId_RS485
  #define SlaveId_RS485 1
#endif

#ifndef reDePin_RS485
  #define reDePin_RS485 22
#endif

// ============================================================================
// Carte SD
// ============================================================================

#ifndef SD_CONTROL_PIN
  #define SD_CONTROL_PIN 53
#endif

// ============================================================================
// Ventilateur
// ============================================================================

#ifndef FAN_CONTROL_PIN
  #define FAN_CONTROL_PIN 6
#endif

#ifndef FAN_DELAY
  #define FAN_DELAY 120000UL
#endif

// ============================================================================
// Verification des conflits de broches
// ============================================================================

#if defined(PMS7003_SENSOR)
  #if PMS_CONTROL_PIN == FAN_CONTROL_PIN
    #error "Conflit broche : PMS_CONTROL_PIN et FAN_CONTROL_PIN utilisent la meme broche"
  #endif
#endif

#if HUMIDITY_TEMP_SWITCH_PIN == FAN_CONTROL_PIN
  #error "Conflit broche : HUMIDITY_TEMP_SWITCH_PIN et FAN_CONTROL_PIN utilisent la meme broche"
#endif

#if HUMIDITY_TEMP_SWITCH_PIN == PMS_CONTROL_PIN
  #if defined(PMS7003_SENSOR)
    #error "Conflit broche : HUMIDITY_TEMP_SWITCH_PIN et PMS_CONTROL_PIN utilisent la meme broche"
  #endif
#endif

#endif // QAMELEO_HARD_CONF_H