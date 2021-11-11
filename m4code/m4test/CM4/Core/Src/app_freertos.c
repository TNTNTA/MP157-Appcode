/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/**
 * start_task
 * RPMsgHandleTask
 *
 * PI5 --TIM8->CH1    PI6 --TIM8->CH2
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Smartcar_common.h"
#include "rpmsghandle.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define START_TASK_PRIO		1
#define START_STK_SIZE 		128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define RPMSG_TASK_PRIO		2
#define RPMSG_STK_SIZE 		512
TaskHandle_t RPMsgTask_Handler;
void RPMsgTask(void *pvParameters);

#define SPORT_CON_TASK_PRIO		3
#define SPORT_CON_STK_SIZE 		512
TaskHandle_t SportConTask_Handler;
void SportControlTask(void *pvParameters);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SMART_CAR_CONTEXT SmartcarContext;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    xTaskCreate((TaskFunction_t )start_task,
                (const char*    )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*  )&StartTask_Handler);
  /* USER CODE END Init */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t )RPMsgTask,
                (const char*    )"RPMsgTask",
                (uint16_t       )RPMSG_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )RPMSG_TASK_PRIO,
                (TaskHandle_t*  )&RPMsgTask_Handler);

    xTaskCreate((TaskFunction_t )SportControlTask,
                (const char*    )"SportControlTask",
                (uint16_t       )SPORT_CON_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )SPORT_CON_TASK_PRIO,
                (TaskHandle_t*  )&SportConTask_Handler);

    taskEXIT_CRITICAL();

    while(1){
    	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    	vTaskDelay(500);
    	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    	vTaskDelay(500);
    }
}



void RPMsgTask(void *pvParameters)
{

	rpmsghandle_init(&SmartcarContext);
	while(1)
	{
		rpmsghandle_data();
		vTaskDelay(2);
	}
}

void SportControlTask(void *pvParameters)
{
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);

	while(1){
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 209);
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 209);
		vTaskDelay(10);
	}
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
