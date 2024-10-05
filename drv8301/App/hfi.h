

/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: hfi.h
** \author: Huazi
** \data: 2024-09-28
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#ifndef __HFI_H_
#define __HFI_H_

#include "stdint.h"
#include "math.h"


typedef struct 
{
	float x1;
	float x2;
	float y1;
	float y2;
	float b[3];
	float a[3];
	float inputGain;
	float outputGain;
	float input;
	float output;

}IIR_Filter_t;

void HFI_Init(void);
void HFI_Process();

#endif
