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
#define MQTT_TOPIC_TIMER      "myTimer"

TEL0177 modem;

// ==========================================================================
// Callback MQTT
// ==========================================================================
void onMqttMessage(const char* topic, const char* payload, uint16_t length) {
  Serial.println("=============================================================");
  Serial.print("Topic   = "); Serial.println(topic);
  Serial.print("Contenu = "); Serial.println(payload);
  Serial.print("Taille  = "); Serial.println(length);
  Serial.println("=============================================================");
}

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(2000);

  Serial.println("=== TEL0177 - MQTT Subscribe ===\n");

  modem.begin(Serial2);
  modem.simUnlock(SIM_PIN);
  modem.networkConnect(GSM_APN);
  modem.mqttsetCallback(onMqttMessage);
  modem.mqttsetClient(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);
  modem.mqttconnect(MQTT_HOST, MQTT_PORT);
  modem.mqttsubscribe(MQTT_TOPIC);

  Serial.print("IP   = "); Serial.println(modem.ipAddress());
  Serial.print("MQTT = "); Serial.println(modem.mqttisConnected() ? "connecte" : "KO");
  Serial.println("\nEcoute en cours...");
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();
}
