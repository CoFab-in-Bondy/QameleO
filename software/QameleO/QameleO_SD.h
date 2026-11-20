#ifndef QAMELEO_SD_H
#define QAMELEO_SD_H

#include <SD.h>

class QameleO_SD {
  private:
    File _myFile;

    uint32_t _position = 0;
    uint32_t _nextPosition  = 0;

  public:

    bool begin();

    bool saveDataInBufferFile(const char * msg);

    bool readNextMessage(char * buffer, size_t size);

    bool confirmMessageSent();
};

#endif // QAMELEO_SD_H