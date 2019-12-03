/*******************************************************************************
* File Name          : usr_sleep.h
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_sleep V1.00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DEVICE_DEEP_SLEEP_H
#define _DEVICE_DEEP_SLEEP_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#define SLEEP_MODE_DBG_TIMEOUT 	(5*1000)

typedef enum{
  STOPMODE = 0,
  WAITMODE = 1,
}SleepCtrl;

// Variable Declared


//Function Declare
extern void SavingPowerConsumption(void);
extern void RecoverNormalOperation(void);
extern void SystemEnterSleepMode(void);
extern void SystemEnterStopMode(void);
extern void SystemGoIntoDeepsleepMode(SleepCtrl SleepMode);
extern void SleepModeDbgTimerCallback(u8 Status);
#endif  /* _DEVICE_DEEP_SLEEP_H */

/*******************************************************************************
End Of The File
*******************************************************************************/

