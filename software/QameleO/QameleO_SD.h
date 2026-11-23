#ifndef QAMELEO_SD_H
#define QAMELEO_SD_H

#include <SD.h>

class QameleO_SD {
  private:
    File _myFile;

    uint32_t _position;
    uint32_t _nextPosition;


    bool _saveDataInBufferFile(const char * msg);

    bool _saveDataInLogFile(const char * msg);

  public:
    QameleO_SD();

    bool begin();

    bool saveToSDBuffer(const char * msg);

    bool readNextMessage(char * buffer, size_t size);

    bool confirmMessageSent();
};

#endif // QAMELEO_SD_H