/*******************************************************************************
* File Name          : usr_sleep.c
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_sleep V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c2;
extern IWDG_HandleTypeDef hiwdg;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
#ifdef BLUE_TOOTH_USART2_SUPPORT
extern UART_HandleTypeDef huart2;
#endif

static void I2CKeepPinHighLevel(void)
{
	#if 1
	HAL_GPIO_DeInit(GPIOA, PA11_I2C2_SCL_Pin|PA12_I2C2_SDA_Pin);
	#else
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_DeInit(GPIOA, PA11_I2C2_SCL_Pin|PA12_I2C2_SDA_Pin);
	
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = PA11_I2C2_SCL_Pin|PA12_I2C2_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT/*GPIO_MODE_OUTPUT_PP*/;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin Output Level */
	//HAL_GPIO_WritePin(GPIOA, PA11_I2C2_SCL_Pin|PA12_I2C2_SDA_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOA, PA11_I2C2_SCL_Pin|PA12_I2C2_SDA_Pin, GPIO_PIN_RESET);
	#endif
}

void SavingPowerConsumption(void)
{
	HAL_GPIO_DeInit(GPIOA, PA6_IGNI_INT_Pin|PA8_ACC_INT1_Pin);
	HAL_GPIO_DeInit(GPIOB, PB2_MDM_WAKE_Pin|PB5_MDM_WINT_Pin|PB7_ACC_INT2_Pin|PB9_BLE_INT_Pin);
		
	// Disable ble adv
	cmd_enable_advertising(FALSE);

	// Mems
	LIS3DH_SetMode(LIS3DH_POWER_DOWN);

	#ifdef MODEM_POWEROFF_IN_DEEPSLEEP
	// Modem power onsequence
	MDM_PWR_KEY_Control(PRESS);
	HAL_Delay(700);
	MDM_PWR_KEY_Control(RELEASE);
	#endif

	// Delay
	HAL_Delay(10);

	// ADC(PB0/PB1)
	HAL_ADC_Stop(&hadc1);
	HAL_ADC_Stop_IT(&hadc1);
	HAL_ADC_DeInit(&hadc1);
	// DMA
	__HAL_RCC_DMA1_CLK_DISABLE();
	// UART1(PA9/PA10)
	UART_Interrupt_DeInit(&huart1);
	HAL_UART_DeInit(&huart1);
	#ifdef BLUE_TOOTH_USART2_SUPPORT
	// UART2(PA2/PA3)
	UART_Interrupt_DeInit(&huart2);
	HAL_UART_DeInit(&huart2);
	#endif
	// I2C(PA11/PA12)
	HAL_I2C_DeInit(&hi2c2);
	HAL_Delay(1);
	I2CKeepPinHighLevel();

	// GPIO
	SYS_CHG_EN_Control(DISABLE);	// PB6-LOW
	BLE_PWR_EN_Control(DISABLE);	// PA0-LOW
	BLE_WAKEUP_Control(SLEEP);		// PA1-LOW
	MDM_PWR_EN_Control(DISABLE);	// PB3-LOW
	MDM_PWR_KEY_Control(RELEASE);	// PB8-LOW
	MDM_RESET_Control(RESET);		// PA4-LOW
	MDM_WAKE_Control(SLEEP);		// PB2-LOW
	EXT_RELAY_Control(DISABLE);		// PA5-LOW
	BUZZER_EN_Control(DISABLE);		// PC6-LOW
	DCDC_EN_Control(DISABLE);		// PA15-LOW
	EXT_GPIO_Control(LOW);			// PB4-LOW
	BATTERY_EN_Control(ENABLE);		// PA7-HIGH

	//HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
	//HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All);
	//HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
	//HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All);
	
	// Close GPIO clock
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOA_CLK_DISABLE();
}

