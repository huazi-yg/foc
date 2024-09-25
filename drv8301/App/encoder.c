/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: encoder.c
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#include "spi.h"
#include "encoder.h"
#include "main.h"
#include "focmath.h"
#include "stdbool.h"
#include "dataAcq.h"

#define AS5407P_CS_L HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
#define AS5407P_CS_H HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);

extern Foc_Controller Controller;

// 计算奇偶函数
uint16_t Parity_bit_Calculate(uint16_t data_2_cal)
{
	uint16_t parity_bit_value=0;
	while(data_2_cal != 0)
	{
		parity_bit_value ^= data_2_cal; 
		data_2_cal >>=1;
	}
	return (parity_bit_value & 0x1); 
}

uint16_t rxdata1;

uint8_t flag = 0;

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi)
{
	
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi)
{
	flag = 1;
}

uint16_t rxdata2[10];
// SPI发送读取函数
uint16_t SPI_ReadWrite_OneByte(uint16_t _txdata)
{
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
	uint16_t rxdata;
	
	HAL_SPI_TransmitReceive_DMA(&hspi3, (uint8_t *)&_txdata,(uint8_t*)&rxdata1,1);
	while(0 == flag);
	flag = 0;
	
    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
	return rxdata1;
}

float angle_diff; 
float mangle = 0.0f;
float last_mangle = 0.0f;

float test_mangle = 0.0f;
float test_last_mangle = 0.0f;
// 速度计算函数
void GetMotor_Speed(Foc_Controller *motor)
{

   static float last_speed = 0.0f;
    float temp_speed = 0.0f;
	

    mangle = motor->mechanical_angle;
	
	// 角度标准化到 [0, 360)
	mangle = fmod(mangle + 360.0f, 360.0f);

    #define FREQ_SAM 500.0f // 假设采样频率为1000Hz
    #define MAX_RPM 5000.0f // 最大速度限制

	test_mangle = mangle;
	test_last_mangle = last_mangle;
    // 处理角度跨越
    angle_diff = mangle - last_mangle;

    if (fabs(angle_diff) > 180.0f) {
        // 跨越360度的情况
        if (mangle > last_mangle) {
            // 从负方向跨越
            angle_diff -= 360.0f;
        } else {
            // 从正方向跨越
            angle_diff += 360.0f;
        }
    }

    // 计算速度
    temp_speed = 60.0f * FREQ_SAM * angle_diff / 360.0f; // delta_theta / t_sample * 60 / 360 => rpm

    // 限制速度
    if (fabs(temp_speed) > MAX_RPM) {
        temp_speed = copysign(MAX_RPM, temp_speed); // 使用copysign函数保留符号
    }

    // 低通滤波器
    motor->now_speed = 0.333333f * temp_speed + 0.699999f * last_speed;
	DumpTrace();
    // 更新状态变量
    last_mangle = mangle;
    last_speed = temp_speed;
}

uint16_t tx_data[2] = {0,0};
uint16_t rx_data[2] = {0,0};
// 读取as5047p角度
uint16_t AS5047_read(uint16_t add,uint8_t *error_flag)
{
	uint16_t data;
	add |= 0x4000;	// 读指令 bit14 置1
	if(Parity_bit_Calculate(add)==1) add=add|0x8000; //如果前15位 1的个数位偶数，则Bit15 置1
	SPI_ReadWrite_OneByte(add);		//发送一条指令，不管读回的数据
	data=SPI_ReadWrite_OneByte(NOP|0x4000); //发送一条空指令，读取上一次指令返回的数据。

	if(Parity_bit_Calculate(data & 0x7FFF) == ((data >> 15)&0x01))
	{
		data &=0x3fff;
		*error_flag = 0;
		return data;
	}
	else
	{
		*error_flag = 1;
		return 0;
	}
	
	
}

float test_angle;
uint32_t test_data ;
uint32_t error_flag ;
uint32_t test_diaagc ;
// 角度(弧度)获取
void Read_Angle(Foc_Controller *motor)
{
 static uint32_t data ;
   uint32_t temp_data ;
   static float angle;
    static float angle_el;
    uint8_t error = 0;

    temp_data = AS5047_read(ANGLEUNC, &error);
//	test_diaagc = AS5047_read(DIAAGC, &error);
	
    if (0 == error)
    {
        data = temp_data;
    }
    else
    {
        // 处理错误
    }
	error_flag = error*10000;
    // 转换为角度
    angle = ((float)(data - motor->offset_electrical_angle) * 360.0f) / 16384.0f;
	
	test_data = data;
	test_angle = angle;

    // 调整角度到 0-360°
    angle = fmod(angle + 360.0f, 360.0f);
	//改变方向
	 angle = 360.0f - angle;

    // 计算电气角度
    angle_el = angle * (float)motor->pole_pairs;

    // 调整电气角度到 0-360°
    angle_el = fmod(angle_el + 360.0f, 360.0f);

    // 更新电机角度
    motor->mechanical_angle = angle;
    motor->electrical_angle = angle_el * (float)FOC_EANGLE_TO_ERADIN; // 转换为弧度 rad

    // 调整电气角度到 0-2π
//    motor->electrical_angle = fmod(motor->electrical_angle + 6.283185f, 6.283185f);
}