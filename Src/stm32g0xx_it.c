/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
#ifdef BLUE_TOOTH_USART2_SUPPORT
extern UART_HandleTypeDef huart2;
#endif
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
	u32 r_sp = __get_PSP(); // Get SP Value

	/* __asm{
		MOV v_r14, R14;
	}*/
	Sprintf(URT_NRCMD,"\r\nSys:Hard Fault! SP(%lx)",r_sp);

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  // Software Timer Counter
  SoftwareTimerCounter();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */

  /* USER CODE END EXTI4_15_IRQn 0 */
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) 
  { 
  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
  }
  
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET) 
  { 
  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
  }
  
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET) 
  { 
  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
  }
  
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET) 
  { 
  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
  }

  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET) 
  { 
    //if(HAL_GPIO_ReadPin(PB9_BLE_INT_GPIO_Port,PA0_BLE_EN_Pin) == GPIO_PIN_SET)
    	SetBleIntIrqStatus(TRUE);
	  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
  }
  /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
  IsTimer3TimeroutFlag = TRUE;
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles I2C2 global interrupt.
  */
void I2C2_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_IRQn 0 */

  /* USER CODE END I2C2_IRQn 0 */
  if (hi2c2.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&hi2c2);
  } else {
    HAL_I2C_EV_IRQHandler(&hi2c2);
  }
  /* USER CODE BEGIN I2C2_IRQn 1 */

  /* USER CODE END I2C2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  UART_HandleTypeDef *huart=&huart1;
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  //HAL_UART_IRQHandler(&huart1);
  UARTx_IRQ_Com_Handler(huart);
  if(__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET)
  { 
	/* Read one byte from the receive data register and send it back */
	// Uart1RxBuffer[Uart1RxCount] = (u8)(huart->Instance->RDR & (u8)huart->Mask);
	// if(Uart1RxCount < (UART1_RX_BUFFER_SIZE -2))
	// 	Uart1RxCount++;
    Uart1RxBuffer[0] = (u8)(huart->Instance->RDR & (u8)huart->Mask);
    // huart->Instance->TDR = Uart1RxBuffer[0];
    // while ((__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) ? SET : RESET) == RESET);

	// Check ymodem protocol status
	SoftwareTimerReset(&UART1ReceiveTimer,UART1ReceiveTimerCallback,UART1_RECEIVE_TIMEOUT);
	SoftwareTimerStart(&UART1ReceiveTimer);
	/* Clear RXNE interrupt flag */
	__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);
  }
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

#ifdef BLUE_TOOTH_USART2_SUPPORT
/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
