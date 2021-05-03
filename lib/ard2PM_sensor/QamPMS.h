/*
 *QamPMS.h
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 */

#ifndef QAM_PMS_H
#define QAM_PMS_H

#define PMS7003 1
#define SPS30 2
#define NEXT_PM 3

#define PM_SENSOR_SET_PIN 10 

#include "Stream.h"


class PMS
{
public:



  struct DATA {
    // Standard Particles, CF=1
    uint16_t PM_UG_1_0;
    uint16_t PM_UG_2_5;
    uint16_t PM_UG_10_0;


  };

private:
  Stream* _stream;


};

#endif
