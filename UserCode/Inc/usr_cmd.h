/*******************************************************************************
* File Name          : usr_cmd.h
* Author               : Jevon
* Description        : This file provides all the usrtimer functions.

* History:
*  04/11/2019 : usr_cmd V1.00
*******************************************************************************/
#ifndef _COMMAND_CONTROL_H
#define _COMMAND_CONTROL_H

#include "stm32g0xx_hal.h"

#define MAX_PARAMS 		(10)

typedef struct
{
  const char*	cmdStr;
  u8			cmdType;
  u8			isMatchAll;
}ATCompareTableT;

typedef struct
{
	int	nIndex;
	const char*	strCmdString;
}ParamCompareTableT;

typedef enum
{
	CMD_PRARM_TYPE_QUERYALL,		// "=?"
	CMD_PRARM_TYPE_EVALUATE,		// "="
	CMD_PRARM_TYPE_QUERY,			// "?"
	CMD_PRARM_TYPE_EXECUTE,			// ""

	CMD_PRARM_TYPE_INVALID
}CmdPrarmTypeT;

typedef enum
{
	AT_CMD_DEF_CMDLIST,
	AT_CMD_DEF_RESET,
	AT_CMD_DEF_ADC,
	AT_CMD_DEF_TIM,
	AT_CMD_DEF_BLE,
	AT_CMD_DEF_IOCTL,
	AT_CMD_DEF_LOGINFO,
	AT_CMD_DEF_MEMS,
	AT_CMD_DEF_SLEEP,
	AT_CMD_DEF_STATUS,
	AT_CMD_DEF_VERSION,
	AT_CMD_DEF_LAST,

	AT_CMD_DEF_NULL = 0xFF
}ATCmdTypeT;

// Variable Declared 
extern u8 IsTimer3TimeroutFlag;

//Function Declare
extern void CommandDetection(void);

#endif  /* _COMMAND_CONTROL_H */

/*******************************************************************************
End Of The File
*******************************************************************************/

