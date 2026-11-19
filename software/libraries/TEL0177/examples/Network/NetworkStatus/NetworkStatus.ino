#include <TEL0177.h>

// ==========================================================================
// Configuration
// ==========================================================================
#define SIM_PIN     "0000"
#define GSM_APN     "mmsbouygtel.com"

TEL0177 modem;

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(2000);

  Serial.println("=== TEL0177 - Network Status ===\n");

  // Modem
  modem.begin(Serial2);
  Serial.print("Alive  = "); Serial.println(modem.isAlive() ? "oui" : "non");

  // SIM
  if (!modem.isSIMReady()) modem.simUnlock(SIM_PIN);
  Serial.print("SIM    = "); Serial.println(modem.isSIMReady() ? "prete" : "KO");

  // Réseau
  modem.waitForNetwork(30000);

  int rssi = 0, ber = 0;
  modem.getSignalQuality(rssi, ber);
  Serial.print("RSSI   = "); Serial.println(rssi);

  char oper[64] = {0};
  modem.getOperator(oper, sizeof(oper));
  Serial.print("Oper.  = "); Serial.println(oper);

  // Data
  bool netOk = modem.networkConnect(GSM_APN);
  Serial.print("Net OK = ");
  Serial.println(netOk ? "oui" : "non");

  char ip[32] = {0};
  bool ipOk = modem.getIPAddress(ip, sizeof(ip));
  Serial.print("getIPAddress OK = ");
  Serial.println(ipOk ? "oui" : "non");
  Serial.print("IP buffer       = ");
  Serial.println(ip);
  Serial.print("IP cache        = ");
  Serial.println(modem.ipAddress());

  Serial.print("IP     = "); Serial.println(modem.ipAddress());
  Serial.print("Data   = "); Serial.println(modem.isDataConnected() ? "connecte" : "KO");

  // Infos module
  char imei[32] = {0};
  modem.getIMEI(imei, sizeof(imei));
  Serial.print("IMEI   = "); Serial.println(imei);

  char model[32] = {0};
  modem.getModel(model, sizeof(model));
  Serial.print("Model  = "); Serial.println(model);

  char fw[200] = {0};
  modem.getFirmware(fw, sizeof(fw));
  Serial.print("FW     = "); Serial.println(fw);

  Serial.println("\n=== Fin ===");
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();
}
