/*******************************************************************************
* File Name          : usr_adc.h
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_adc V1.00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADC_DETECT_H
#define _ADC_DETECT_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#define ADC_BUFFER_SIZE 	(2)

// Variable Declared


//Function Declare
extern void ShowAdcConvertValue(u8 PrintEnable);

#endif  /* _ADC_DETECT_H */

/*******************************************************************************
End Of The File
*******************************************************************************/

