/*
 * SPS30.cpp
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 */





#include "Arduino.h"
#include "SPS30.h"


SPS30::SPS30(HardwareSerial& s): PM_Sensor(s)
{
	converterFloat.array[0]=0;
	converterFloat.array[1]=0;
	converterFloat.array[2]=0;
	converterFloat.array[3]=0;
}

void SPS30::configure()
{
	hstream->begin(SPS_30_BAUD,SERIAL_8N1);
}

bool SPS30::shutdown()
{
	byte message [] = {0x7E, 0x00, 0x01, 0x00, 0xFE, 0x7E };
	hstream->write(message,sizeof(message));
	delay(SPS_30_COMMAND_DELAY);
	return readData()==STOP_MEASURE;
}

bool SPS30::powerOn()
{
	byte message [] = {0x7E, 0x00, 0x00, 0x02, 0x01, 0x03, 0xF9, 0x7E };
	hstream->write(message, sizeof(message));
	delay(SPS_30_COMMAND_DELAY);
	return readData()==START_MEASURE;
}
bool SPS30::read(PM_DATA & data)
{
	byte message1 [] = {0x7E, 0x00, 0x03, 0x00, 0xFC, 0x7E};
	hstream->write(message1, sizeof(message1));
	delay(SPS_30_COMMAND_DELAY);
	this->readData();
	return readData()==PM_MEASURE;
}

SPS30::CommandResponse SPS30::readData()
{
  int hexIn = 0;
  int is_read = 0;
  unsigned int tmp = millis();
  do
  {
      int nbAvailable = hstream->available();
      if( nbAvailable >2)
      {
          byte mbuffer [hstream->available()];
          hstream->readBytes(mbuffer, nbAvailable);
          hexIn = mbuffer[2];
          switch(hexIn)
          {
          	  case 0x03: {readPM(mbuffer,nbAvailable); return PM_MEASURE;}
          	  case 0x00: {return START_MEASURE;}
          	  case 0x01: { return STOP_MEASURE;}
          }
      }
  } while(is_read == 0 && millis() - tmp < 2000);
  return ERROR;
}


void SPS30::readPM(byte * data, int ml)
{
  int i = 5;
  Serial.println("pm1 pcs/L");
  converterFloat.array[3]=data[i];
  converterFloat.array[2]=data[i+1];
  converterFloat.array[1]=data[i+2];
  converterFloat.array[0]=data[i+3];
  Serial.println(converterFloat.float_value);
  Serial.println("pm2,5 pcs/L");
  i = i +4;
  converterFloat.array[3]=data[i];
  converterFloat.array[2]=data[i+1];
  converterFloat.array[1]=data[i+2];
  converterFloat.array[0]=data[i+3];

  Serial.println(converterFloat.float_value);
  Serial.println("pm10 pcs/L");
  i = i +4;
  converterFloat.array[3]=data[i];
  converterFloat.array[2]=data[i+1];
  converterFloat.array[1]=data[i+2];
  converterFloat.array[0]=data[i+3];

  Serial.println(converterFloat.float_value);
}
