/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: svpwm.c
** \author: Huazi
** \data: 2022-06-03
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#include "math.h"
//#include "timer.h"
#include "usart.h"
#include "svpwm.h"
#include "focmath.h"
#include "tim.h"
extern Foc_Controller Controller;

int t1,t2,t3;
float Ta,Tb,Tc;
void svpwm(float u_alpha,float u_beta)
{
	float uref1,uref2,uref3;
	float T1,T2,T3,T4,T5,T6,T7,Ts=1.0f;
	//
	uref1 = 	u_beta;
	uref2 = 	sqrt3_2*u_alpha - u_beta/2.0f;
	uref3 = -	sqrt3_2*u_alpha - u_beta/2.0f;
	
	uint8_t a=0,b=0,c=0,n,sectors;
	
	if(uref1>0) a = 1;
	if(uref2>0) b = 1;
	if(uref3>0) c = 1;
	
	n = 4*c + 2*b + a;
	//判断扇区
	switch(n)
	{
		case 1:sectors = 2;break;
		case 2:sectors = 6;break;
		case 3:sectors = 1;break;
		case 4:sectors = 4;break;
		case 5:sectors = 3;break;
		case 6:sectors = 5;break;
	}
	float K = sqrt3*Ts/Udc;

	switch(sectors){
	case 1:
		T4 = uref2*K;	//计算作用时间
		T6 = uref1*K;
		T7 = (Ts - T4 - T6)/2.0f;
		Ta = T7;
		Tb = T7 + T4;
		Tc = T7 + T4 + T6;

		break;
	case 2:
		T6 = -uref3*K;
		T2 = -uref2*K;
		T7 = (Ts - T2 - T6)/2.0f;
		Ta = T7 + T2;
		Tb = T7;
		Tc = T2 + T6 + T7;
		break;
	case 3:
		T2 = uref1*K;
		T3 = uref3*K;
		T7 = (Ts - T2 - T3)/2.0f;
		Ta = T7 + T2 + T3;
		Tb = T7;
		Tc = T7 + T2;
		break;
	case 4:	
		T1 = -uref1*K;
		T3 = -uref2*K;
		T7 = (Ts - T3 - T1)/2.0f;
		Ta = T7 + T1 + T3;
		Tb = T7 + T1;
		Tc = T7;
		break;
	case 5:
		T1 = uref3*K;
		T5 = uref2*K;
		T7 = (Ts - T1 - T5)/2.0f;
		Ta = T7 + T1;
		Tb = T7 + T1 + T5;
		Tc = T7;			
		break;
	case 6:
		T4 = -uref3*K;
		T5 = -uref1*K;
		T7 = (Ts - T4 - T5)/2.0f;
		Ta = T7;
		Tb = T7 + T4+ T5;
		Tc = T7 + T4;
		break;
	}
	
	t1 = Ta*5599;
	if(t1>5599)t1 = 5599;
	else if(t1<0)t1 = 0;
	t2 = Tb*5599;
	if(t2>5599)t2 = 5599;
	else if(t2<0)t2 = 0;
	t3 = Tc*5599;
	if(t3>5599)t3 = 5599;
	else if(t3<0)t3 = 0;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,t1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,t2);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,t3);
}


