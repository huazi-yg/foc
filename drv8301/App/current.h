
/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: current.h
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#ifndef __CURRENT_H_
#define __CURRENT_H_
#include "stdint.h"
void zero_current(uint32_t wait_tick);
void cal_current(uint16_t adc_in1,uint16_t adc_in2);

#endif
