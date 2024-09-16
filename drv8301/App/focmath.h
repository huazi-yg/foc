
/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: focmath.h
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#ifndef __FOCMATH_H_
#define __FOCMATH_H_

#include "stdint.h"
#include "math.h"

#define sqrt3   1.732050f  //sqrt(3,2.0)
#define sqrt3_2 0.866025f	//sqrt(3,2.0)/2.0
#define devision2 0.5f			//1/2
#define Udc 		  24.0f//
#define Um        0.577350f*Udc
//#define Um        0.6f*Udc
#define _PI 		3.141592f
#define _2PI 		6.283184f
#define FOC_EANGLE_TO_ERADIN (0.017453293)  //角度值转化弧度制系数

typedef struct
{
	/*当前误差*/
	float err;
	/*累计误差*/
	float err_i;
	
	/**/
	float kp;
	
	float ki;
}Pid_struct;

typedef struct
{
	/* 磁极对数 */
	uint8_t pole_pairs;
	/* 电角度对齐电压 */
	float align_voltage;
	/* 电角度 */
	float electrical_angle;
	/* 机械角度 */
	float mechanical_angle;
	/* 初始电角度 */
	float offset_electrical_angle;
	/* 采集的三相电流 */
	float Ia,Ib,Ic;
	/* 上一次采集的三相电流 */
	float p_Ia,p_Ib,p_Ic;
	/* 静态的三相电流 */
	float Ia_zero,Ib_zero,Ic_zero;
	/* 电流矢量在两相静止坐标系下的投影*/
	float i_alpha,i_beta;
	/* 电压矢量在两相静止坐标系下的投影*/
	float u_alpha,u_beta;

	float i_d,i_q;
	/* u_q控制转矩u_d控制励磁 */
	float u_d,u_q;
	/* desire_i_q控制转矩desire_i_d控制励磁 */
	float desire_i_d,desire_i_q;
	/*d轴和i轴的pid结构体*/
	Pid_struct Id_pid;
	Pid_struct Iq_pid;
	Pid_struct Speed_pid;
	
	float target_speed;
	float now_speed;
	float out_speed;
	
}Foc_Controller;


void park(Foc_Controller * struct_ptr);
void clark(Foc_Controller * struct_ptr);
void compute_pid(Foc_Controller * struct_ptr);
void Struct_Init(Foc_Controller * struct_ptr);
void inverse_park(Foc_Controller * struct_ptr);

#endif
