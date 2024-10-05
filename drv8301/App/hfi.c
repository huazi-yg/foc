/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: hfi.c
** \author: Huazi
** \data: 2024-09-28
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#include "focmath.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"
#include "usart.h"
#include "hfi.h"
#include "arm_math.h"
#include "dataAcq.h"

IIR_Filter_t D_IIR_LPF_PARAM;
IIR_Filter_t Q_IIR_LPF_PARAM;


void IIR_Filter_Init(IIR_Filter_t *iir_filter,float *param)
{

	iir_filter->b[0] = param[0];
	iir_filter->b[1] = param[1];
	iir_filter->b[2] = param[2];
	iir_filter->a[0] = param[3];
	iir_filter->a[1] = param[4];
	iir_filter->a[2] = param[5];
	iir_filter->inputGain = param[6];
	iir_filter->outputGain = param[7];
	
	iir_filter->x1 =  0.0f;
	iir_filter->x2 = 0.0f;
	iir_filter->y1 =  0.0f;
	iir_filter->y2 = 0.0f;

}

void IIR_Filter_Run(float input,float *out,IIR_Filter_t * filter)
{

	// 应用输入增益
	filter->input = input;
	
    input *= filter->inputGain;

	// 计算中间值 temp
    float temp = input - filter->a[1] * filter->y1 - filter->a[2] * filter->y2;

    // 计算输出值
    float output = filter->b[0] * temp + filter->b[1] * filter->x1 + filter->b[2] * filter->x2;

	output *= filter->outputGain;
    // Shift delay line variables
    filter->x2 = filter->x1;
    filter->x1 = input;
    filter->y2 = filter->y1;
    filter->y1 = output;
	
	filter->output = output;

}

void HFI_Init(void)
{
	float param[8] = {1.0f,2.0f,1.0f,1.0f,-1.970382f,0.970815f,0.000108f,1.0f};
	IIR_Filter_Init(&D_IIR_LPF_PARAM,param);
}

uint32_t temp1 = 0;
volatile float angle20hz;
volatile float sin20hz;

uint32_t temp2 = 0;
volatile float angle500hz;
volatile float sin500hz;

volatile float sin50_500hz;
float lfp_out;

void HFI_Process()
{
	if(temp1>750)
	{
		temp1 = 0;
	}
	angle20hz = (float)(temp1/750.0f)*2.0f*PI;
	sin20hz = arm_sin_f32(angle20hz);
	temp1++;
	
	if(temp2 >30)
	{
		temp2 = 0;
	}
	angle500hz = (float)(temp2/30.0f)*2.0f*PI;
	sin500hz = arm_sin_f32(angle500hz);
	temp2++;
	
	sin50_500hz = sin500hz+sin20hz;
	IIR_Filter_Run(sin50_500hz,&lfp_out,&D_IIR_LPF_PARAM);
	
}