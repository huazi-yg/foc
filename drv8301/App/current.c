/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: current.c
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#include "usart.h"
#include "stdio.h"
//#include "systick.h"
#include "focmath.h"
#include "current.h"
//#include "gd32c10x.h"
extern Foc_Controller Controller;
/* 12k/300r */
#define Gain 			40.000000f
#define R_shunt  		0.001000f
/* Gain*0.001 */
#define Gain_R_shunt 	0.04f
#define V_ref 			1.650000f
/* 3.3/4095 */
#define Vcc_resolution 	0.000805f

void cal_current(uint16_t adc_in1,uint16_t adc_in2)
{
	Controller.Ia = ((float)adc_in1*Vcc_resolution - V_ref)/Gain_R_shunt - Controller.Ia_zero;
	Controller.Ib = ((float)adc_in2*Vcc_resolution - V_ref)/Gain_R_shunt - Controller.Ib_zero;
	Controller.Ic = -Controller.Ia -Controller.Ib;
	
	Controller.Ia = 0.1f*Controller.Ia+0.9f*Controller.p_Ia;
	Controller.Ib = 0.1f*Controller.Ib+0.9f*Controller.p_Ib;
	Controller.Ic = 0.1f*Controller.Ic+0.9f*Controller.p_Ic;
	
	Controller.p_Ia = Controller.Ia;
	Controller.p_Ib = Controller.Ib;
	Controller.p_Ic = Controller.Ic;
	
	
}
void zero_current(uint32_t wait_tick)
{
//	uint32_t tick_start = get_tick();
//	float pre_ia = 0.000000f,pre_ib=0.000000f;
//	/* 关闭adc中断 */
//	adc_interrupt_disable(ADC0, ADC_INT_EOIC);
//	adc_interrupt_disable(ADC1, ADC_INT_EOIC);
//	while(get_tick()-tick_start<wait_tick)
//	{
//		uint16_t adc_in1 = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
//		uint16_t adc_in2 = adc_inserted_data_read(ADC1, ADC_INSERTED_CHANNEL_0);
//		Controller.Ia = ((float)adc_in1*Vcc_resolution - V_ref)/Gain_R_shunt;
//		Controller.Ib = ((float)adc_in2*Vcc_resolution - V_ref)/Gain_R_shunt;
//		Controller.Ia_zero = Controller.Ia*0.100000f + pre_ia*0.900000f;
//		Controller.Ib_zero = Controller.Ib*0.100000f + pre_ib*0.900000f;
//		pre_ia = Controller.Ia;
//		pre_ib = Controller.Ib;
//	}
////	/* 打开adc中断 */
//	adc_interrupt_enable(ADC0, ADC_INT_EOIC);
//	adc_interrupt_enable(ADC1, ADC_INT_EOIC);
}
