/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
void update_leds(uint16_t value);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define buttonInt0_Pin GPIO_PIN_1
#define buttonInt0_GPIO_Port GPIOB
#define buttonInt0_EXTI_IRQn EXTI1_IRQn
#define buttonInt1_Pin GPIO_PIN_2
#define buttonInt1_GPIO_Port GPIOB
#define buttonInt1_EXTI_IRQn EXTI2_IRQn
#define led0_Pin GPIO_PIN_9
#define led0_GPIO_Port GPIOA
#define led1_Pin GPIO_PIN_10
#define led1_GPIO_Port GPIOA
#define led2_Pin GPIO_PIN_11
#define led2_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
extern uint16_t counter;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
