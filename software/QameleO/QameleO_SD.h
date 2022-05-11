#ifndef QAMELEO_SD_H
#define QAMELEO_SD_H

#include <SD.h>
#include "MyPile.h"

class QameleO_SD
{
  private :
    /**
     * Use to open, write and read on the SD card
     */
    File myFile;

    /**
     * Use to open, write and read on the SD card
     */
    File myFile2;

    /**
     * Write in the SD card
     * 
     * @param msg - The message to write
     */
    void saveDataIn(String msg);

    /**
     * That save the number return by millis() on a file
     */
    void saveMillisInFile();

    /**
     * Check and start the SD card
     */
    bool wakeupSD();
    
  public : 
    /**
     * Maker of the class
     */
    QameleO_SD();

    /**
     * Return all data saved but not send
     */
    Pile<String> getDataToSend();

    /**
     * Remove the file use to save the data not send
     */
    bool resetFileDataToSend();

    /**
     * Read the file backup
     * 
     * @param idReboot  - the id of reboot 
     * @param nbRebootMax - the number of program restarts
     * @return the time millis() save on the file for the corresponding id 
     */
    long readBackup(int idReboot, int nbRebootMax);

    /**
     * Read on a file, and 
     * 
     * @return the number of reboot
     */
    int readNbReboot();

    /**
     * Used before a reboot of the program. 
     * Call saveMillisInFile() & saveNbRebootInFile(int n)
     */
    void saveOnBackup(int n);
    
    /**
     * Call wakeupSD and saveDataIn
     * 
     * @param msg - The argument of saveDataIn
     */
    void saveToSD(String msg);

    /**
     * Setup the pin of the SD card
     */
    void setupSD();

    /**
     * This is a temporary method. This use to remove file without micro sd card reader.
     */
    void temporaryMethod();

    /**
     * This method is use to read on the "backup" files without micro sd card reader.
     */
    void temporaryMethod2();

    /**
     * This method is use to read the data save but not send without micro sd card reader.
     */
    void temporaryMethod3();
   
};
#endif //QAMELEO_SD_H
