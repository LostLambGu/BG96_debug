/*******************************************************************************
* File Name          : usr_cmd.c
* Author               : Jevon
* Description        : This file provides all the usrtimer functions.

* History:
*  04/30/2019 : usr_cmd V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 IsTimer3TimeroutFlag = FALSE;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;

static char* params[MAX_PARAMS];

static const ParamCompareTableT ATprarmType[CMD_PRARM_TYPE_INVALID]=
{
	{CMD_PRARM_TYPE_QUERYALL,		"=?"},
	{CMD_PRARM_TYPE_EVALUATE,		"="	},
	{CMD_PRARM_TYPE_QUERY,			"?"	},
	{CMD_PRARM_TYPE_EXECUTE, 		""	},
};

static const ATCompareTableT ATCmdTypeTable[AT_CMD_DEF_LAST] =
{
	{ "##LIST",			AT_CMD_DEF_CMDLIST,			TRUE },
	{ "##RESET",		AT_CMD_DEF_RESET,			TRUE },
	{ "##ADC",			AT_CMD_DEF_ADC,				TRUE },
	{ "##TIM",			AT_CMD_DEF_TIM, 			TRUE },
	{ "##BLE", 			AT_CMD_DEF_BLE, 			TRUE },
	{ "##IO",			AT_CMD_DEF_IOCTL,			TRUE },
	{ "##LOG",			AT_CMD_DEF_LOGINFO,			TRUE },
	{ "##MEMS",			AT_CMD_DEF_MEMS,			TRUE },
	{ "##SLEEP",		AT_CMD_DEF_SLEEP,			TRUE },
	{ "##STAT",			AT_CMD_DEF_STATUS,			TRUE },
	{ "##VERSION",		AT_CMD_DEF_VERSION,			TRUE },
};

static void Edit(char* str)
{
	int i = 0;
	int len = strlen(str);
	do
	{
		if (str[i] == '\b') // backspace
		{
			// Move everything from the next character to the
			// character that the bs is trying to delete
			memcpy(&str[i - 1], &str[i + 1], strlen(&str[i + 1]) + 1);
			len = strlen(str); // Reset Length
			i -= 2; // back up 2 one for deleted character and one for \b
		}
		i++;
	} while (i < len);
}

static char *Trim(char *str)
{
	char *end;

	while (isspace(*str))
		str++;

	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;

	*(++end) = '\0';

	return str;
}

static int Split(char *str, char delim, char *tokv[], int maxtoks)
{
	int tokc = 0;
	char *loc;

	while (tokc < maxtoks && (loc = strchr(str, delim)) != NULL)
	{
		*loc = '\0';
		*tokv++ = Trim(str);
		tokc++;
		str = loc + 1;
	}

	if (tokc < maxtoks)
	{
		*tokv++ = Trim(str);
		tokc++;
	}

	return tokc;
}

static void ToUpper(char* str)
{
	int i;
	for (i = 0; i < strlen(str); i++)
		str[i] = (char) toupper((int)str[i]);
}

static int strnicmp(const char *dst,const char *src,int count)
{
   int ch1, ch2;
   do
   {
      if (((ch1 = (unsigned char)(*(dst++))) >= 'A') &&(ch1 <= 'Z'))
        ch1 += 0x20;
      if (((ch2 = (unsigned char)(*(src++))) >= 'A') &&(ch2 <= 'Z'))
        ch2 += 0x20;
   } while ( --count && ch1 && (ch1 == ch2) );
   return (ch1 - ch2);
}

static u8 ATCmdGetType( ATCompareTableT *checkTable, u8 tableSize, char* dataBuf, u8 *idxSearch )
{
	u8				idx;
	u8				cmdLen;
	ATCompareTableT	*pTable;

	if( dataBuf == NULL || idxSearch == NULL || checkTable == NULL )
		return AT_CMD_DEF_NULL;

	for ( idx = 0/*1*/; idx < tableSize ; idx++)
	{
		pTable = &checkTable[idx];
		cmdLen = strlen( pTable->cmdStr);
	
		if(pTable->isMatchAll)
		{
			if( strncmp( (char *)dataBuf, pTable->cmdStr, cmdLen) == 0 )
			{
				*idxSearch = cmdLen;
				return pTable->cmdType;
			}
		}
		else
		{
			if( strnicmp ( (char *)dataBuf, pTable->cmdStr, cmdLen) == 0 )
			{
				*idxSearch = cmdLen;
				return pTable->cmdType;
			}
		}
	}
	return AT_CMD_DEF_NULL;
}

