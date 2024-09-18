/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: controller.c
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#include "tim.h"
#include "math.h"
#include "main.h"
#include "svpwm.h"
#include "encoder.h"
#include "focmath.h"
#include "controller.h"
#include "adc.h"
#include "stdbool.h"
#include "align.h"

extern Foc_Controller Controller;

#define SHUNT_R 0.01f   //10mR
#define V_REF 1.65f   //10mR
#define Gain  10.0f   //10mR

void speed_pid(Foc_Controller * struct_ptr)
{
	struct_ptr->Speed_pid.kp = 0.005f;
	struct_ptr->Speed_pid.ki = 0.0025f;
	
	struct_ptr->Speed_pid.err = struct_ptr->target_speed - struct_ptr->now_speed;
	
	struct_ptr->Speed_pid.err_i+=struct_ptr->Speed_pid.err;
	if(struct_ptr->Speed_pid.err_i>5000.0)struct_ptr->Speed_pid.err_i=5000.0;
	else if(struct_ptr->Speed_pid.err_i<-5000.0)struct_ptr->Speed_pid.err_i=-5000.0;
	
	struct_ptr->desire_i_q = struct_ptr->Speed_pid.kp*struct_ptr->Speed_pid.err + struct_ptr->Speed_pid.ki * struct_ptr->Speed_pid.err_i;
	
	if(struct_ptr->desire_i_q >= 24.0)	struct_ptr->desire_i_q=24.0;
	else if(struct_ptr->desire_i_q <= -24.0f)   struct_ptr->desire_i_q = -24.0f;
//	struct_ptr->desire_i_q = 24;

}


GPIO_PinState key  = 0;
uint32_t key_cnt = 0;

uint8_t key_flag  = 0;
uint8_t key_change_flag  = 0;

short Get_Count(void)
{
    return (short)(__HAL_TIM_GET_COUNTER(&htim3));
}

float angle = 0.0f;

unsigned int even_check(unsigned int v)
{
      if(v == 0) return 0;
      v ^= v >> 8;
      v ^= v >> 4;
      v ^= v >> 2;
      v ^= v >> 1;
      return v & 1;
}

int32_t tim_tick = 0;
int32_t last_tim_tick = 0;
int32_t real_cnt;

void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
	
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
	{
		if(0 == key_change_flag)
		{
			key_cnt ++;
			if(key_cnt>=20)
			{
				key_change_flag = 1;
				key_cnt = 20;
				if(1 == key_flag)
				{
				key_flag = 0;
				}
				else
				{
					key_flag = 1;
				}
			}
			else
			{
			}
		}
	}
	else
	{
		key_cnt = 0;
		key_change_flag = 0;
	}

	
	
//	if(1 == key_flag)
//	{
//		Controller.target_positon = 180;
//	}
//	else
//	{
//		Controller.target_positon = 0;
//	}
}
struct V3
{
	float a;
	float b;
	float c;
};

struct V3 Io;
struct V3 Io_Fillter;
struct V3 Vo;
uint16_t ADC1_VALUE,ADC2_VALUE,ADC3_VALUE;
uint16_t ADC1_VALUE_OFFSET,ADC2_VALUE_OFFSET,ADC3_VALUE_OFFSET;

void Cal_Current(void)
{
	ADC1_VALUE = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
	Io.a = ((ADC1_VALUE - ADC1_VALUE_OFFSET)*3.3f/4095.0f)/Gain/SHUNT_R;
	
	ADC2_VALUE = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
	Io.b =((ADC2_VALUE - ADC2_VALUE_OFFSET)*3.3f/4095.0f)/Gain/SHUNT_R;
	
	ADC3_VALUE = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3);
	Io.c = ((ADC3_VALUE - ADC3_VALUE_OFFSET)*3.3f/4095.0f)/Gain/SHUNT_R;
	
	Io_Fillter.a = 0.209439f*Io.a + (1 - 0.209439f)*Io_Fillter.a;
	Io_Fillter.b = 0.209439f*Io.b + (1 - 0.209439f)*Io_Fillter.b;
	Io_Fillter.c = 0.209439f*Io.c + (1 - 0.209439f)*Io_Fillter.c;
	
	Controller.Ia = Io_Fillter.a;
	Controller.Ib = Io_Fillter.b;
	Controller.Ic = Io_Fillter.c;

}

void Get_offset(void)
{
	ADC1_VALUE = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
	ADC1_VALUE_OFFSET = 0.209439f*ADC1_VALUE + (1 - 0.209439f)*ADC1_VALUE_OFFSET;
	
	ADC2_VALUE = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
	ADC2_VALUE_OFFSET = 0.209439f*ADC2_VALUE + (1 - 0.209439f)*ADC2_VALUE_OFFSET;
	
	ADC3_VALUE = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3);
	ADC3_VALUE_OFFSET = 0.209439f*ADC3_VALUE + (1 - 0.209439f)*ADC3_VALUE_OFFSET;
}
bool get_current_offset_flag = true;

Foc_Controller Controller;

uint8_t speed_tick = 0;
uint8_t pos_tick = 0;
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_3);
	if(true == get_current_offset_flag)
	{
		Get_offset();
	}
	else
	{
		speed_tick++;
		if(speed_tick >= 15)
		{
			speed_tick = 0;
			if(key_flag == 1)
			{
			GetMotor_Speed(&Controller);
			}
		}
		
		pos_tick++;
		if(pos_tick >= 30)
		{
			pos_tick = 0;
//			position_pid(&Controller);
			if(key_flag == 1)
			{
			speed_pid(&Controller);
			}
		}
		
		Cal_Current();
		
		//去掉按键后，直接运行电流环会出现跑不起来的问题，大概率是电流环积分导致的
		if(key_flag == 1)
		{
			run_close(&Controller);
		}
	}
}

void run_open(Foc_Controller * struct_ptr)
{
	Controller.u_q = 2.0f;
	struct_ptr->electrical_angle+=0.003f;
	inverse_park(&Controller);
	svpwm(Controller.u_alpha,Controller.u_beta);
}
void run_close(Foc_Controller * struct_ptr)
{
	struct_ptr->desire_i_d = 0.0f;
//	struct_ptr->desire_i_q = 2.0f;
	clark(struct_ptr);
	park(struct_ptr);
	compute_pid(struct_ptr);
	inverse_park(&Controller);
	svpwm(Controller.u_alpha,Controller.u_beta);
}

void Motor_Init(void)
{
	//PWM初始匿
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,5599);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,5599);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,5599);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

	//通道4触发ADC采样
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4,10);
	//弿启ADC注入转换
	HAL_ADCEx_InjectedStart_IT(&hadc1);
	HAL_ADCEx_InjectedStart_IT(&hadc2);
	HAL_ADCEx_InjectedStart_IT(&hadc3);
	
	#if 1
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,2799);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,2799);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,2799);
	#else
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	#endif

	HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_SET);
	
	HAL_Delay(500);
	get_current_offset_flag = false;

}

void main_entery(void)
{
	HAL_SYSTICK_Config(SystemCoreClock / (2000U / uwTickFreq));
	Struct_Init(&Controller);
	Motor_Init();
	Align_Alectrical_Angle(&Controller);
	
	while(1)
	{
		Read_Angle(&Controller);
	}
}
