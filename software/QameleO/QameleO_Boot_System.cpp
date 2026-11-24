#include "QameleO_Boot_System.h"


#include "QameleO_SD.h"
#include <Arduino.h>
#include <MsTimer2.h>
#include <avr/wdt.h>

QameleO_Boot_System syst;

void processRebootWatchdog()
{
    syst.addOne();
    Serial.println("watchdog courant got up\n");
    if ((syst.getTheCurrentWatchdog() ) >= syst.getWatchdogMax())
    {
      Serial.println("reboot via watchdog");
      syst.saveTimeSpent();
      MsTimer2::stop();
      wdt_enable(WDTO_500MS);
      delay(500);
   }
}

/**
 * Maker of the class
 */
QameleO_Boot_System::QameleO_Boot_System()
{
  sd = new QameleO_SD();
}

long QameleO_Boot_System::watchdog_courant = 0;
int QameleO_Boot_System::nbReboot = 0;

/**
 * adds plus one to watchdog_courant
 */
void QameleO_Boot_System::addOne()
{
  watchdog_courant++;
}

/**
 * Return the number of reboot. By default it's 0
 */
int QameleO_Boot_System::getNbReboot()
{
  return this->nbReboot;
}

/**
 * Call readNbReboot() to set nbReboot
 */
void QameleO_Boot_System::setNbReboot()
{
  this->nbReboot=sd->readNbReboot();
}

/**
 * Return the watchdog_courant
 */
long QameleO_Boot_System::getTheCurrentWatchdog()
{
  return watchdog_courant;
}

/**
 * Return watchdog_max
 */
long QameleO_Boot_System::getWatchdogMax()
{
  return this->watchdog_max;
}

/**
 * Reset watchdog courant
 */
void QameleO_Boot_System::razWatchdog()
{
  watchdog_courant=0;
}

/**
 * Reboot the timer
 */
void QameleO_Boot_System::reboot_now()
{
  MsTimer2::stop();
  watchdog_courant = watchdog_max;
  Serial.println("rebooting via reboot_now\n");
  MsTimer2::set(10, processRebootWatchdog);
  MsTimer2::start();
}

/**
 * Call saveOnBackup()
 */
void QameleO_Boot_System::saveTimeSpent()
{
  sd->saveOnBackup(this->nbReboot+1);
}

/**
 * Start the timer
 */
void QameleO_Boot_System::startMyWatchdog()
{
  this->razWatchdog();
  MsTimer2::set(watchdog_delay, processRebootWatchdog);
  MsTimer2::start();
  Serial.println("start watchdog");
}

/**
 * Stop the timer
 */
void QameleO_Boot_System::stopMyWatchdog()
{
  MsTimer2::stop();
}
