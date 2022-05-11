/*
 * SPS30.h
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 */

#ifndef SPS30_H_
#define SPS30_H_


#include "PM_Sensor.h"

#define SPS_30_BAUD 115200
#define SPS_30_COMMAND_DELAY 5000

class SPS30 : PM_Sensor
{

public:
	enum CommandResponse { ERROR, PM_MEASURE, START_MEASURE, STOP_MEASURE };


	union ArrayToInteger {
	 byte array[4];
	 uint32_t integer;
	};

	union ArrayToFloat {
	 byte array[4];
	 float float_value;
	};

  bool read_1min(PM_DATA & data);
  bool read_10sec(PM_DATA & data);
  bool read_5min(PM_DATA & data);

  void configure();
  bool shutdown();
  bool powerOn();
  bool read(PM_DATA & data);
  SPS30(HardwareSerial&, int p);
  SPS30(HardwareSerial&);

protected:
  CommandResponse readData();
  void readPM(byte * data, int ml);

private:
  ArrayToFloat converterFloat;

};




#endif /* SPS30_H_ */
