/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef enum{
	IDLE,
	SENDING,
	SENT,
	RECEIVING,
	RECEIVED,
	NOT_RESPONSE
} TransmissionStatus;
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
#define KEY_R2_Pin GPIO_PIN_0
#define KEY_R2_GPIO_Port GPIOC
#define KEY_R3_Pin GPIO_PIN_1
#define KEY_R3_GPIO_Port GPIOC
#define KEY_C3_Pin GPIO_PIN_0
#define KEY_C3_GPIO_Port GPIOA
#define KEY_C3_EXTI_IRQn EXTI0_IRQn
#define KEY_C2_Pin GPIO_PIN_1
#define KEY_C2_GPIO_Port GPIOA
#define KEY_C2_EXTI_IRQn EXTI1_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define KEY_C1_Pin GPIO_PIN_4
#define KEY_C1_GPIO_Port GPIOA
#define KEY_C1_EXTI_IRQn EXTI4_IRQn
#define RFID_SCK_Pin GPIO_PIN_5
#define RFID_SCK_GPIO_Port GPIOA
#define RFID_MISO_Pin GPIO_PIN_6
#define RFID_MISO_GPIO_Port GPIOA
#define RFID_MOSI_Pin GPIO_PIN_7
#define RFID_MOSI_GPIO_Port GPIOA
#define KEY_R4_Pin GPIO_PIN_0
#define KEY_R4_GPIO_Port GPIOB
#define KEY_R1_Pin GPIO_PIN_10
#define KEY_R1_GPIO_Port GPIOB
#define RFID_SDA_Pin GPIO_PIN_8
#define RFID_SDA_GPIO_Port GPIOA
#define DOOR_ENABLE_Pin GPIO_PIN_9
#define DOOR_ENABLE_GPIO_Port GPIOA
#define BUZZER_ENABLE_Pin GPIO_PIN_10
#define BUZZER_ENABLE_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SONAR_TRIG_Pin GPIO_PIN_4
#define SONAR_TRIG_GPIO_Port GPIOB
#define SONAR_ECHO_Pin GPIO_PIN_5
#define SONAR_ECHO_GPIO_Port GPIOB
#define LCD_SCL_Pin GPIO_PIN_6
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_7
#define LCD_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef enum{
	FALSE,
	TRUE
} BoolType;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
