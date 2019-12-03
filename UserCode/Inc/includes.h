/*******************************************************************************
* File Name          : includes.h
* Author               : Jevon
* Description        : This file provides all the includes functions.

* History:
*  09/11/2017 : version V1.00
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//#define NUCLEO_G031K8_BOARD_DEBUG 1
//#define BLUE_TOOTH_USART2_SUPPORT 1

#define MODEM_POWEROFF_IN_DEEPSLEEP 1

// Sleep mode debug
//#define STOP_MODE_CURRENT_DEBUG 1
//#define WAIT_MODE_CURRENT_DEBUG 1

/**
  * @}
  */

#ifndef __GLOBAL_INCLUDES_H
#define __GLOBAL_INCLUDES_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

// Start... ...
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef ALWAYS
#define ALWAYS 2
#endif

typedef enum{
  WAKE = 0,
  SLEEP = 1,
}WakeCtrl;

typedef enum{
  PRESS = 0,
  RELEASE = 1,
}PressCtrl;

typedef enum{
  LOW 	= 0,
  HIGH  	= 1,
}etLevel;

typedef enum{
  ACK 	= 1,
  NACK 	= 2,
  NOACK 	= 3,
}etAck;

// Error codes
typedef enum{
  NO_ERROR 			= 0x00,
  ACK_ERROR 		= 0x01,
  TIME_OUT_ERROR 	= 0x02, // timeout error
  CHECKSUM_ERROR 	= 0x04, // checksum mismatch error
  UNIT_ERROR 		= 0x08,
  PARM_ERROR 		= 0x80, // parameter out of range error
  //CHECKSUM_ERROR 	= 0x04,
}etError;

/**
  * @}
  */
//#define BIG ENDIAN   //e.g. Motorola (not tested at this time)
//#define LITTLE_ENDIAN  //e.g. PIC, 8051, NEC V850
typedef int32_t  		s32;
typedef int16_t 		s16;
typedef int8_t  		s8;

typedef uint32_t  		u32;
typedef uint16_t 		u16;
typedef uint8_t  		u8;
typedef uint8_t 		AtDHEvent;
// End... ...

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __GLOBAL_INCLUDES_H */

/**
  * @}
  */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <setjmp.h>

#include "main.h"

// User source code
#include "usr_adc.h"
#include "usr_i2c.h"
#include "usr_ble.h"
#include "usr_cmd.h"
#include "usr_timer.h"
#include "usr_usart.h"
#include "usr_ioctl.h"
#include "usr_sleep.h"
#include "usr_lis3dh.h"
#include "usr_initial.h"
#include "usr_version.h"

/*******************************************************************************
*   Copyrights (C) Asiatelco Technologies Co., 2003-2017. All rights reserved
*                                 End Of The File
*******************************************************************************/

