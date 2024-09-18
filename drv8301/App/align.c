/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: align.c
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#include "align.h"
#include "svpwm.h"
#include "main.h"
#include "encoder.h"
#include "focmath.h"
#include "tim.h"
extern Foc_Controller Controller;

unsigned int even_check(unsigned int v);
	
void Align_Alectrical_Angle(Foc_Controller* struct_ptr)
{
	struct_ptr->u_d = struct_ptr->align_voltage;
	struct_ptr->u_q = 0.0f;
	struct_ptr->offset_electrical_angle = 0.000000f;
	inverse_park(struct_ptr);
	svpwm(struct_ptr->u_alpha,struct_ptr->u_beta);
	HAL_Delay(500);
	//2.读取角度
	uint16_t temp_angle;
    for (uint8_t i = 0; i < 10; i++) {
//        Read_Angle(struct_ptr);
		temp_angle = AS5047_read(ANGLEUNC);
//        printf("1:float%f\r\n",motor_a.angle);
        HAL_Delay(10);
    }
	struct_ptr->offset_electrical_angle = temp_angle;
	struct_ptr->u_q = 0.0f;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,5599);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,5599);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,5599);
}
