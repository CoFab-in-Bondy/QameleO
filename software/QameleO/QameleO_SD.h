#ifndef QAMELEO_SD_H
#define QAMELEO_SD_H

#include <SD.h>

class QameleO_SD {
  private:
    File _myFile;

    uint32_t _position;                             // Current read/write position in the buffer file
    uint32_t _nextPosition;                          // Next read/write position in the buffer file

    /** 
     * @brief Save data in the buffer file
     * 
     * @param msg The message to save
     * @return true if the message was successfully saved, false otherwise
     */
    bool _saveDataInBufferFile(const char * msg);

    /** 
     * @brief Save data in the log file
     * 
     * @param msg The message to save
     * @return true if the message was successfully saved, false otherwise
     */
    bool _saveDataInLogFile(const char * msg);

    /** 
     * @brief Save the current millis in the file
     */
    void _saveMillisInFile();



  public:
    /**
     * @brief Construct a new QameleO_SD object
     */
    QameleO_SD();

    /**
     * @brief Initialize the SD card
     * 
     * @return true if the SD card was successfully initialized, false otherwise
     */
    bool begin();

    /**
     * @brief Reset the SD card
     * 
     * @return true if the SD card was successfully reset, false otherwise
     */
    bool resetSD();

    /**
     * @brief Save a message to the SD buffer
     * 
     * @param msg The message to save
     * @return true if the message was successfully saved, false otherwise
     */
    bool saveToSDBuffer(const char * msg);

    /**
     * @brief Save a message to the SD log
     * 
     * @param msg The message to save
     * @return true if the message was successfully saved, false otherwise
     */
    bool saveToSDLog(const char * msg);

    /**
     * @brief Read the next message from the SD buffer
     * 
     * @param buffer The buffer to store the message
     * @param size The size of the buffer
     * @return true if a message was successfully read, false otherwise
     */
    bool readNextMessage(char * buffer, size_t size);

    /**
     * @brief Confirm that the last read message was sent
     * 
     * @return true if the message was successfully confirmed, false otherwise
     */
    bool confirmMessageSent();

    /**
     * @brief Check if there are pending messages in the SD buffer
     * 
     * @return true if there are pending messages, false otherwise
     */
    bool hasPendingMessages();

    /**
     * @brief Compact the SD buffer to remove confirmed messages
     * 
     * @return true if the buffer was successfully compacted, false otherwise
     */
    bool compactBuffer();

    /**
     * @brief Read the number of reboots
     * 
     * @return The number of reboots
     */
    int readNbReboot();

    /**
     * @brief Read the backup for a specific reboot
     * 
     * @param idReboot The ID of the reboot
     * @param nbRebootMax The maximum number of reboots
     * @return The backup value
     */
    long readBackup(int idReboot, int nbRebootMax);

    /**
     * @brief Save data to the backup
     * 
     * @param n The data to save
     */
    void saveOnBackup(int n);
};
#endif // QAMELEO_SD_H