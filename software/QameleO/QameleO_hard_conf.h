#ifndef QAMELEO_HARD_CONF_H
#define QAMELEO_HARD_CONF_H




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
// Capteur particules PMS / NextPM
// ============================================================================
#ifndef SerialPMS
  #define SerialPMS Serial1
#endif

#ifndef PMS_CONTROL_PIN
  #define PMS_CONTROL_PIN 6
#endif

#ifndef PMS_ACQUIRE_DELAY
  #define PMS_ACQUIRE_DELAY 60000
#endif

// ============================================================================
// Module GSM sur TEL0177
// ============================================================================
#ifndef SerialGSM
  #define SerialGSM Serial2
#endif

// ============================================================================
// Carte SD
// ============================================================================

#ifndef SD_CONTROL_PIN
  #define SD_CONTROL_PIN 53
#endif




#endif // QAMELEO_HARD_CONF_H