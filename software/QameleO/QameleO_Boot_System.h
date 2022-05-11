#ifndef QAMELEO_BOOT_SYSTEM_H
#define QAMELEO_BOOT_SYSTEM_H

#include "QameleO_conf.h"
#include "QameleO_SD.h"

void processRebootWatchdog();

class QameleO_Boot_System
{
  private :
    static long watchdog_courant;
    
    const long watchdog_max = REBOOT_DELAY;
    const long watchdog_delay = 60000;

    QameleO_SD *sd;

    /**
     * Contain the number of reboot
     */
    static int nbReboot;
    
    /**
     * Reset watchdog courant
     */
    void razWatchdog();

  public :
  
    /**
     * Maker of the class
     */
    QameleO_Boot_System();

    /**
     * adds plus one to watchdog_courant
     */
    void addOne();
        
    /**
     * Return the number of reboot. By default it's 0
     */
    int getNbReboot();
    
    /**
     * Return the watchdog_courant
     */
    long getTheCurrentWatchdog();

    /**
     * Return watchdog_max
     */
    long getWatchdogMax();
     
    /**
     * Reboot the timer
     */
    void reboot_now();

    /**
     * Call saveOnBackup() 
     */
    void saveTimeSpent();

    /**
     * Call readNbReboot() to set nbReboot
     */
    void setNbReboot();
    
    /**
     * Start the timer
     */
    void startMyWatchdog();

    /**
     * Stop the timer
     */
    void stopMyWatchdog();

};

#endif //QAMELEO_BOOT_SYSTEM_H
