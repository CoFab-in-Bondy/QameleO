#include <TEL0177.h>

// ==========================================================================
// Configuration
// ==========================================================================
#define SIM_PIN         "0000"
#define GSM_APN         "mmsbouygtel.com"

#define MQTT_HOST       "mqtt.qameleo.fr"
#define MQTT_PORT       1935
#define MQTT_CLIENT_ID  "Emmanuel"
#define MQTT_USER       "gamasenseit"
#define MQTT_PASS       "gamasenseit"
#define MQTT_TOPIC      "gamasenseit"

TEL0177 modem;

unsigned long lastPublish = 0;

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(2000);

  Serial.println("=== TEL0177 - MQTT Publish ===\n");

  modem.begin(Serial2);
  modem.simUnlock(SIM_PIN);
  modem.networkConnect(GSM_APN);
  modem.mqttsetClient(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);
  modem.mqttconnect(MQTT_HOST, MQTT_PORT);

  Serial.print("IP     = "); Serial.println(modem.ipAddress());
  Serial.print("MQTT   = "); Serial.println(modem.mqttisConnected() ? "connecte" : "KO");
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();

  if (millis() - lastPublish >= 10000) {
    lastPublish = millis();

    if (!modem.mqttisConnected()) {
      Serial.println("Reconnexion MQTT...");
      modem.mqttreconnect();
    }

    if (modem.mqttisConnected()) {
      Serial.println("Publish : hello TEL0177");
      modem.mqttpublish(MQTT_TOPIC, "hello TEL0177");
    }
  }
}
