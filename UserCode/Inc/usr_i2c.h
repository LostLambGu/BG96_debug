/*******************************************************************************
* File Name          : usr_i2c.h
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  05/06/2019 : usr_i2c V1.00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _APPLICATION_I2C_H
#define _APPLICATION_I2C_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#define BLE_BUF_SIZE 	(64)
#define BLE_DEV_ADDR 	(0x5C&0xFE)
#define BLE_I2C_TIMEOUT (20)

// Variable Declared


//Function Declare
extern void SetBleIntIrqStatus(u8 Status);
extern u8 GetBleIntIrqStatus(void);
extern void I2cDataReading(void);
extern void I2cDataSending(u8* pData, u8 len);
extern void I2cIntIrqProcess(void);

#endif  /* _APPLICATION_I2C_H */

/*******************************************************************************
End Of The File
*******************************************************************************/