void RecoverNormalOperation(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_DeInit(GPIOA, PA10_URT1_TX_Pin|PA9_URT1_RX_Pin);
	#ifdef BLUE_TOOTH_USART2_SUPPORT
	HAL_GPIO_DeInit(GPIOA, PA2_URT2_RX_Pin|PA3_URT2_TX_Pin);
	#endif
	HAL_GPIO_DeInit(GPIOA, PA6_IGNI_INT_Pin|PA8_ACC_INT1_Pin);
	HAL_GPIO_DeInit(GPIOB, PB2_MDM_WAKE_Pin|PB5_MDM_WINT_Pin|PB7_ACC_INT2_Pin|PB9_BLE_INT_Pin);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = PB2_MDM_WAKE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(PB2_MDM_WAKE_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = PA6_IGNI_INT_Pin|PA8_ACC_INT1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PA6_IGNI_INT_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = PB9_BLE_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PB9_BLE_INT_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = PB5_MDM_WINT_Pin|PB7_ACC_INT2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PB5_MDM_WINT_GPIO_Port, &GPIO_InitStruct);

	// DMA
	MX_DMA_Init();
	// I2C
	HAL_I2C_DeInit(&hi2c2);
	MX_I2C2_Init();
	// UART
	MX_USART1_UART_Init();
	#ifdef BLUE_TOOTH_USART2_SUPPORT
	MX_USART2_UART_Init();
	#endif
	// ADC
	MX_ADC1_Init();
	// GPIO
	SYS_CHG_EN_Control(ENABLE);
	BLE_PWR_EN_Control(ENABLE);
	BLE_WAKEUP_Control(WAKE);
	MDM_PWR_EN_Control(ENABLE);
	MDM_PWR_KEY_Control(RELEASE);
	MDM_RESET_Control(SET);
	MDM_WAKE_Control(WAKE);
	EXT_RELAY_Control(DISABLE);
	BATTERY_EN_Control(ENABLE);
	BUZZER_EN_Control(DISABLE);
	DCDC_EN_Control(ENABLE);
	EXT_GPIO_Control(LOW);

	HAL_Delay(10);
	// Modem power onsequence
	#ifdef MODEM_POWEROFF_IN_DEEPSLEEP
	MDM_PWR_KEY_Control(PRESS);
	HAL_Delay(500);
	MDM_PWR_KEY_Control(RELEASE);
	#endif
}

void SystemEnterSleepMode(void)
{
	/* Suspend Tick */
	HAL_SuspendTick();
	
	/* Request to enter SLEEP mode */
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	
	/* Resume Tick */
	HAL_ResumeTick();
}

void SystemEnterStopMode(void)
{
	/* Suspend Tick */
	HAL_SuspendTick();
	
	/* Request to enter STOP mode */
	//HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	
	/* Resume Tick */
	HAL_ResumeTick();
}

void SystemGoIntoDeepsleepMode(SleepCtrl SleepMode)
{
	SavingPowerConsumption();
	// --------enter--------
	if(SleepMode == STOPMODE)
	{
		SystemEnterStopMode();
	}
	else if(SleepMode == WAITMODE)
	{
		SystemEnterSleepMode();
	}
	// --------exit--------
	RecoverNormalOperation();
}

void SleepModeDbgTimerCallback(u8 Status)
{
	#ifdef STOP_MODE_CURRENT_DEBUG
	// Delay
	HAL_Delay(1*1000);
	// Loop
	while (1)
	{
		// Deep sleep
		SystemGoIntoDeepsleepMode(STOPMODE);
	}
	#elif WAIT_MODE_CURRENT_DEBUG
	// Delay
	HAL_Delay(10*1000);
	// Sleep mode
	SavingPowerConsumption();
	// Loop
	while (1)
	{
	  // Start 50ms timer
	  HAL_TIM_Base_Start_IT(&htim3);
	  // Deep sleep
	  SystemEnterSleepMode();
	  // Stop 50ms timer
	  HAL_TIM_Base_Stop_IT(&htim3);
	}
	#endif
}

/*******************************************************************************
End Of The File
*******************************************************************************/