static u8 ATParamGetType( ParamCompareTableT *checkTable, u8 tableSize, char* dataBuf, u8 *idxSearch )
{
	u8					idx;
	u8					cmdLen;
	ParamCompareTableT	*pTable;

	if( dataBuf == NULL || idxSearch == NULL || checkTable == NULL )
		return CMD_PRARM_TYPE_INVALID;

	for ( idx = 0; idx < tableSize ; idx++)
	{
		pTable = &checkTable[idx];
		cmdLen = strlen( pTable->strCmdString);
	
		if( strncmp( (char *)dataBuf, pTable->strCmdString, cmdLen) == 0 )
		{
			*idxSearch = cmdLen;
			return pTable->nIndex;
		}
	}
	return CMD_PRARM_TYPE_INVALID;
}

static u8 ATGetCmdType( char* dataBuf, u8 *idxSearch, u8 datalen)
{
	return ATCmdGetType( (ATCompareTableT *)ATCmdTypeTable, (sizeof(ATCmdTypeTable)/sizeof(ATCmdTypeTable[0])), dataBuf, idxSearch );
}

static u8 ATGetPrarmType( char* dataBuf, u8 *idxSearch)
{
	return ATParamGetType( (ParamCompareTableT *)ATprarmType, (sizeof(ATprarmType)/sizeof(ATprarmType[0])), dataBuf, idxSearch );
}

/*****************************************************************************
******************************* AT COMMAND START ******************************
*****************************************************************************/
static void ATCmdDefCmdList(void)
{
	u8 Count = 0;
	// Print out
	Sprintf(URT_ATCMD,"\r\n==SUPPORT AT AS FOLLOW==");
	for(Count=0;Count<(sizeof(ATCmdTypeTable)/sizeof(ATCmdTypeTable[0]));Count++)
	{
		// Print out
		Sprintf(URT_ATCMD,"\r\n    %s",ATCmdTypeTable[Count].cmdStr);
	}
	// Print out
	Sprintf(URT_ATCMD,"\r\n==END OF THE AT LIST==");
}

static void ATCmdDefReset(void)
{
	// Print Out
	Sprintf(URT_ATCMD,"\r\n*RST:Done");
	Sprintf(URT_ATCMD,"\r\nOK\r\n");
	// Delay
	HAL_Delay(20);
	// Jumping
	NVIC_SystemReset();
}

static void ATCmdDefAdc(u8 paramtype, int paramCnt, char* params[])
{
	ShowAdcConvertValue(URT_ATCMD);
	// Print Out
	Sprintf(URT_ATCMD,"\r\nOK\r\n");
}

static void ATCmdDefTim(u8 paramtype, int paramCnt, char* params[])
{
	//HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);
	// Print Out
	Sprintf(URT_ATCMD,"\r\n*TIM:Start...\r\n");
	Sprintf(URT_ATCMD,"\r\nOK\r\n");
}

