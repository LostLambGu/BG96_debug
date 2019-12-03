/*******************************************************************************
* File Name          : usr_usart.c
* Author               : Jevon
* Description        : This file provides all the uart_driver functions.

* History:
*  04/29/2019 : usr_usart V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

extern UART_HandleTypeDef huart1;
#ifdef BLUE_TOOTH_USART2_SUPPORT
extern UART_HandleTypeDef huart2;
#endif

__IO u16 Uart1RxCount;
__IO u8 UART1_RX_NUM;
u8 Uart1RxBuffer[UART1_RX_BUFFER_SIZE];
LogCtlPrarm gLogInfoCtl = {TRUE, FALSE};

void SetGlobalTraceLogFlag(u8 Status)
{
	gLogInfoCtl.GlobalInfoEn = Status;
}

u8 GetGlobalTraceLogFlag(void)
{
	return gLogInfoCtl.GlobalInfoEn;
}

void SetCollectMoreLogFlag(u8 Status)
{
	gLogInfoCtl.MoreInfoEn = Status;
}

u8 GetCollectMoreLogFlag(void)
{
	return gLogInfoCtl.MoreInfoEn;
}

void UartBufferInit(void)
{
	// Bebug
	Uart1RxCount = 0;
	UART1_RX_NUM = FALSE;
	memset((char *)Uart1RxBuffer,0,sizeof(Uart1RxBuffer));
}

void UART1ReceiveTimerCallback(u8 Status)
{
	if(Uart1RxCount > 0)
	{
		//HAL_Delay(1);
		UART1_RX_NUM = TRUE;
		//HAL_Delay(1);
	}
}

void PutStrToUart1(u8 *string, u16 slen)
{
	UART_Transmit_Data(&huart1, string, slen,UART_SEND_DATA_TIMEOUT);
}

int Sprintf(u8 type, char *fmt, ...)
{
	int cnt;
	u8 string[MAX_PRINTF_STR_SIZE + 2] = {'\0'};
	va_list ap;
	
	// Check trace output status
	if(type == FALSE)
		return -1;
	
	va_start(ap,fmt);
	//cnt = vsprintf(string, fmt, ap);
	cnt = vsnprintf((char*)string,MAX_PRINTF_STR_SIZE ,fmt, ap);
	if(cnt > MAX_PRINTF_STR_SIZE)
		cnt = MAX_PRINTF_STR_SIZE;
	if(cnt > 0)
	{
		// Print out
		if(type != URT_ATCMD && type != URT_NRCMD)
		{
			va_end(ap);
			return -2;
		}
		// Output log information
		PutStrToUart1(string,cnt);
	}
	va_end(ap);
	
  	return (cnt);
}

void DumpMem(char *Disp, unsigned char *pBuff, int size, u8 PrintFlag)
{
	#define DUMP_LINE_MAX 8
	int i, j;
	int line = 0;
	char pucMessage[MAX_PRINTF_STR_SIZE];
	int iCnt;
	int iPosn = 0;
	int iMax = sizeof(pucMessage);
	
	memset(pucMessage,0,sizeof(pucMessage));
	Sprintf(PrintFlag,"\r\n[DUMP] %s: [%d]\r\n", Disp, size);

	for (i = 0; i < size;)
	{
		iPosn = 0;
		for (j = 0; ((i + j) < size) && (j < DUMP_LINE_MAX); j++) 
		{
			if (((i + j) % DUMP_LINE_MAX) == 0) 
			{
				iCnt = snprintf(&pucMessage[iPosn], (iMax - iPosn), "\r\n");
				iPosn += iCnt;
			}
			iCnt = snprintf(&pucMessage[iPosn], (iMax - iPosn), "%02x ", pBuff[i + j]);
			iPosn += iCnt;
		}
		for (; j < DUMP_LINE_MAX; j++) 
		{
			iCnt = snprintf(&pucMessage[iPosn], (iMax - iPosn), "   ");
			iPosn += iCnt;
		}
		iCnt = snprintf(&pucMessage[iPosn], (iMax - iPosn), "  ");
		iPosn += iCnt;
		for (j = 0; ((i + j) < size) && (j < DUMP_LINE_MAX); j++) 
		{
			iCnt = snprintf(&pucMessage[iPosn], (iMax - iPosn), "%c ", isprint((int) pBuff[i + j]) ? pBuff[i + j] : '.');
			iPosn += iCnt;
		}

		Sprintf(PrintFlag,pucMessage);

		i += DUMP_LINE_MAX;
		line++;
	}
	Sprintf(PrintFlag,"\r\n");
}
/*******************************************************************************
End Of The File
*******************************************************************************/

