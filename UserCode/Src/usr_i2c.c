/*******************************************************************************
* File Name          : usr_i2c.h
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  05/06/2019 : usr_i2c V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u8 BleInterruptIrqFlag = FALSE;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c2;

void SetBleIntIrqStatus(u8 Status)
{
	BleInterruptIrqFlag = Status;
}

u8 GetBleIntIrqStatus(void)
{
	return BleInterruptIrqFlag;
}

static void GpioStatusMonitoring(void)
{
	// Old
	static GPIO_PinState BleIntStatusOld = GPIO_PIN_HIZ;
	// New
	static GPIO_PinState BleIntStatusNew;
	
	// Ble Wakeup MCU
	BleIntStatusNew = HAL_GPIO_ReadPin(PB9_BLE_INT_GPIO_Port,PB9_BLE_INT_Pin);
	if(BleIntStatusNew != BleIntStatusOld)
	{
		BleIntStatusOld = BleIntStatusNew;
		if(BleIntStatusNew == GPIO_PIN_SET)
			Sprintf(URT_NRCMD,"\r\n[I2C] INT BLE->MCU(HIGH)");
		else
			Sprintf(URT_NRCMD,"\r\n[I2C] INT BLE->MCU(LOW)");
	}
}

void I2cDataReading(void)
{
	HAL_StatusTypeDef Result;
	u8 Data[BLE_BUF_SIZE];

	memset(Data, 0, BLE_BUF_SIZE);
	Result = HAL_I2C_Master_Receive(&hi2c2, BLE_DEV_ADDR, Data, BLE_BUF_SIZE, BLE_I2C_TIMEOUT);

	if(GetCollectMoreLogFlag() == TRUE)
	{
		// Print out
		Sprintf(URT_ATCMD,"\r\n*BLE:RD data %s", Result?"ERR":"DONE");
		// Show hex data
		DumpMem("RD", Data, 64/*Data[1]+2*/, URT_ATCMD);
	}

	// Data process
	BleSppEventsProcess(Data, BLE_BUF_SIZE);
}

void I2cDataSending(u8* pData, u8 len)
{
	if(len <= BLE_BUF_SIZE)
	{
		HAL_StatusTypeDef Result;
		
		Result = HAL_I2C_Master_Transmit(&hi2c2, BLE_DEV_ADDR, pData, len, BLE_I2C_TIMEOUT);
		
		if(GetCollectMoreLogFlag() == TRUE)
		{
			// Print out
			Sprintf(URT_ATCMD,"\r\n*BLE:WR data %s", Result?"ERR":"DONE");
			// Show hex data
			DumpMem("WR", pData, len , URT_ATCMD);
		}
		//-----------------------------------------------------------------
		//HAL_Delay(100);
		//HAL_GPIO_TogglePin(PA1_BLE_WAKE_GPIO_Port,PA1_BLE_WAKE_Pin);
		//-----------------------------------------------------------------
	}
}

void I2cIntIrqProcess(void)
{
	if(GetBleIntIrqStatus() == TRUE)
	{
		// Reset BLE int Flag
		SetBleIntIrqStatus(FALSE);
		// Print out
		if(GetCollectMoreLogFlag() == TRUE)
			Sprintf(URT_NRCMD,"\r\n[I2C] Interupt1 Occured");
		// Read data
		I2cDataReading();
	}

	// Gpio checking
	if(GetCollectMoreLogFlag() == TRUE)
		GpioStatusMonitoring();
}


/*******************************************************************************
End Of The File
*******************************************************************************/

