/*******************************************************************************
* File Name          : usr_initial.c
* Author               : Jevon
* Description        : This file provides all the usrtimer functions.

* History:
*  04/30/2019 : usr_initial V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern IWDG_HandleTypeDef hiwdg;

// Delay Us Time
void DelayUsTime(uint16_t time)
{
	u16 Count;
	Count = time;
	while(--Count);
	while(--time);
}

void TickHardwareWatchdog(u8 PrintoutEn)
{
	u8 IwdRefreshResult = 0;
	// Refresh IWDG: reload counter
	IwdRefreshResult = HAL_IWDG_Refresh(&hiwdg);
	if(PrintoutEn == TRUE && IwdRefreshResult != 0)
	{
		Sprintf(URT_NRCMD,"\r\n:iwdg tick %d",IwdRefreshResult);
	}
}

void SystemInitialization(void)
{
	InitSoftwareTimers();
	UartBufferInit();
	ShowSoftVersion();

	HAL_Delay(100);

	// Modem power onsequence
	MDM_PWR_KEY_Control(PRESS);
	HAL_Delay(500);
	MDM_PWR_KEY_Control(RELEASE);
	
	// Mems init
	//Lis3dhInitialize();
	// Lis3dhCheckWhoAmI();

	// #ifdef NUCLEO_G031K8_BOARD_DEBUG
	// // GREEN LED ON
	// HAL_GPIO_WritePin(PC6_BUZZER_GPIO_Port, PC6_BUZZER_Pin, GPIO_PIN_RESET);
	// #endif
}

void CheckSysStatusTimerCallback(u8 Status)
{
	#ifdef NUCLEO_G031K8_BOARD_DEBUG
	HAL_GPIO_TogglePin(PC6_BUZZER_GPIO_Port, PC6_BUZZER_Pin);
	#endif

	// Reset Check Sys Stat Timer
	SoftwareTimerReset(&CheckSysStatusTimer,CheckSysStatusTimerCallback,CHECK_SYS_STAT_TIMEOUT);
	SoftwareTimerStart(&CheckSysStatusTimer);
}


/*******************************************************************************
End Of The File
*******************************************************************************/

