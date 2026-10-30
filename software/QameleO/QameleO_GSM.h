#ifndef QAMELEO_GSM_H
#define QAMELEO_GSM_H

#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"

#include <PubSubClient.h>
#include <TinyGsmClient.h>

/**
 *  
 */
void callback(char* topic, byte *payload, unsigned int length);

class QameleO_GSM
{
  private:
    
    TinyGsmClient *client;
    TinyGsm *modem;
    PubSubClient *mqtt;

    /**
     * Contians the number of times that data has been sent by the GSM system
     */
    unsigned long nbSendMeasure;

    /**
     * Contians the number of times that unix time has been receive by the GSM system
     */
    unsigned long nbUnixTimeReceive;
    
  public:
    /**
     * Maker of the class
     */
    QameleO_GSM();

    /**
     * Adds plus one to nbSendMeasure
     */
    void addOneSendMeasure();

    /**
     * Adds plus one to nbUnixTimeReceive
     */
    void addOneReceiveTime();
    
    /**
     *  Close the connection
     */
    void closeGSM();

    /**
     * Return the number of SendMeasure
     */
    unsigned long getNbOfSendMeasure();

    /**
     * Return the number of unix time have been receive
     */
    unsigned long getNbOfUnixTimeReceive();

    /**
     * Connect to the server on topic myTimer, and read the clock to know what time is it
     * 
     * @return true - if the program has been subscribe to the server
     * @return false - if the program meet a problem to subscribe to the server
     */
    bool readTheClock();
    
    /**
     * Send the message create
     * 
     * @param msg - The message to send
     * @return true if it's a sucess, false if there are a problem
     */
    bool sendData(String msg);

    /**
     * Try the connection and if is a success, start it
     */
    bool setupGSM();

    /**
     * Setup the pin of the GSM system
     */
    void setupGSM_start();

    /**
     * 
     */
    void startupGSM();

    /**
     * 
     */
    void stopGSM();
    
};

#endif //QAMELEO_GSM_H
