/*******************************************************************************
* File Name          : usr_usart.h
* Author               : Jevon
* Description        : This file provides all the uart_driver functions.

* History:
*  04/29/2019 : usr_usart V1.00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HYPERTERMINAL_H
#define _HYPERTERMINAL_H

#include "stm32g0xx_hal.h"

typedef enum 
{
    URT_ATCMD = 2,
    URT_NRCMD = 3,

    URT_MAXIMUM
}Urt1CtlT;

typedef struct 
{
    u8 GlobalInfoEn;
    u8 MoreInfoEn;
} LogCtlPrarm;

/* Define max printf string size in chars */
#define UART_SEND_DATA_TIMEOUT 		(50) // 256Byte 22.22ms
#define UART1_RECEIVE_TIMEOUT		(10)
#define UART1_RX_BUFFER_SIZE 		(128)
#define MAX_PRINTF_STR_SIZE  		(UART1_RX_BUFFER_SIZE)

// Variable Declared 
extern __IO u16 Uart1RxCount;
extern __IO u8 UART1_RX_NUM;
extern u8 Uart1RxBuffer[UART1_RX_BUFFER_SIZE];
extern LogCtlPrarm gLogInfoCtl;

//Function Declare
extern void SetGlobalTraceLogFlag(u8 Status);
extern u8 GetGlobalTraceLogFlag(void);
extern void SetCollectMoreLogFlag(u8 Status);
extern u8 GetCollectMoreLogFlag(void);
extern void UartBufferInit(void);
extern void UART1ReceiveTimerCallback(u8 Status);
extern void PutStrToUart1(u8 *string, u16 slen);
extern int Sprintf(u8 type, char *fmt, ...);
extern void DumpMem(char *Disp, unsigned char *pBuff, int size, u8 PrintFlag);
#endif  /* _HYPERTERMINAL_H */
/*******************************************************************************
End Of The File
*******************************************************************************/

