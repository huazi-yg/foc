/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define VC_FB_Pin GPIO_PIN_0
#define VC_FB_GPIO_Port GPIOC
#define OCTWSDI_Pin GPIO_PIN_1
#define OCTWSDI_GPIO_Port GPIOC
#define IA_FB_Pin GPIO_PIN_0
#define IA_FB_GPIO_Port GPIOA
#define IB_FB_Pin GPIO_PIN_1
#define IB_FB_GPIO_Port GPIOA
#define LPUART1_TX_Pin GPIO_PIN_2
#define LPUART1_TX_GPIO_Port GPIOA
#define LPUART1_RX_Pin GPIO_PIN_3
#define LPUART1_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define VA_FB_Pin GPIO_PIN_6
#define VA_FB_GPIO_Port GPIOA
#define PWM_AL_Pin GPIO_PIN_7
#define PWM_AL_GPIO_Port GPIOA
#define VB_FB_Pin GPIO_PIN_4
#define VB_FB_GPIO_Port GPIOC
#define PWM_BL_Pin GPIO_PIN_0
#define PWM_BL_GPIO_Port GPIOB
#define DC_V_FB_Pin GPIO_PIN_1
#define DC_V_FB_GPIO_Port GPIOB
#define FAULT_Pin GPIO_PIN_10
#define FAULT_GPIO_Port GPIOB
#define IC_FB_Pin GPIO_PIN_14
#define IC_FB_GPIO_Port GPIOB
#define DC_CAL_Pin GPIO_PIN_7
#define DC_CAL_GPIO_Port GPIOC
#define PWM_AH_Pin GPIO_PIN_8
#define PWM_AH_GPIO_Port GPIOA
#define PWM_BH_Pin GPIO_PIN_9
#define PWM_BH_GPIO_Port GPIOA
#define PWM_CH_Pin GPIO_PIN_10
#define PWM_CH_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define DRV8301_CS_Pin GPIO_PIN_15
#define DRV8301_CS_GPIO_Port GPIOA
#define SPI3_CS_Pin GPIO_PIN_2
#define SPI3_CS_GPIO_Port GPIOD
#define DRV8301_SCLK_Pin GPIO_PIN_3
#define DRV8301_SCLK_GPIO_Port GPIOB
#define DRV8301_MOSI_Pin GPIO_PIN_4
#define DRV8301_MOSI_GPIO_Port GPIOB
#define DRV8301_MISO_Pin GPIO_PIN_5
#define DRV8301_MISO_GPIO_Port GPIOB
#define EN_GATE_Pin GPIO_PIN_6
#define EN_GATE_GPIO_Port GPIOB
#define PWM_CL_Pin GPIO_PIN_9
#define PWM_CL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
