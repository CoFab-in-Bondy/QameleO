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

#endif // QAMELEO_HARD_CONF_H