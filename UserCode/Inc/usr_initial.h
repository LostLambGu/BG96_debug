/*******************************************************************************
* File Name          : usr_initial.h
* Author               : Jevon
* Description        : This file provides all the usrtimer functions.

* History:
*  04/11/2019 : usr_initial V1.00
*******************************************************************************/
#ifndef __USR_INITIAL_H__
#define __USR_INITIAL_H__

#include "stm32g0xx_hal.h"

#define CHECK_SYS_STAT_TIMEOUT 	(500/*1000*/)


// Variable Declared 


//Function Declare
extern void DelayUsTime(uint16_t time);
void TickHardwareWatchdog(u8 PrintoutEn);
extern void SystemInitialization(void);
extern void CheckSysStatusTimerCallback(u8 Status);

#endif  /* __USR_INITIAL_H__ */

/*******************************************************************************
End Of The File
*******************************************************************************/

