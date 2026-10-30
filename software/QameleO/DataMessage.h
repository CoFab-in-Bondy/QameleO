#ifndef DATAMESSAGE_H
#define DATAMESSAGE_H

#include "QameleO_struct.h"

#include <Arduino.h>

class DataMessage
{
  private :

    /**
     * Check if a dust sensor is use
     */
    bool dustSensor;

    /**
     * Check if a humidity temperature sensor is use
     */
    bool humTempSensor;

    /**
     * Keep the last message create
     */
    String message;

    /**
     * Keep the id of the last message create
     */
    int messageID;

    /**
     * Keep the unix time send by the server or by default 0
     */
    static unsigned long unixTime;

    /*uint32_t startTime;
    unsigned long initOffset;
    unsigned long lastUpdate;*/

    /**
     * Add a time on the string
     *
     * @param data  - The string with the sensor data
     */
    String buildCaptureMessage(String data, int nbReboot);

    /**
     * Update the clock
     */
    //void updateClock();

  public :

    /**
     * Maker of the class
     */
    DataMessage();

    /**
     * Create a string with the data collected
     *
     * @param data - The struct QameleO_measure, use to get the data
     */
    String createMessage(QameleO_measure data, int nbReboot);

    /**
     * Replace the message clock with a negative clock
     *
     * @param msg - the String where to put the negative clock
     * @param nbRebootMax - the number of program restarts
     * @return the changed message
     */
     String haveNegativeClock(String msg, int nbRebootMax);

    /**
     * Replace the message clock with the unix time
     *
     * @param msg - the String where to put the unix time
     * @param nbRebootMax - the number of program restarts
     * @return the changed message
     */
     String haveUnixTime(String msg, int nbRebootMax);

     /**
      * Set the unix time to use
      *
      * @param n - the unix time to use
      */
     static void setUnixTime(unsigned long n);

     /**
      * Use to check if the unix time is set
      *
      * @return true - if the unix time has been set
      * @return false - if the unix time hasn't been set
      */
     bool unixTimeIsSet();


};

#endif //DATAMESSAGE_H
