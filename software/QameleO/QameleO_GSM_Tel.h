#ifndef QAMELEO_GSM_TEL_H
#define QAMELEO_GSM_TEL_H

#include <TEL0177.h>

class QameleO_GSM_Tel 
{
  private:
    TEL0177 _modem;

    bool _ensureData();

    bool _ensureMqtt();


  public:
    /**
     * 
     */
    QameleO_GSM_Tel();

    /**
     * 
     */
    bool begin();


    bool sendData(const char * msg);

};

#endif // QAMELEO_GSM_TEL_H