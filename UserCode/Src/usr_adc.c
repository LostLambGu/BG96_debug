/*******************************************************************************
* File Name          : usr_adc.c
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_adc V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO u16 AdcDmaBuffer[ADC_BUFFER_SIZE];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

// PB0 	CHANNEL 8 	12V
// PB0 	CHANNEL 9 	BAT

void ShowAdcConvertValue(u8 PrintEnable)
{
	u16 VdcdcVolage = 0;
	u16 VBatVolage = 0;

	HAL_StatusTypeDef OpResult;

	OpResult = HAL_ADC_Start_DMA(&hadc1, (u32 *)AdcDmaBuffer, ADC_BUFFER_SIZE);
	if (OpResult != HAL_OK)
	{
		// Print out
		Sprintf(URT_NRCMD,"\r\n*ADC:Start fail %d", OpResult);
	}
	// Check result
	//OpResult = HAL_ADC_PollForConversion(&hadc, 10);
	//OpResult = HAL_ADC_PollForEvent(&hadc, ADC_FLAG_EOC, 10);
	OpResult = HAL_ADC_PollForEvent(&hadc1, ADC_FLAG_EOS, 10);
	if (OpResult != HAL_OK)
	{
		// Print out
		Sprintf(URT_NRCMD,"\r\n*ADC:Convert fail %d", OpResult);
		//return;
	}

	/* Check if the continous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
	{
		// Print out
		Sprintf(URT_NRCMD,"\r\n*ADC:Convert successful");
	}
	
	// Delay
	//HAL_Delay(1);

/*	VmainVolage = ((AdcDmaBuffer[0] * 3300) / 4096)*((1000+100)/100);
	VBatVolage = ((AdcDmaBuffer[1] * 3300) / 4096)*((432+1000)/1000);*/

	VdcdcVolage = ((AdcDmaBuffer[0] * 3300 * 11) / 4096);
	VBatVolage = ((AdcDmaBuffer[1] * 3300 * 1.432) / 4096);

	// Print out
	Sprintf(PrintEnable,(char *)"\r\n*ADC:Value(%04d) DC-DC Volt(%dmV)", \
		AdcDmaBuffer[0],VdcdcVolage);
	// Print out
	Sprintf(PrintEnable,(char *)"\r\n*ADC:Value(%04d) BATTERY Volt(%dmV)", \
		AdcDmaBuffer[1],VBatVolage);
}

/*******************************************************************************
End Of The File
*******************************************************************************/

