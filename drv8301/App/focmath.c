/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: focmath.c
** \author: Huazi
** \data: 2022-06-04
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

/*!
    \brief      Struct_Init function   
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Struct_Init(Foc_Controller * struct_ptr)
{
	
	struct_ptr->pole_pairs = 7;
	struct_ptr->align_voltage = 2.0f;
	struct_ptr->electrical_angle = 0.0f;
	struct_ptr->offset_electrical_angle = 0.0f;
	struct_ptr->Ia = 0.0f;
	struct_ptr->Ib = 0.0f;
	struct_ptr->Ic = 0.0f;
	struct_ptr->Ia_zero = 0.0f;
	struct_ptr->Ib_zero = 0.0f;
	struct_ptr->Ic_zero = 0.0f;
	struct_ptr->i_alpha = 0.0f;
	struct_ptr->i_beta = 0.0f;
	struct_ptr->u_alpha = 0.0f;
	struct_ptr->u_beta = 0.0f;
	struct_ptr->i_d = 0.0f;
	struct_ptr->i_q = 0.0f;
	struct_ptr->u_q = 0.0f;
	struct_ptr->u_d = 0.0f;
	struct_ptr->desire_i_d = 0.0f;
	struct_ptr->desire_i_q = 0.0f;
	
	struct_ptr->Id_pid.err = 0.0f;
	struct_ptr->Id_pid.err_i = 0.0f;
	struct_ptr->Iq_pid.err = 0.0f;
	struct_ptr->Iq_pid.err_i = 0.0f;
	
	struct_ptr->Id_pid.kp = 0.457f;
	struct_ptr->Id_pid.ki = 0.03800f;
	
	struct_ptr->Iq_pid.kp = 0.457f;
	struct_ptr->Iq_pid.ki = 0.03800f;
}

/*!
    \brief      clark function   
    \param[in]  none
    \param[out] none
    \retval     none
*/
void clark(Foc_Controller * struct_ptr)
{
	struct_ptr->i_alpha = struct_ptr->Ia;
	struct_ptr->i_beta = (struct_ptr->Ia + 2.0f * struct_ptr->Ib) / sqrt3;
}

/*!
    \brief      inverse park function   
    \param[in]  none
    \param[out] none
    \retval     none
*/
void park(Foc_Controller * struct_ptr)
{
	float _Cos = cosf(struct_ptr->electrical_angle);
	float _Sin = sinf(struct_ptr->electrical_angle);

	struct_ptr->i_d =  struct_ptr->i_alpha * _Cos + struct_ptr->i_beta * _Sin;
	struct_ptr->i_q = -struct_ptr->i_alpha * _Sin + struct_ptr->i_beta * _Cos;
//	Waveform(&struct_ptr->i_q,1);	
}
/*!
    \brief      park function
	将两相旋转坐标系下的u_q,u_d,变换成alpha,beta两相静止坐标系的u_alpha,u_beta 
    \param[in]  Foc Controller Struct
    \param[out] none
    \retval     none
*/
void inverse_park(Foc_Controller * struct_ptr)
{
	float _cos = cosf(struct_ptr->electrical_angle);
	float _sin = sinf(struct_ptr->electrical_angle);
	if((struct_ptr->u_d * struct_ptr->u_d + struct_ptr->u_q * struct_ptr->u_q) > (Um * Um))
	{
		float temp = sqrt(struct_ptr->u_d * struct_ptr->u_d + struct_ptr->u_q * struct_ptr->u_q);
		struct_ptr->u_d = Um * struct_ptr->u_d/temp;
		struct_ptr->u_q = Um * struct_ptr->u_q/temp;
	}
	struct_ptr->u_alpha 	= 		struct_ptr->u_d * _cos - struct_ptr->u_q * _sin;
	struct_ptr->u_beta 	 	= 		struct_ptr->u_d * _sin + struct_ptr->u_q * _cos;
}

void compute_pid(Foc_Controller * struct_ptr)
{
	
	struct_ptr->Id_pid.err = struct_ptr->desire_i_d - struct_ptr->i_d;
	
	struct_ptr->Id_pid.err_i += struct_ptr->Id_pid.err;
	if(struct_ptr->Id_pid.err_i > 24)struct_ptr->Id_pid.err_i=24;
	else if(struct_ptr->Id_pid.err_i < -24)struct_ptr->Id_pid.err_i=-24;
	
	struct_ptr->u_d = struct_ptr->Id_pid.kp*struct_ptr->Id_pid.err + struct_ptr->Id_pid.ki * struct_ptr->Id_pid.err_i;
	
	if(struct_ptr->u_d>24.0)			  struct_ptr->u_d=24.0;
	else if(struct_ptr->u_d<=-24.0f)   struct_ptr->u_d = -24.0f;
	
	struct_ptr->Iq_pid.err = struct_ptr->desire_i_q - struct_ptr->i_q;
	
	struct_ptr->Iq_pid.err_i+=struct_ptr->Iq_pid.err;
	if(struct_ptr->Iq_pid.err_i > 24)struct_ptr->Iq_pid.err_i = 24;
	else if(struct_ptr->Iq_pid.err_i < -24)struct_ptr->Iq_pid.err_i = -24;
	
	struct_ptr->u_q = struct_ptr->Iq_pid.kp*struct_ptr->Iq_pid.err + struct_ptr->Iq_pid.ki * struct_ptr->Iq_pid.err_i;
		
	if(struct_ptr->u_q>24.0)           struct_ptr->u_q=24.0;
	else if(struct_ptr->u_q<=-24.0f)   struct_ptr->u_q = -24.0f;

	
}