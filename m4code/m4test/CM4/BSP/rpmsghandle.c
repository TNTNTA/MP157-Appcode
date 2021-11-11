/*
 * rpmsghandle.c   handle A7 msg
 *
 *  msg:    |header|data|
 *  header: |msg_type(set/get)|msg_prop|len|
 *
 *  Created on: Nov 12, 2021
 *      Author: tangtao
 *
 */
#include "rpmsghandle.h"

#define RPMSG_SERVICE_NAME   "rpmsg-tty-channel"
#define MAX_BUFFER_SIZE RPMSG_BUFFER_SIZE


static VIRT_UART_HandleTypeDef huart0;
static __IO FlagStatus VirtUart0RxMsg = RESET;
static portSHORT VirtUart0ChannelRxSize = 0;
static portCHAR VirtUart0ChannelBuffRx[MAX_BUFFER_SIZE];

static portCHAR SendBuff[MAX_BUFFER_SIZE];
static PSMART_CAR_CONTEXT spSmartCarContext  = NULL;


static void VIRT_UART0_RxCpltCallback(VIRT_UART_HandleTypeDef *huart);

SmartCar_err_code SetPropCarSpeed(PSMART_CAR_CONTEXT pSmartCarContext, void *data, portCHAR len);
SmartCar_err_code GetPropCarSpeed(PSMART_CAR_CONTEXT pSmartCarContext, void *data, portCHAR *len);

RPMMSG_HANDLE_FUNC_PTR_STRUCT_TYPE MsgHandle[MSG_PROP_CAR_MAX] = {
	{MSG_PROP_CAR_SPEED, GetPropCarSpeed, SetPropCarSpeed},
	{MSG_PROP_CAR_DIR,   NULL, NULL},
	{MSG_PROP_CAR_PID_P, NULL, NULL},
	{MSG_PROP_CAR_PID_I, NULL, NULL},
	{MSG_PROP_CAR_PID_D, NULL, NULL},
};


SmartCar_err_code SetPropCarSpeed(PSMART_CAR_CONTEXT pSmartCarContext, void *data, portCHAR len)
{
	portCHAR *speed = (portCHAR*)data;

	printf("SetPropCarSpeed:%d len:%d\r\n", *speed, len);

	pSmartCarContext->SmartCarConfigData.SpeedConfig = *speed;
	return SMARTCAR_SUCCESS;
}

SmartCar_err_code GetPropCarSpeed(PSMART_CAR_CONTEXT pSmartCarContext, void *data, portCHAR *len)
{
	portCHAR *speed = (portCHAR*)data;

	*speed = pSmartCarContext->SmartCarConfigData.SpeedConfig;
	*len = sizeof(portCHAR);

	printf("GetPropCarSpeed:%d len:%d\r\n", *speed, *len);
	return SMARTCAR_SUCCESS;
}


SmartCar_err_code rpmsghandle_init(PSMART_CAR_CONTEXT pSmartCarContext)
{
	spSmartCarContext = pSmartCarContext;


    if (VIRT_UART_Init(&huart0) != VIRT_UART_OK) {
    	printf("rpmsghandle_init UART failed.\r\n");
    	return SMARTCAR_FAILED;
    }

    if(VIRT_UART_RegisterCallback(&huart0, VIRT_UART_RXCPLT_CB_ID, VIRT_UART0_RxCpltCallback) != VIRT_UART_OK)
	{
    	printf("rpmsghandle_init RegisterCallback failed.\r\n");
    	return SMARTCAR_FAILED;
	}

    printf("rpmsghandle_init succes.\r\n");
    return SMARTCAR_SUCCESS;
}

void VIRT_UART0_RxCpltCallback(VIRT_UART_HandleTypeDef *huart)
{

    printf("Rx Msg received:%s \r\n", (char *) huart->pRxBuffPtr);

    /* copy received msg in a variable to sent it back to master processor in main infinite loop*/
    VirtUart0ChannelRxSize = huart->RxXferSize < MAX_BUFFER_SIZE? huart->RxXferSize : MAX_BUFFER_SIZE-1;
    memcpy(VirtUart0ChannelBuffRx, huart->pRxBuffPtr, VirtUart0ChannelRxSize);
    VirtUart0RxMsg = SET;
}

void rpmsghandle_data()
{
	PRPMMSG_HEADER_TYPE pheader = NULL;
	portCHAR senddatalen = 0;

	OPENAMP_check_for_message();

	if (VirtUart0RxMsg)
	{
		pheader = (PRPMMSG_HEADER_TYPE)VirtUart0ChannelBuffRx;
		if(pheader->msg_prop > MSG_PROP_CAR_MAX || pheader->msg_prop < 0){
			printf("msg_prop not support.\r\n");
			goto out;
		}

		switch(pheader->msg_type){
		case MSG_TYPE_SET:
			if(MsgHandle[pheader->msg_prop].RpmsgHandleSetProp != NULL)
				MsgHandle[pheader->msg_prop].RpmsgHandleSetProp(spSmartCarContext,VirtUart0ChannelBuffRx+sizeof(RPMMSG_HEADER_TYPE), pheader->len);
			break;
		case MSG_TYPE_GET:
			if(MsgHandle[pheader->msg_prop].RpmsgHandleGetProp != NULL)
				MsgHandle[pheader->msg_prop].RpmsgHandleGetProp(spSmartCarContext, SendBuff+sizeof(RPMMSG_HEADER_TYPE), &senddatalen);

			memcpy(SendBuff, pheader, sizeof(RPMMSG_HEADER_TYPE));
			pheader = (PRPMMSG_HEADER_TYPE)SendBuff;
			pheader->len = senddatalen;
			VIRT_UART_Transmit(&huart0, (uint8_t *)SendBuff, sizeof(RPMMSG_HEADER_TYPE) + senddatalen);
			//printf("Transmit len:%d data:%d\r\n", pheader->len, *(SendBuff+sizeof(RPMMSG_HEADER_TYPE)));
			break;
		default:
			printf("msg_type not support:%d\r\n", pheader->msg_type);
			break;
		}
out:
		VirtUart0RxMsg = RESET;
	}
}

