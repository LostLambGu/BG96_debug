/*******************************************************************************
* File Name          : usr_version.c
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_version V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAIN_VERSION_NUM			1
#define SUB_VERSION_NUM				6
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void ShowSoftVersion(void)
{
	Sprintf(URT_NRCMD,"\r\n+SYSTEM UP\r\n");
	Sprintf(URT_ATCMD,"\r\nVERSION:V%d.%d(%s %s)\r\n", \
		MAIN_VERSION_NUM, \
		SUB_VERSION_NUM, \
		__DATE__, \
		__TIME__);
}

void GetSoftVersion(void)
{
	Sprintf(URT_ATCMD,"\r\n*VERSION:V%d.%d(%s %s)", \
		MAIN_VERSION_NUM, \
		SUB_VERSION_NUM, \
		__DATE__, \
		__TIME__);
}


/*******************************************************************************
End Of The File
*******************************************************************************/

