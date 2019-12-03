/*******************************************************************************
* File Name          : usr_ioctl.c
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_ioctl V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// IO INPUT
void Show_IO_IN_Status(void)
{
	// Input IO Status
	Sprintf(URT_ATCMD,"\r\n*IO: INPUT %s %s %s %s %s",
		(ACC1_SIG_PF0_READ == GPIO_PIN_RESET) ? "ACC1 SIG PA8(LOW)" : "ACC1 SIG PA8(HIGH)", \
		(ACC2_SIG_PF0_READ == GPIO_PIN_RESET) ? "ACC2 SIG PB7(LOW)" : "ACC2 SIG PB7(HIGH)", \
		(IGNI_SIG_PA6_READ == GPIO_PIN_RESET) ? "IGNI SIG PA6(LOW)" : "IGNI SIG PA6(HIGH)", \
		(BLE_WAKE_PA12_READ == GPIO_PIN_SET) ? "BLE WAKE PA12(LOW)" : "BLE WAKE PA12(HIGH)", \
		(MDM_WAKE_PB5_READ == GPIO_PIN_SET) ? "MDM WKAE PB5(LOW)" : "MDM WKAE PB5(HIGH)");
}

// IO OUTPUT
void SYS_CHG_EN_Control(Enctl Status)
{
	// Active high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PB6_CHG_EN_GPIO_Port,PB6_CHG_EN_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PB6_CHG_EN_GPIO_Port,PB6_CHG_EN_Pin,GPIO_PIN_RESET);
}

void BLE_PWR_EN_Control(Enctl Status)
{
	// Poweron-high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PA0_BLE_EN_GPIO_Port,PA0_BLE_EN_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PA0_BLE_EN_GPIO_Port,PA0_BLE_EN_Pin,GPIO_PIN_RESET);
}

void BLE_WAKEUP_Control(WakeCtrl Status)
{
	// WAKE-high
	if(Status == WAKE)
		HAL_GPIO_WritePin(PA1_BLE_WAKE_GPIO_Port,PA1_BLE_WAKE_Pin,GPIO_PIN_SET);
	else if(Status == SLEEP)
		HAL_GPIO_WritePin(PA1_BLE_WAKE_GPIO_Port,PA1_BLE_WAKE_Pin,GPIO_PIN_RESET);
}

void MDM_PWR_EN_Control(Enctl Status)
{
	// Poweron-high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PB3_MDM_EN_GPIO_Port,PB3_MDM_EN_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PB3_MDM_EN_GPIO_Port,PB3_MDM_EN_Pin,GPIO_PIN_RESET);
}

void MDM_PWR_KEY_Control(PressCtrl Status)
{
	// ON-high
	if(Status == PRESS)
		HAL_GPIO_WritePin(PB8_MDM_PWK_GPIO_Port,PB8_MDM_PWK_Pin,GPIO_PIN_SET);
	else if(Status == RELEASE)
		HAL_GPIO_WritePin(PB8_MDM_PWK_GPIO_Port,PB8_MDM_PWK_Pin,GPIO_PIN_RESET);
}

void MDM_RESET_Control(RstCtrl Status)
{
	// Reset-low(DNP)
	if(Status == RESET)
		HAL_GPIO_WritePin(PA4_MDM_RST_GPIO_Port,PA4_MDM_RST_Pin,GPIO_PIN_RESET);
	else if(Status == SET)
		HAL_GPIO_WritePin(PA4_MDM_RST_GPIO_Port,PA4_MDM_RST_Pin,GPIO_PIN_SET);
}

void MDM_WAKE_Control(WakeCtrl Status)
{
	// Poweron-high
	if(Status == WAKE)
		HAL_GPIO_WritePin(PB2_MDM_WAKE_GPIO_Port,PB2_MDM_WAKE_Pin,GPIO_PIN_SET);
	else if(Status == SLEEP)
		HAL_GPIO_WritePin(PB2_MDM_WAKE_GPIO_Port,PB2_MDM_WAKE_Pin,GPIO_PIN_RESET);
}

void EXT_RELAY_Control(Enctl Status)
{
	// Enable-high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PA5_RELAY_GPIO_Port,PA5_RELAY_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PA5_RELAY_GPIO_Port,PA5_RELAY_Pin,GPIO_PIN_RESET);
}

void BATTERY_EN_Control(Enctl Status)
{
	// Enable-high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PA7_BAT_EN_GPIO_Port,PA7_BAT_EN_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PA7_BAT_EN_GPIO_Port,PA7_BAT_EN_Pin,GPIO_PIN_RESET);
}

void BUZZER_EN_Control(Enctl Status)
{
	// Poweron-high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PC6_BUZZER_GPIO_Port,PC6_BUZZER_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PC6_BUZZER_GPIO_Port,PC6_BUZZER_Pin,GPIO_PIN_RESET);
}

void DCDC_EN_Control(Enctl Status)
{
	// Poweron-high
	if(Status == ENABLE)
		HAL_GPIO_WritePin(PA15_DCDC_EN_GPIO_Port,PA15_DCDC_EN_Pin,GPIO_PIN_SET);
	else if(Status == DISABLE)
		HAL_GPIO_WritePin(PA15_DCDC_EN_GPIO_Port,PA15_DCDC_EN_Pin,GPIO_PIN_RESET);
}

void EXT_GPIO_Control(etLevel Status)
{
	// HIGH-LOW
	if(Status == HIGH)
		HAL_GPIO_WritePin(PB4_MITI_GPIO_GPIO_Port,PB4_MITI_GPIO_Pin,GPIO_PIN_SET);
	else if(Status == LOW)
		HAL_GPIO_WritePin(PB4_MITI_GPIO_GPIO_Port,PB4_MITI_GPIO_Pin,GPIO_PIN_RESET);
}

/*******************************************************************************
End Of The File
*******************************************************************************/

