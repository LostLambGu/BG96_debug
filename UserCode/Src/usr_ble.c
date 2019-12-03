/*******************************************************************************
* File Name          : usr_ble.c
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_ble V1.00
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//BLE SPP Command (from Host to Telink BLE module)
//Field	 		Bytes 	Description
//CmdID 		  2 		Command ID
//ParaLen		  2 		Length of parameters
//Parameters	  n 		Parameter payload
void cmd_set_advertising_interval(u16 interval)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x01;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x02;
	pCmd[3] = 0x00;
	//Parameters
	pCmd[4] = interval & 0xFF;
	pCmd[5] = interval >> 8 & 0xFF;
	
	//e.g. 0x0050 interval=80*0.625ms=50ms
	I2cDataSending(pCmd, 6);
}

void cmd_set_advertising_data(u8* pData, u8 len)
{
	u8 pCmd[22];

	if(len > 0x10)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:cmd len error(%d)", len);
		return;
	}

	//cmdID
	pCmd[0] = 0x02;
	pCmd[1] = 0xFF;
	//ParaLen <= 0x10(16)
	pCmd[2] = len;
	pCmd[3] = 0x00;
	//Parameters
	memcpy((char *)(pCmd+4), (char *)pData, len);
	
	//e.g. 0x0050 interval=80*0.625ms=50ms
	I2cDataSending(pCmd, len+4);
}

void cmd_enable_advertising(u8 enable)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x0A;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x01;
	pCmd[3] = 0x00;
	//Parameters
	if(enable == TRUE)
		pCmd[4] = 0x01;
	else
		pCmd[4] = 0x00;
	
	I2cDataSending(pCmd, 5);
}

void cmd_get_module_available_data_buffer(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x0C;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

void cmd_set_advertising_type(u8 type)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x0D;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x01;
	pCmd[3] = 0x00;
	//Parameters
	if(type > 0x03)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:type error(%d)", type);
		return;
	}
	else
	{
		//0x00:connectable undirected adv
		//0x01:connectable directed adv
		//0x02:scannable undirected adv
		//0x03:non-connectable adv
		pCmd[4] = type;
	}
	
	I2cDataSending(pCmd, 5);
}

void cmd_set_device_name(u8* pData, u8 len)
{
	u8 pCmd[24];

	if(len > 0x12)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:name len error(%d)", len);
		return;
	}

	//cmdID
	pCmd[0] = 0x13;
	pCmd[1] = 0xFF;
	//ParaLen <= 0x12(18)
	pCmd[2] = len;
	pCmd[3] = 0x00;
	//Parameters
	memcpy((char *)(pCmd+4), (char *)pData, len);
	
	//e.g. Device name in hex format
	I2cDataSending(pCmd, len+4);
}

void cmd_get_connection_parameters(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x14;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

void cmd_get_current_work_state(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x16;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

void cmd_terminate_connection(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x17;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

void cmd_restart_module(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x18;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

void cmd_send_data(u8* pData, u8 len)
{
	u8 pCmd[BLE_BUF_SIZE];

	//Base on test, max size 40 byte
	if(len > (BLE_BUF_SIZE - 6 - 18))
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:data len error(%d)", len);
		return;
	}

	//cmdID
	pCmd[0] = 0x1C;
	pCmd[1] = 0xFF;
	//ParaLen <= 0x16(22)
	pCmd[2] = len+2;
	pCmd[3] = 0x00;
	//Handle (0x0011)
	pCmd[4] = 0x11;
	pCmd[5] = 0x00;
	//Parameters
	memcpy((char *)(pCmd+6), (char *)pData, len);
	
	//Handle (2 bytes) of the Attribute
	//"Service to client"
	//e.g. 0x0011
	//Data payload (Max Len:20bytes)
	//e.g. 01,02,03,04,05
	I2cDataSending(pCmd, len+6);
}

void cmd_get_version(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x81;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

void cmd_get_mac_addr(void)
{
	u8 pCmd[8];

	//cmdID
	pCmd[0] = 0x82;
	pCmd[1] = 0xFF;
	//ParaLen
	pCmd[2] = 0x00;
	pCmd[3] = 0x00;
	//Parameters
	//NONE
	
	I2cDataSending(pCmd, 4);
}

// BLE SPP Events (from Telink BLE module to Host)
//Field	 		Bytes 	Description
//Token 		  1 		Always be 0xFF
//ParaLen 		  1 		Length of parameters
//EventID 		  2 		Event ID
//Parameters 	  n 		Parameter payload
void BleCmdExecutionResultEvent(u16 ResEvent, u8 ResCode)
{
	u16 CompleteEvent = (ResEvent & 0x3ff) | 0x400;

	// Check cmmand sending complete event
	if(CompleteEvent == COMP_EVENT_SET_ADV_INTV)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-set advertising interval[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_SET_ADV_DATA)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-set advertising data[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_SET_ADV_ENABLE)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-enable/disable advertising[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_GET_BUF_SIZE)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-get available data buffer[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_SET_ADV_TYPE)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-set advertising type[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_SET_DEV_NAME)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-set device name[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_GET_CONN_PARA)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-get connection param[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_GET_CUR_STATE)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-get current work state[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_TERMINATE)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-terminate[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_RESTART_MOD)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-restart[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else if(CompleteEvent == COMP_EVENT_SEND_NOTIFY_DATA)
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-send data[%s,%d]", \
			ResCode?"FAIL CODE":"SUCC",ResCode);
	}
	else
	{
		Sprintf(URT_NRCMD,"\r\n[BLE]:unknow evid[%04x]", ResEvent);
	}
}

void BleSppEventsProcess(u8* pEvent, u8 len)
{
	if(pEvent[0] == BLE_EVENT_TOKEN)
	{
		u16 EventLen = pEvent[1];
		u16 EventID = pEvent[2]|pEvent[3]<<8;

		// Check event type
		if(EventID == BLE_EVENT_DATA_RECEIVED)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-data received %d", EventLen);
			// Data received
			Ble_Data_Received((pEvent+4+3), (EventLen-2-3));
		}
		else if(EventID == BLE_EVENT_GET_AVAIL_BUF_NUM)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-get available buffer number");
		}
		else if(EventID == BLE_EVENT_CONNECTION)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-connected");
		}
		else if(EventID == BLE_EVENT_TERMINATE)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-terminate");
		}
		else if(EventID == BLE_EVENT_CHL_MAP_CHANGE)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-channel map change event");
		}
		else if(EventID == BLE_EVENT_CONNECTION_PARAM)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-connection parameter update");
		}
		else if(EventID == BLE_EVENT_FLAG_CHN_MAP_UPDATE)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-CHN MAP UPDATE");
		}
		else if(EventID == BLE_EVENT_FLAG_CONN_PARA_UPDATE)
		{
			Sprintf(URT_NRCMD,"\r\n[BLE]:EVT-CONN PARA UPDATE");
		}
		else if(EventID == COMP_EVENT_GET_VERSION_NUMBER)
		{
			if(pEvent[4] == 0)
			{
				Sprintf(URT_NRCMD,"\r\n[BLE]:Version number (%s)", pEvent+5);
			}
		}
		else if(EventID == COMP_EVENT_GET_DEVMAC_ADDRESS)
		{
			if(pEvent[4] == 0)
			{
				char pMac[32] = {0};
				
				memset(pMac, 0, sizeof(pMac));
				sprintf(pMac,"%02X:%02X:%02X:%02X:%02X:%02X", \
					pEvent[5],pEvent[6],pEvent[7],pEvent[8],pEvent[9],pEvent[10]);

				Sprintf(URT_NRCMD,"\r\n[BLE]:Mac address (%s)", pMac);
			}
		}
		else
		{
			// Check event
			BleCmdExecutionResultEvent(EventID, pEvent[4]);
		}
	}
}

void Ble_Data_Sending(u8* pdata, u8 len)
{
	//Precondition:BLE in connection status
	cmd_send_data(pdata, len);
}

void Ble_Data_Received(u8* pdata, u8 len)
{
	//Precondition: BLE in connection status
	// Show hex data
	DumpMem("BLE DATA", pdata, len , URT_ATCMD);
}

/*******************************************************************************
End Of The File
*******************************************************************************/

