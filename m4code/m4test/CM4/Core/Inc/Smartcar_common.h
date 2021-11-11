#ifndef SMARTCAR_COMMON_H
#define SMARTCAR_COMMON_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "ipcc.h"
#include "openamp.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"


typedef enum {
	SMARTCAR_SUCCESS,
	SMARTCAR_FAILED,
	SMARTCAR_NOTSUPPORT,
	SMARTCAR_NOVAL,
}SmartCar_err_code;

//Command to control the smart car direction of movement
typedef enum {
	SMARTCAR_CMD_DIR_FRONT,
	SMARTCAR_CMD_DIR_BACK,
	SMARTCAR_CMD_DIR_LEFT,
	SMARTCAR_CMD_DIR_RIGHT,
	SMARTCAR_CMD_DIR_MAX
} SMARTCAR_CMD_DIRECTION;

//Command to control the smart car action
typedef enum {
	SMARTCAR_CMD_ACT_HELLO,
	SMARTCAR_CMD_ACT_MAX
} SMARTCAR_CMD_ACTION;


typedef struct _SMART_CAR_CONFIG_PID{
	portSHORT Kp;
	portFLOAT Ki;
	portFLOAT Kd;
} SMART_CAR_CONFIG_PID, *PSMART_CAR_CONFIG_PID;

//All Config for the smart car
typedef struct _SMART_CAR_CONFIG_DATA{
	portSHORT				SpeedConfig;
	SMART_CAR_CONFIG_PID 	PidConfig;

} SMART_CAR_CONFIG_DATA, *PSMART_CAR_CONFIG_DATA;


//All Context
typedef struct _SMART_CAR_CONTEXT{
	SMART_CAR_CONFIG_DATA SmartCarConfigData;





} SMART_CAR_CONTEXT, *PSMART_CAR_CONTEXT;









#endif
