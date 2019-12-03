/*******************************************************************************
* File Name          : usr_ble.h
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_ble V1.00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BLE_TOOTH_H
#define _BLE_TOOTH_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

// BLE SPP Events (from Telink BLE module to Host)
//Field	 		Bytes 	Description
//Token 		  1 		Always be 0xFF
//ParaLen 		  1 		Length of parameters
//EventID 		  2 		Event ID
//Parameters 	  n 		Parameter payload
#define BLE_EVENT_TOKEN 					0xFF

//Response EventID 
#define BLE_EVENT_DATA_RECEIVED 			0x07A0
#define BLE_EVENT_GET_AVAIL_BUF_NUM			0x070C
#define BLE_EVENT_CONNECTION				0x0783
#define BLE_EVENT_TERMINATE 				0x0784
#define BLE_EVENT_CHL_MAP_CHANGE 			0x078A
#define BLE_EVENT_CONNECTION_PARAM 			0x078B
#define BLE_EVENT_FLAG_CHN_MAP_UPDATE 		0x078C
#define BLE_EVENT_FLAG_CONN_PARA_UPDATE 	0x078D

//complete EventID 
#define COMP_EVENT_SET_ADV_INTV				0x0701
#define COMP_EVENT_SET_ADV_DATA				0x0702

#define COMP_EVENT_SET_ADV_ENABLE           0x070A
#define COMP_EVENT_GET_BUF_SIZE				0x070C
#define COMP_EVENT_SET_ADV_TYPE				0x070D
#define COMP_EVENT_SET_DEV_NAME				0x0713
#define COMP_EVENT_GET_CONN_PARA			0x0714
#define COMP_EVENT_GET_CUR_STATE			0x0716
#define COMP_EVENT_TERMINATE				0x0717
#define COMP_EVENT_RESTART_MOD				0x0718
#define COMP_EVENT_SEND_NOTIFY_DATA         0x071C
// New command
#define COMP_EVENT_GET_VERSION_NUMBER       0x0781
#define COMP_EVENT_GET_DEVMAC_ADDRESS       0x0782

// Variable Declared


// Function Declare
extern void cmd_set_advertising_interval(u16 interval);
extern void cmd_set_advertising_data(u8* pData, u8 len);
extern void cmd_enable_advertising(u8 enable);
extern void cmd_get_module_available_data_buffer(void);
extern void cmd_set_advertising_type(u8 type);
extern void cmd_set_device_name(u8* pData, u8 len);
extern void cmd_get_connection_parameters(void);
extern void cmd_get_current_work_state(void);
extern void cmd_terminate_connection(void);
extern void cmd_restart_module(void);
extern void cmd_send_data(u8* pData, u8 len);
extern void cmd_get_version(void);
extern void cmd_get_mac_addr(void);

// data process
extern void BleCmdExecutionResultEvent(u16 ResEvent, u8 ResCode);
extern void BleSppEventsProcess(u8* pEvent, u8 len);
extern void Ble_Data_Sending(u8* pdata, u8 len);
extern void Ble_Data_Received(u8* pdata, u8 len);


#endif  /* _BLE_TOOTH_H */

/*******************************************************************************
End Of The File
*******************************************************************************/

