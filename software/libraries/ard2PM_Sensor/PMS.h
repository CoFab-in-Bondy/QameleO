/*
 *PMS.h
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 */


#ifndef PMS_H
#define PMS_H

#include "Stream.h"

struct DATA {
    // Standard Particles, CF=1
    uint16_t PM_SP_UG_1_0;
    uint16_t PM_SP_UG_2_5;
    uint16_t PM_SP_UG_10_0;

    // Atmospheric environment
    uint16_t PM_AE_UG_1_0;
    uint16_t PM_AE_UG_2_5;
    uint16_t PM_AE_UG_10_0;

    uint16_t PM_SP_PPM_0_3;
    uint16_t PM_SP_PPM_0_5;
    uint16_t PM_SP_PPM_1_0;
    uint16_t PM_SP_PPM_2_5;
    uint16_t PM_SP_PPM_5_0;
    uint16_t PM_SP_PPM_10_0;

  };


class PMS
{
public:
  static const uint16_t SINGLE_RESPONSE_TIME = 1000;
  static const uint16_t TOTAL_RESPONSE_TIME = 1000 * 10;
  static const uint16_t STEADY_RESPONSE_TIME = 1000 * 30;

  static const uint16_t BAUD_RATE = 9600;


  PMS(Stream&);

  void begin();
  void sleep();
  void wakeUp();
  void activeMode();
  void passiveMode();

  void requestRead();
  bool read(DATA & data);
  bool readUntil( DATA &,  uint16_t timeout = SINGLE_RESPONSE_TIME);

private:
  enum STATUS { STATUS_WAITING, STATUS_OK };
  enum MODE { MODE_ACTIVE, MODE_PASSIVE };

  uint8_t _payload[12];
  Stream* _stream;
  DATA* _data;
  STATUS _status =STATUS_OK ;
  MODE _mode = MODE_ACTIVE;

  uint8_t _index = 0;
  uint16_t _frameLen;
  uint16_t _checksum;
  uint16_t _calculatedChecksum;

  void loop();
};

#endif
