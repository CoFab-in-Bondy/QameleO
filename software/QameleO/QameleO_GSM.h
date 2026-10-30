#ifndef QAMELEO_GSM_H
#define QAMELEO_GSM_H

#include <Arduino.h>
#include <TEL0177.h>

class QameleO_GSM {
  private:
    TEL0177 _modem;
    static QameleO_GSM* _instance;

    bool _has_new_unix_time = false;
    unsigned long _last_unix_time = 0;

    unsigned long _nbSendMeasure = 0;
    unsigned long _nbUnixTimeReceive = 0;

    static void _mqttCallbackRouter(const char* topic, const char* payload, uint16_t length);
    static void _mqttRawCallbackRouter(const char* urc);

    void _onMqttMessage(const char* topic, const char* payload, uint16_t length);
    void _onMqttRaw(const char* urc);
    bool _extractUnixTime(const char* payload);

    bool _setupNetwork();
    bool _setupMQTT();
    bool _subscribeClockTopic();

  public:
    QameleO_GSM();

    bool begin();

    bool setupGSM_start();
    bool setupMQTTAndSubscribe();
    bool stopGSM();
    bool reset();
    void closeGSM();
    void loop();
    void loopFor(uint32_t durationMs);

    bool sendData(const String& msg, bool is_last = false);
    bool sendData(const char* msg, bool is_last = false);

    bool readTheClock(uint32_t listenMs = 5000);
    bool reconnectMQTT();
    bool resubscribeTopics();

    void addOneSendMeasure();
    void addOneReceiveTime();

    unsigned long getNbOfSendMeasure() const;
    unsigned long getNbOfUnixTimeReceive() const;
    unsigned long getLastUnixTime() const;

    bool hasNewUnixTime() const;
    void clearNewUnixTimeFlag();

    bool connectGSMAndMQTT();

    bool stopMQTTOnly();

    //TEL_Status lastError() const;
    //const char* lastResponse() const;
};

#endif // QAMELEO_GSM_H