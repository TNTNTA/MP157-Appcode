/*
 * rpmsghandle.h
 *
 *  Created on: Nov 12, 2021
 *      Author: tangtao
 */

#ifndef RPMSGHANDLE_H_
#define RPMSGHANDLE_H_

#include "Smartcar_common.h"
#include "virt_uart.h"

typedef enum {
	MSG_TYPE_SET = 's',
	MSG_TYPE_GET = 'g',
	MSG_TYPE_MAX
}MSG_TYPE;

typedef enum {
	MSG_PROP_CAR_SPEED,
	MSG_PROP_CAR_DIR,
	MSG_PROP_CAR_PID_P,
	MSG_PROP_CAR_PID_I,
	MSG_PROP_CAR_PID_D,
	MSG_PROP_CAR_MAX
}MSG_PROP;

typedef struct
{
	MSG_TYPE 	msg_type;
	MSG_PROP 	msg_prop;
	portCHAR 	len;
}RPMMSG_HEADER_TYPE, *PRPMMSG_HEADER_TYPE;

typedef SmartCar_err_code (*RPMMSG_HANDLE_GET_PROP)(PSMART_CAR_CONTEXT pSmartCarContext, void *data, portCHAR *len);
typedef SmartCar_err_code (*RPMMSG_HANDLE_SET_PROP)(PSMART_CAR_CONTEXT pSmartCarContext, void *data, portCHAR len);

typedef struct {
	MSG_PROP 	msg_prop;
	RPMMSG_HANDLE_GET_PROP RpmsgHandleGetProp;
	RPMMSG_HANDLE_SET_PROP RpmsgHandleSetProp;
}RPMMSG_HANDLE_FUNC_PTR_STRUCT_TYPE;

SmartCar_err_code rpmsghandle_init(PSMART_CAR_CONTEXT pSmartCarContext);
void rpmsghandle_data();

#endif /* RPMSGHANDLE_H_ */
