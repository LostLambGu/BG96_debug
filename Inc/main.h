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
#include "stm32g0xx_hal.h"

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
#define PB9_BLE_INT_Pin GPIO_PIN_9
#define PB9_BLE_INT_GPIO_Port GPIOB
#define PB9_BLE_INT_EXTI_IRQn EXTI4_15_IRQn
#define PA0_BLE_EN_Pin GPIO_PIN_0
#define PA0_BLE_EN_GPIO_Port GPIOA
#define PA1_BLE_WAKE_Pin GPIO_PIN_1
#define PA1_BLE_WAKE_GPIO_Port GPIOA
#ifdef BLUE_TOOTH_USART2_SUPPORT
#define PA2_URT2_RX_Pin GPIO_PIN_2
#define PA2_URT2_RX_GPIO_Port GPIOA
#define PA3_URT2_TX_Pin GPIO_PIN_3
#define PA3_URT2_TX_GPIO_Port GPIOA
#endif
#define PA4_MDM_RST_Pin GPIO_PIN_4
#define PA4_MDM_RST_GPIO_Port GPIOA
#define PA5_RELAY_Pin GPIO_PIN_5
#define PA5_RELAY_GPIO_Port GPIOA
#define PA6_IGNI_INT_Pin GPIO_PIN_6
#define PA6_IGNI_INT_GPIO_Port GPIOA
#define PA6_IGNI_INT_EXTI_IRQn EXTI4_15_IRQn
#define PA7_BAT_EN_Pin GPIO_PIN_7
#define PA7_BAT_EN_GPIO_Port GPIOA
#define PB0_12V_ADC_Pin GPIO_PIN_0
#define PB0_12V_ADC_GPIO_Port GPIOB
#define PB1_BAT_ADC_Pin GPIO_PIN_1
#define PB1_BAT_ADC_GPIO_Port GPIOB
#define PB2_MDM_WAKE_Pin GPIO_PIN_2
#define PB2_MDM_WAKE_GPIO_Port GPIOB
#define PA8_ACC_INT1_Pin GPIO_PIN_8
#define PA8_ACC_INT1_GPIO_Port GPIOA
#define PA8_ACC_INT1_EXTI_IRQn EXTI4_15_IRQn
#define PA9_URT1_RX_Pin GPIO_PIN_9
#define PA9_URT1_RX_GPIO_Port GPIOA
#define PC6_BUZZER_Pin GPIO_PIN_6
#define PC6_BUZZER_GPIO_Port GPIOC
#define PA10_URT1_TX_Pin GPIO_PIN_10
#define PA10_URT1_TX_GPIO_Port GPIOA
#define PA11_I2C2_SCL_Pin GPIO_PIN_11
#define PA11_I2C2_SCL_GPIO_Port GPIOA
#define PA12_I2C2_SDA_Pin GPIO_PIN_12
#define PA12_I2C2_SDA_GPIO_Port GPIOA
#define PA15_DCDC_EN_Pin GPIO_PIN_15
#define PA15_DCDC_EN_GPIO_Port GPIOA
#define PB3_MDM_EN_Pin GPIO_PIN_3
#define PB3_MDM_EN_GPIO_Port GPIOB
#define PB4_MITI_GPIO_Pin GPIO_PIN_4
#define PB4_MITI_GPIO_GPIO_Port GPIOB
#define PB5_MDM_WINT_Pin GPIO_PIN_5
#define PB5_MDM_WINT_GPIO_Port GPIOB
#define PB5_MDM_WINT_EXTI_IRQn EXTI4_15_IRQn
#define PB6_CHG_EN_Pin GPIO_PIN_6
#define PB6_CHG_EN_GPIO_Port GPIOB
#define PB7_ACC_INT2_Pin GPIO_PIN_7
#define PB7_ACC_INT2_GPIO_Port GPIOB
#define PB7_ACC_INT2_EXTI_IRQn EXTI4_15_IRQn
#define PB8_MDM_PWK_Pin GPIO_PIN_8
#define PB8_MDM_PWK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

// Variable Declared 

//Function Declare
extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);
extern void MX_USART1_UART_Init(void);
#ifdef BLUE_TOOTH_USART2_SUPPORT
extern void MX_USART2_UART_Init(void);
#endif
extern void MX_DMA_Init(void);
extern void MX_ADC1_Init(void);
extern void MX_I2C2_Init(void);
extern void MX_TIM3_Init(void);
extern void MX_IWDG_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
