#ifndef QAMELEO_GSM_H
#define QAMELEO_GSM_H

#include "QameleO_hard_conf.h"
#include "QameleO_struct.h"

#include <PubSubClient.h>
#include <TinyGsmClient.h>

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
    
  public:
    /**
     * Maker of the class
     */
    QameleO_GSM();

    /**
     * Adds plus one to cycle
     */
    void addOneSendMeasure();
    
    /**
     *  Close the connection
     */
    void closeGSM();

    /**
     * Return the number of cycle
     */
    unsigned long getNbOfSendMeasure();
    
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
