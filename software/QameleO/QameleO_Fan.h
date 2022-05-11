#ifndef QAMELEO_FAN_H
#define QAMELEO_FAN_H

class QameleO_Fan
{
  public :
    /**
     * Maker of the class
     */
    QameleO_Fan();

    /**
     * Start the fan
     */
    void runFanProcess();

    /**
     * Setup the pin of the fan
     */
    void setupFan();
};
#endif //QAMELEO_FAN_H
