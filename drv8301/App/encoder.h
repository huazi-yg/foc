
/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: encoder.h
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "stdint.h"
#include "controller.h"
// AS5047p 地址
#define NOP 0x0000
#define ERRFL 0x0001
#define PROG 0x0003
#define DIAAGC 0x3FFC
#define MAG 0x3FFD
#define ANGLEUNC 0x3FFE
#define ANGLECOM 0x3FFF

#define ZPOSM 0x0016
#define ZPOSL 0x0017
#define SETTINGS1 0x0018
#define SETTINGS2 0x0019


float Get_Absolute_Rad();
float Get_Absolute_Angle();
void Read_Angle(Foc_Controller *motor);
void GetMotor_Speed(Foc_Controller *motor);
float Get_Electrical_Angle();
uint16_t Read_Register(uint16_t addr);
uint16_t Parity_bit_Calculate(uint16_t data_2_cal);
uint16_t AS5047_read(uint16_t add);


#endif
