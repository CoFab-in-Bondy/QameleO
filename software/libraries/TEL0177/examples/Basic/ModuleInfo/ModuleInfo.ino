#include <TEL0177.h>

// ==========================================================================
// Configuration
// ==========================================================================
#define SERIAL_MONITOR_BAUD 115200
#define MODEM_BAUD          115200

// Si ta SIM a un code PIN, garde-le ici.
// Si elle n'a pas de PIN, tu peux laisser nullptr.
#define SIM_PIN "0000"

TEL0177 modem;

// ==========================================================================
// Utilitaires
// ==========================================================================
void printResult(const char* label, bool ok) {
  Serial.print(label);
  Serial.print("... ");
  Serial.print(ok ? "OK" : "KO");

  if (!ok) {
    Serial.print(" | lastError = ");
    Serial.print((int)modem.lastError());

    Serial.print(" | lastResponse = ");
    Serial.print(modem.lastResponse());
  }

  Serial.println();
}

void printInfo(const char* label, bool ok, const char* value) {
  Serial.print(label);
  Serial.print(" : ");

  if (ok) {
    Serial.println(value);
  } else {
    Serial.print("KO");
    Serial.print(" | lastError = ");
    Serial.print((int)modem.lastError());
    Serial.print(" | lastResponse = ");
    Serial.println(modem.lastResponse());
  }
}

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(SERIAL_MONITOR_BAUD);
  Serial2.begin(MODEM_BAUD);

  delay(2000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("      TEL0177 - ModuleInfo Test");
  Serial.println("========================================");
  Serial.println();

  // --------------------------------------------------------------------------
  // 1. Initialisation modem
  // --------------------------------------------------------------------------
  printResult("Init modem", modem.begin(Serial2));

  if (!modem.isAlive()) {
    Serial.println();
    Serial.println("Modem non detecte. Fin du test.");
    return;
  }

  printResult("Modem alive", true);

  // --------------------------------------------------------------------------
  // 2. Informations module
  // --------------------------------------------------------------------------
  char buffer[128] = {0};

  memset(buffer, 0, sizeof(buffer));
  printInfo("Fabricant", modem.getManufacturer(buffer, sizeof(buffer)), buffer);

  memset(buffer, 0, sizeof(buffer));
  printInfo("Modele", modem.getModel(buffer, sizeof(buffer)), buffer);

  memset(buffer, 0, sizeof(buffer));
  printInfo("Firmware", modem.getFirmware(buffer, sizeof(buffer)), buffer);

  memset(buffer, 0, sizeof(buffer));
  printInfo("IMEI", modem.getIMEI(buffer, sizeof(buffer)), buffer);

  // --------------------------------------------------------------------------
  // 3. Informations SIM
  // --------------------------------------------------------------------------
  Serial.println();
  Serial.println("---------- SIM ----------");

  bool simReady = modem.isSIMReady();
  printResult("SIM prete", simReady);

  if (!simReady) {
    printResult("Deverrouillage SIM", modem.simUnlock(SIM_PIN));
  }

  memset(buffer, 0, sizeof(buffer));
  printInfo("ICCID", modem.getICCID(buffer, sizeof(buffer)), buffer);

  memset(buffer, 0, sizeof(buffer));
  printInfo("IMSI", modem.getIMSI(buffer, sizeof(buffer)), buffer);

  // --------------------------------------------------------------------------
  // 4. Informations reseau
  // --------------------------------------------------------------------------
  Serial.println();
  Serial.println("---------- RESEAU ----------");

  printResult("Enregistrement reseau", modem.isNetworkRegistered());
  printResult("Enregistrement data", modem.isDataNetworkRegistered());
  printResult("Enregistrement LTE", modem.isLTERegistered());

  int rssi = 0;
  int ber = 0;

  if (modem.getSignalQuality(rssi, ber)) {
    Serial.print("Signal RSSI : ");
    Serial.println(rssi);

    Serial.print("Signal BER  : ");
    Serial.println(ber);
  } else {
    Serial.print("Signal : KO | lastError = ");
    Serial.print((int)modem.lastError());
    Serial.print(" | lastResponse = ");
    Serial.println(modem.lastResponse());
  }

  memset(buffer, 0, sizeof(buffer));
  printInfo("Operateur", modem.getOperator(buffer, sizeof(buffer)), buffer);

  Serial.println();
  Serial.println("========================================");
  Serial.println("           Test termine");
  Serial.println("========================================");
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();
}