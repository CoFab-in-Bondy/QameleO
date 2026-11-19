#ifndef QAMELEO_GSM_TEL_H
#define QAMELEO_GSM_TEL_H

#include <TEL0177.h>

class QameleO_GSM_Tel 
{
  private:
    TEL0177 _modem;

    static char _unix_time [11];
    bool _unix_time_received = false;
    static unsigned long _nbUnixTimeReceive;

    unsigned long _nbSendMeasure = 0;



    /**
     * 
     */
    bool _ensureData();

    /**
     * 
     */
    bool _ensureMqtt();

    /**
     * 
     */
    bool _subscribeTimer();

    /**
     * 
     */
    static void _mqttCallbackRouter(const char * topic, const char * payload, uint16_t length);

    static bool _saveUnixTime(const char * payload, uint16_t length);


  public:
    /**
     * 
     */
    QameleO_GSM_Tel();

    /**
     * 
     */
    bool begin();

    /**
     * 
     */
    bool sendData(const char * msg);

    void addOneSendMeasure();
    static void addOneReceiveTime();


    const char * getUnixTime() const;

    unsigned long getNbSendMeasure() const;

    unsigned long getNbUnixTimeReceive() const;

    bool requestUnixTime();


    void loop();

    void loopFor(int timer);


};

#endif // QAMELEO_GSM_TEL_H