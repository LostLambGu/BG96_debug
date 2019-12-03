/*******************************************************************************
* File Name          : usr_ioctl.h
* Author               : Jevon
* Description        : This file provides all the version functions.

* History:
*  04/30/2019 : usr_ioctl V1.00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DEVICE_IO_CTL_H
#define _DEVICE_IO_CTL_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#define ACC1_SIG_PF0_READ 	HAL_GPIO_ReadPin(PA8_ACC_INT1_GPIO_Port,PA8_ACC_INT1_Pin)
#define ACC2_SIG_PF0_READ 	HAL_GPIO_ReadPin(PB7_ACC_INT2_GPIO_Port,PB7_ACC_INT2_Pin)
#define IGNI_SIG_PA6_READ 	HAL_GPIO_ReadPin(PA6_IGNI_INT_GPIO_Port,PA6_IGNI_INT_Pin)
#define BLE_WAKE_PA12_READ 	HAL_GPIO_ReadPin(PB9_BLE_INT_GPIO_Port,PB9_BLE_INT_Pin)
#define MDM_WAKE_PB5_READ 	HAL_GPIO_ReadPin(PB5_MDM_WINT_GPIO_Port,PB5_MDM_WINT_Pin)


// Variable Declared


//Function Declare
extern void Show_IO_IN_Status(void);
extern void SYS_CHG_EN_Control(Enctl Status);
extern void BLE_PWR_EN_Control(Enctl Status);
extern void BLE_WAKEUP_Control(WakeCtrl Status);
extern void MDM_PWR_EN_Control(Enctl Status);
extern void MDM_PWR_KEY_Control(PressCtrl Status);
extern void MDM_RESET_Control(RstCtrl Status);
extern void MDM_WAKE_Control(WakeCtrl Status);
extern void EXT_RELAY_Control(Enctl Status);
extern void BATTERY_EN_Control(Enctl Status);
extern void BUZZER_EN_Control(Enctl Status);
extern void DCDC_EN_Control(Enctl Status);
extern void EXT_GPIO_Control(etLevel Status);

#endif  /* _DEVICE_IO_CTL_H */

/*******************************************************************************
End Of The File
*******************************************************************************/

