/*
 *PMS7003.cpp
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 */


#include "Arduino.h"
#include "PMS7003.h"

#include "PMS.h"

PMS7003::PMS7003(HardwareSerial& s) : PMS7003(s,PMS_7003_SLEEP_PIN)
{}


PMS7003::PMS7003(HardwareSerial& s,  int p) : PM_Sensor(s)
{
	this->driver = new PMS(s);
	this->pin = p;
}


void PMS7003::configure()
{
	this->hstream->begin(9600);
	this->driver->activeMode();
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

bool PMS7003::shutdown()
{
	(this->driver)->sleep();
	delay(500);
	digitalWrite(pin,LOW);
	return true;
}

bool PMS7003::powerOn()
{
	digitalWrite(pin,HIGH);
	delay(500);
	(this->driver)->wakeUp();
	return true;
}

void PMS7003::cleanSensor()
{
	//do nothing
}


bool PMS7003::read(PM_DATA & data)
{
	driver -> requestRead();
	delay(500);
	if (driver->readUntil(sub_data))
	{

		data.PM_PPM_10_0 = sub_data.PM_SP_PPM_10_0;
		data.PM_PPM_1_0 = sub_data.PM_SP_PPM_1_0;
		data.PM_PPM_2_5 = sub_data.PM_SP_PPM_2_5;
		Serial.println(sub_data.PM_SP_PPM_2_5 );
		data.PM_UG_10_0 = sub_data.PM_SP_UG_10_0;
		data.PM_UG_2_5 = sub_data.PM_SP_UG_2_5 ;
		data.PM_UG_1_0 = sub_data.PM_SP_UG_1_0 ;
		return true;

	}
	return false;
}