static void ATCmdDefBlueTooth(u8 paramtype, int paramCnt, char* params[])
{
	if(paramtype == CMD_PRARM_TYPE_EVALUATE)
	{
		if (paramCnt >= 1 && strcmp(params[0], "I2C") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "RD") == 0)
			{
				I2cDataReading();
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "WR") == 0)
			{
				u8 idx;
				u8 Data[BLE_BUF_SIZE];

				for(idx=0; idx<BLE_BUF_SIZE; idx++)
				{
					Data[idx] = idx;
					//Data[idx] = 0;
				}
				
				I2cDataSending(Data, BLE_BUF_SIZE);
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}			
		}
		else if (paramCnt >= 1 && strcmp(params[0], "CMD") == 0)
		{
			if (paramCnt >= 3 && strcmp(params[1], "ADVTIME") == 0)
			{
				u16 interval = atoi(params[2]);
				
				//e.g. 0x0050 interval=80*0.625ms=50ms
				cmd_set_advertising_interval(interval);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: adv interval set(%d)", interval);
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ADVDATA") == 0)
			{
				u8 pData[]= {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
			
				cmd_set_advertising_data(pData, strlen((char*)pData));
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: adv data send");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ADVENABLE") == 0)
			{
				cmd_enable_advertising(TRUE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: adv enable");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ADVDISABLE") == 0)
			{
				cmd_enable_advertising(FALSE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: adv disable");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "BUFFGET") == 0)
			{
				cmd_get_module_available_data_buffer();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: get module available data buffer");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 3 && strcmp(params[1], "ADVTYPE") == 0)
			{
				u8 type = atoi(params[2]);

				//0x00:connectable undirected adv
				//0x01:connectable directed adv
				//0x02:scannable undirected adv
				//0x03:non-connectable adv
				cmd_set_advertising_type(type);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: set advertising type(%d)", type);
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 3 && strcmp(params[1], "NAMESET") == 0)
			{				
				cmd_set_device_name((u8*)params[2], strlen(params[2]));
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: set_device_name");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "CONNPARAM") == 0)
			{
				cmd_get_connection_parameters();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: get connection parameters");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "CSTATE") == 0)
			{
				cmd_get_current_work_state();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: get current work state");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "DISCONNECT") == 0)
			{
				cmd_terminate_connection();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: terminate connection");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "RESTART") == 0)
			{
				cmd_restart_module();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: restart module");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 3 && strcmp(params[1], "SENDING") == 0)
			{
				Ble_Data_Sending((u8*)params[2],strlen(params[2]));
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: data sending");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "VERSION") == 0)
			{
				cmd_get_version();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: version number");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "MACADDR") == 0)
			{
				cmd_get_mac_addr();
				// Print out
				Sprintf(URT_ATCMD,"\r\n*BLE: mac adress");
				// Print out
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
	}
	// Print out
	Sprintf(URT_ATCMD, "\r\nERROR\r\n");
	Sprintf(URT_ATCMD, "\r\nNOTE:Please input \"I2C,RD/WR\" or \"CMD,ADVENABLE/ADVDISABLE...\"\r\n");
}

static void ATCmdDefIOCtl(u8 paramtype, int paramCnt, char* params[])
{
	if(paramtype == CMD_PRARM_TYPE_QUERY)
	{
		// Input IO Status
		Show_IO_IN_Status();
		// Print out
		Sprintf(URT_ATCMD,"\r\nOK\r\n");
		return;
	}
	else if(paramtype == CMD_PRARM_TYPE_EVALUATE)
	{
		if (paramCnt >= 1 && strcmp(params[0], "CHG") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				SYS_CHG_EN_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB6 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:charger disable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				SYS_CHG_EN_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB6 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:charger enable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "BT") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				BLE_PWR_EN_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA0 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:ble power off");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				BLE_PWR_EN_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA0 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:ble power on");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "WAKE") == 0)
			{
				BLE_WAKEUP_Control(WAKE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA1 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:ble wakeup");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "SLEEP") == 0)
			{
				BLE_WAKEUP_Control(SLEEP);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA1 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:ble deepsleep");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "MDN") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				MDM_PWR_EN_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB3 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem power off");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				MDM_PWR_EN_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB3 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem power on");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "PRESS") == 0)
			{
				MDM_PWR_KEY_Control(PRESS);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB8 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem power key press down");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "RELEASE") == 0)
			{
				MDM_PWR_KEY_Control(RELEASE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB8 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem power key release up");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "RESET") == 0)
			{
				MDM_RESET_Control(RESET);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA4 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem hardware reset");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "SET") == 0)
			{
				MDM_RESET_Control(SET);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA4 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem hardware set");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "WAKE") == 0)
			{
				MDM_WAKE_Control(WAKE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB2 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem wakeup");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "SLEEP") == 0)
			{
				MDM_WAKE_Control(SLEEP);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB2 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:modem deepsleep");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "RELAY") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				EXT_RELAY_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA5 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:extend relay disable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				EXT_RELAY_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA5 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:extend relay enable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "BAT") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				BATTERY_EN_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA7 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:battery disable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				BATTERY_EN_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA7 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:battery enable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "BUZZER") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				BUZZER_EN_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PC6 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:buzzer disable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				BUZZER_EN_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PC6 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:buzzer enable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "DCDC") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				DCDC_EN_Control(DISABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA15 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:dc-dc disable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				DCDC_EN_Control(ENABLE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PA15 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:dc-dc enable");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
		else if (paramCnt >= 1 && strcmp(params[0], "EXTIO") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "HIGH") == 0)
			{
				EXT_GPIO_Control(HIGH);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB4 high level");
				Sprintf(URT_ATCMD,"\r\n*IO:extend gpio high level");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "LOW") == 0)
			{
				EXT_GPIO_Control(LOW);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*IO:PB4 low level");
				Sprintf(URT_ATCMD,"\r\n*IO:extend gpio low level");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
	}
	// Print out
	Sprintf(URT_ATCMD, "\r\nERROR\r\n");
	Sprintf(URT_ATCMD, "\r\nNOTE:Please input \"?\" or \"CHG,OFF/ON\" or \"BT,OFF/ON/WAKE/SLEEP\"\r\n");
}

static void ATCmdLogInfo(u8 paramtype, int paramCnt, char* params[])
{
	if(paramtype == CMD_PRARM_TYPE_QUERY)
	{
		// Print out
		Sprintf(URT_ATCMD,"\r\n*log:NORMAL(%s) MORE(%s) flag(%d %d)", \
			GetGlobalTraceLogFlag()?"ENABLE":"DISABLE", \
			GetCollectMoreLogFlag()?"ENABLE":"DISABLE", \
			URT_ATCMD, URT_NRCMD);
		// Print out
		Sprintf(URT_ATCMD,"\r\nOK\r\n");
		return;
	}
	else if(paramtype == CMD_PRARM_TYPE_EVALUATE)
	{
		if (paramCnt >= 1 && strcmp(params[0], "INFO") == 0)
		{
			if (paramCnt >= 2 && strcmp(params[1], "OFF") == 0)
			{
				SetGlobalTraceLogFlag(FALSE);
				SetCollectMoreLogFlag(FALSE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*log:ALL TRACE OFF");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "ON") == 0)
			{
				SetGlobalTraceLogFlag(TRUE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*log:NORMAL TRACE ON");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
			else if (paramCnt >= 2 && strcmp(params[1], "MORE") == 0)
			{
				SetCollectMoreLogFlag(TRUE);
				// Print out
				Sprintf(URT_ATCMD,"\r\n*log:MORE LOG ON");
				Sprintf(URT_ATCMD,"\r\nOK\r\n");
				return;
			}
		}
	}
	Sprintf(URT_ATCMD, "\r\nERROR\r\n");
	Sprintf(URT_ATCMD, "\r\nNOTE:Please input \"? \" or \"INFO,ON/OFF/MORE\"\r\n");
}

static void ATCmdDefAccelerometer(u8 paramtype, int paramCnt, char* params[])
{
	if(paramtype == CMD_PRARM_TYPE_QUERY)
	{
		u8 ChipID = 0;
		
		LIS3DH_GetWHO_AM_I(&ChipID);
		// Check Accelerometer
		if(ChipID == WHOAMI_LIS3DH_ACC)
		{
			Sprintf(URT_ATCMD,"\r\n*MEMS:Accelerometer PASS,0x%X",ChipID);
		}
		else
		{
			Sprintf(URT_ATCMD,"\r\n*MEMS:Accelerometer FAIL,0x%X",ChipID);
		}
		// Print out
		Sprintf(URT_ATCMD,"\r\nOK\r\n");
		return;
	}
	else if(paramtype == CMD_PRARM_TYPE_EVALUATE)
	{
		if (paramCnt >= 1 && strcmp(params[0], "INIT") == 0)
		{
			Lis3dhInitialize();
			// Print out
			Sprintf(URT_ATCMD,"\r\n*MEMS:Initialization");
			Sprintf(URT_ATCMD,"\r\nOK\r\n");
			return;
		}
		else if (paramCnt >= 1 && strcmp(params[0], "AXIS") == 0)
		{
			AxesRaw_t data;
			
			LIS3DH_GetAccAxesRaw(&data);
			// Print out
			Sprintf(URT_ATCMD, "\r\n*MEMS:X=%6d Y=%6d Z=%6d ", \
				data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
			Sprintf(URT_ATCMD,"\r\nOK\r\n");
			return;
		}
		else if (paramCnt >= 1 && strcmp(params[0], "DISABLE") == 0)
		{
			LIS3DH_SetMode(LIS3DH_POWER_DOWN);
			// Print out
			Sprintf(URT_ATCMD, "\r\n*MEMS:Disable");
			Sprintf(URT_ATCMD,"\r\nOK\r\n");
			return;
		}
	}
	// Print out
	Sprintf(URT_ATCMD, "\r\nERROR\r\n");
	Sprintf(URT_ATCMD, "\r\nNOTE:Please input \"? \" or \"INIT/AXIS/DISABLE\"\r\n");
}

static void ATCmdDefSleep(void)
{
	// Print Out
	Sprintf(URT_ATCMD,"\r\n*sleep:done!");
	Sprintf(URT_ATCMD,"\r\nOK\r\n");

	// Delay
	HAL_Delay(20);

	// Deep sleep
	SystemGoIntoDeepsleepMode(STOPMODE);
}

static void ATCmdGetStatus(void)
{
	// Input IO Status
	Show_IO_IN_Status();
	// End of status
	Sprintf(URT_ATCMD,"\r\nOK\r\n");
}

static void ATCmdVerionInfo(void)
{
	// MCU version
	GetSoftVersion();	
	// Print out
	Sprintf(URT_ATCMD,"\r\nOK\r\n");
}

static void ATCmdDefDefault(void)
{
	Sprintf(URT_ATCMD,"\r\nERROR\r\n");
}

static void ATCmdProcessing(u8 matchCmd, u8 paramtype, int paramCnt, char* params[])
{
	switch(matchCmd)
	{
		case AT_CMD_DEF_CMDLIST:
			ATCmdDefCmdList();
			break;

		case AT_CMD_DEF_RESET:
			ATCmdDefReset();
			break;

		case AT_CMD_DEF_ADC:
			ATCmdDefAdc(paramtype, paramCnt, params);
			break;

		case AT_CMD_DEF_TIM:
			ATCmdDefTim(paramtype, paramCnt, params);
			break;

		case AT_CMD_DEF_BLE:
			ATCmdDefBlueTooth(paramtype, paramCnt, params);
			break;

		case AT_CMD_DEF_IOCTL:
			ATCmdDefIOCtl(paramtype, paramCnt, params);
			break;

		case AT_CMD_DEF_LOGINFO:
			ATCmdLogInfo(paramtype, paramCnt, params);
			break;

		case AT_CMD_DEF_MEMS:
			ATCmdDefAccelerometer(paramtype, paramCnt, params);
			break;

		case AT_CMD_DEF_SLEEP:
			ATCmdDefSleep();
			break;

		case AT_CMD_DEF_STATUS:
			ATCmdGetStatus();
			break;
			
		case AT_CMD_DEF_VERSION:
			ATCmdVerionInfo();
			break;

		default:
			ATCmdDefDefault();
			break;
	}
}

static void ProcessCommand(char* data, u8 datalen)
{
	int paramCnt;
	u8 idxSearch = 0;
	u8 matchCmd = AT_CMD_DEF_NULL;

	Edit(data); // Allow for backspace
	ToUpper(data);
	
	// Get AT Command Type
	matchCmd = ATGetCmdType(data, &idxSearch, datalen);
	// Print out
	if(GetCollectMoreLogFlag() == TRUE)
		Sprintf(URT_ATCMD, "\r\n...%d...%d...%s...",matchCmd,datalen,data);
	// Check command
	if(matchCmd != AT_CMD_DEF_NULL)
	{
		u8 paramSearch = 0;
		u8 paramtype = CMD_PRARM_TYPE_INVALID;

		data += idxSearch;
		paramtype = ATGetPrarmType(data, &paramSearch);
		data += paramSearch;

		if (paramtype != CMD_PRARM_TYPE_INVALID)
		{
			paramCnt = Split(data, ',', params, MAX_PARAMS);
			// Check param
			if (paramCnt < MAX_PARAMS)
			{
				// Print out
				if(GetCollectMoreLogFlag() == TRUE)
					Sprintf(URT_ATCMD, "\r\n....%d....%d....%d....",matchCmd,paramtype,paramCnt);
				// Check Cmd
				ATCmdProcessing(matchCmd, paramtype, paramCnt, params);
				return;
			}
		}
	}
	Sprintf(URT_ATCMD, "\r\nERROR\r\n");
}

void CommandDetection(void)
{
	if(UART1_RX_NUM == TRUE)
	{
		u16 DataLen;
		u8 UartData[UART1_RX_BUFFER_SIZE];

		// Clear flag
		UART1_RX_NUM = FALSE;
		
		// Copy Data
		DataLen = Uart1RxCount;
		memset((char *)UartData,0,sizeof(UartData));
		memcpy((char *)UartData, (char *)Uart1RxBuffer, DataLen);
		UartData[DataLen] = '\0';

		// Clear Buffer
		Uart1RxCount = 0;
		memset((char *)Uart1RxBuffer,0,sizeof(Uart1RxBuffer));

		// Print out
		//Sprintf(URT_ATCMD,"\r\n[CMD] RX:%d %s",DataLen,UartData);
		
		// Check length
		if((DataLen - 2) > 0 && DataLen <= UART1_RX_BUFFER_SIZE)
		{
			// remove 0d 0a
			if(UartData[DataLen-2] == 0x0D && \
				UartData[DataLen-1] == 0x0A)
			{
				UartData[DataLen -1] = '\0';
				UartData[DataLen -2] = '\0';
				DataLen -= 2;
			}
			//Sprintf(URT_ATCMD,"\r\n[CMD] content:%d %s",DataLen,UartData);
			// Parse AT Cmd(AT*)
			ProcessCommand((char*)UartData, DataLen);
		}
		else
		{
			Sprintf(URT_ATCMD, "\r\n[CMD] NO CMD ERR %d", DataLen);
		}
	}
}

/*******************************************************************************
End Of The File
*******************************************************************************/

