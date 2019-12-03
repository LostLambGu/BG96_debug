/*******************************************************************************
* File Name          : usr_timer.h
* Author               : Jevon
* Description        : This file provides all the usrtimer functions.

* History:
*  04/29/2019 : usr_timer V1.00
*******************************************************************************/
#ifndef __USR_TIMER_H__
#define __USR_TIMER_H__

#include "stm32g0xx_hal.h"

#define TimeMsec(NumMsec) ((NumMsec) *1000)

// define softtimer structure.
typedef struct	_TIMER
{
	__IO u32	TimeOutVal; 		//time out value
	u8 	TimeId;				// time ID
	u32	RecTickVal;			//softtimer setting value
	u8	IsTimeOverflow;		//time out flag
	u8 	TimerStartCounter;
	void (*Routine)(u8);
} TIMER;

// Variable Declared 
extern TIMER CheckSysStatusTimer;
extern TIMER UART1ReceiveTimer;
extern TIMER SleepModeDbgTimer;

//Function Declare
// InitAll Software Timer
extern void InitSoftwareTimers(void);
// Software Timer Counter
extern void SoftwareTimerCounter(void);
// Software Check Timer Status
extern void SoftwareCheckTimerStatus(void);
// Software Timer Start
extern void SoftwareTimerStart(TIMER *timer);
// Software Timer Stop
extern void SoftwareTimerStop(TIMER *timer);
// Software Timer Reset
extern void SoftwareTimerReset(TIMER *timer,void (*Routine)(u8),u32 timeout);
// Software Timer Create
extern void SoftwareTimerCreate(TIMER *timer,u8 TimeId,void (*Routine)(u8),u32 timeout);
// Check whether time out.
extern u8 IsSoftwareTimeOut(TIMER *timer);

#endif  /* __USR_TIMER_H__ */

/*******************************************************************************
End Of The File
*******************************************************************************/

