/*
 *PMS7003.h
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 */


#ifndef PMS7003_H
#define PMS7003_H

#include "PMS.h"
#include "PM_Sensor.h"


#define PMS_7003_BAUD 9600 
#define PMS_7003_SLEEP_PIN 7

class PMS7003 : public PM_Sensor
{
public:


  void configure();
  bool shutdown();
  bool powerOn();
  void cleanSensor();
  bool read(PM_DATA & data);


  PMS7003(HardwareSerial&, int p);
  PMS7003(HardwareSerial&);

protected:
	PMS* driver;
	DATA sub_data;
	int pin = PMS_7003_SLEEP_PIN;

};


#endif
