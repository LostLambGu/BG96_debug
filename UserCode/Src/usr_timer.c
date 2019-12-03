/*******************************************************************************
* File Name          : usr_timer.c
* Author               : Jevon
* Description        : This file provides all the usrtimer functions.

* History:
*  04/29/2019 : usr_timer V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIMER CheckSysStatusTimer;
TIMER UART1ReceiveTimer;
TIMER SleepModeDbgTimer;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// Init All Software Timer
void InitSoftwareTimers(void)
{
	// Check System Status
	SoftwareTimerCreate(&CheckSysStatusTimer,1,CheckSysStatusTimerCallback,CHECK_SYS_STAT_TIMEOUT);
	SoftwareTimerStart(&CheckSysStatusTimer);

	// UART1 Rev DBG
	SoftwareTimerCreate(&UART1ReceiveTimer,1,UART1ReceiveTimerCallback,UART1_RECEIVE_TIMEOUT);
	//SoftwareTimerStart(&UART1ReceiveTimer);

	// Deep Sleep DBG
	SoftwareTimerCreate(&SleepModeDbgTimer,1,SleepModeDbgTimerCallback,SLEEP_MODE_DBG_TIMEOUT);
	//SoftwareTimerStart(&SleepModeDbgTimer);
}

// Software Timer Counter
void SoftwareTimerCounter(void)
{	
	// Check System Status
	if(CheckSysStatusTimer.TimerStartCounter == TRUE)
		CheckSysStatusTimer.TimeOutVal++;
	
	// UART1 Rev DBG
	if(UART1ReceiveTimer.TimerStartCounter == TRUE)
		UART1ReceiveTimer.TimeOutVal++;

	// Deep Sleep DBG
	if(SleepModeDbgTimer.TimerStartCounter == TRUE)
		SleepModeDbgTimer.TimeOutVal++;
}

// Software Check Timer Status
void SoftwareCheckTimerStatus(void)
{
	// Check System Status
	if (IsSoftwareTimeOut(&CheckSysStatusTimer) == TRUE){}
	
	// UART1 Rev DBG
	if (IsSoftwareTimeOut(&UART1ReceiveTimer) == TRUE){}

	// Deep Sleep DBG
	if (IsSoftwareTimeOut(&SleepModeDbgTimer) == TRUE){}
}

// Software Timer Start
void SoftwareTimerStart(TIMER *timer)
{		
	timer->TimerStartCounter  = TRUE;
}

// Software Timer Stop
void SoftwareTimerStop(TIMER *timer)
{		
	timer->TimerStartCounter  = FALSE;
}

// Software Timer Reset
void SoftwareTimerReset(TIMER *timer,void (*Routine)(u8),u32 timeout)
{
	timer->TimerStartCounter  = FALSE;	//timer stop
	timer->RecTickVal = timeout;			//softtimer setting value
	timer->TimeOutVal = 0;				//time out value
	timer->IsTimeOverflow  = FALSE;	 	//time out flag
	timer->Routine = Routine;
}

// Software Timer Create
void SoftwareTimerCreate(TIMER *timer,u8 TimeId,void (*Routine)(u8),u32 timeout)
{
	timer->TimeId = TimeId;
	timer->RecTickVal = timeout;			//softtimer setting value
	timer->TimeOutVal = 0;				//time out value
	timer->IsTimeOverflow  = FALSE;		//time out flag
	timer->TimerStartCounter  = FALSE;	//timer stop
	timer->Routine  = Routine;
}

// Check whether time out.
u8 IsSoftwareTimeOut(TIMER *timer)
{		
	//ET0 = 0;
	if(!timer->IsTimeOverflow)
	{
		//After gSysTick and timer->TimeOutVal overflow, 
		//the software timer function can still work well
		//the next statement is equivalent to:
		//(gSysTick - timer->TimeOutVal) < 0x80000000 
		if(timer->TimeOutVal > timer->RecTickVal)
		{
			timer->IsTimeOverflow = TRUE;
			SoftwareTimerStop(timer);
			timer->Routine(timer->TimeId);
		}
	}	
	//ET0 = 1;

	return timer->IsTimeOverflow;				
}

/*******************************************************************************
End Of The File
*******************************************************************************/

