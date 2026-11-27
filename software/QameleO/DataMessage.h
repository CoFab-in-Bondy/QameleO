#ifndef DATAMESSAGE_H
#define DATAMESSAGE_H

#include "QameleO_struct.h"

#include <Arduino.h>

class DataMessage 
{
  private :
    bool _dustSensor;
    bool _humTempSensor;

    char _message [150];
    int _messageID;

    static unsigned long unixTime;
    static uint32_t lastUpdateTimeClock;


    const char * _buildCapteurMessage(const char * data, int nbReboot);

  public :

    DataMessage();

    void buildCapteurMessage(const char * data, int nbReboot);

    const char * createMessage(QameleO_measure data, int nbReboot);

    const char * haveNegativeClock(char * msg, int nbRebootMax);

    const char * haveUnixTime(char * msg, size_t capacity);

    static void setUnixTime(unsigned long n);

    bool unixTimeIsSet();

    static unsigned long getUnixTime();
};

#endif //DATAMESSAGE_H