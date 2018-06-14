/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId sys_watcherHandle;
osThreadId flash_serverHandle;
osThreadId serial_serverHandle;
osThreadId sys_coreHandle;
osThreadId tcm300_serverHandle;
osThreadId web_serverHandle;
osMessageQId serial_queueHandle;
osMessageQId flash_queueHandle;
osMessageQId tcm300_tx_queueHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void sys_watcher_function(void const * argument);
void flash_server_function(void const * argument);
void serial_server_function(void const * argument);
void sys_core_function(void const * argument);
void tcm300_tx_server(void const * argument);
void web_server_function(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of sys_watcher */
  osThreadDef(sys_watcher, sys_watcher_function, osPriorityNormal, 0, 200);
  sys_watcherHandle = osThreadCreate(osThread(sys_watcher), NULL);

  /* definition and creation of flash_server */
  osThreadDef(flash_server, flash_server_function, osPriorityAboveNormal, 0, 600);
  flash_serverHandle = osThreadCreate(osThread(flash_server), NULL);

  /* definition and creation of serial_server */
  osThreadDef(serial_server, serial_server_function, osPriorityHigh, 0, 400);
  serial_serverHandle = osThreadCreate(osThread(serial_server), NULL);

  /* definition and creation of sys_core */
  osThreadDef(sys_core, sys_core_function, osPriorityAboveNormal, 0, 300);
  sys_coreHandle = osThreadCreate(osThread(sys_core), NULL);

  /* definition and creation of tcm300_server */
  osThreadDef(tcm300_server, tcm300_tx_server, osPriorityHigh, 0, 300);
  tcm300_serverHandle = osThreadCreate(osThread(tcm300_server), NULL);

  /* definition and creation of web_server */
  osThreadDef(web_server, web_server_function, osPriorityAboveNormal, 0, 300);
  web_serverHandle = osThreadCreate(osThread(web_server), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of serial_queue */
  osMessageQDef(serial_queue, 8, uint32_t);
  serial_queueHandle = osMessageCreate(osMessageQ(serial_queue), NULL);

  /* definition and creation of flash_queue */
  osMessageQDef(flash_queue, 5, uint32_t);
  flash_queueHandle = osMessageCreate(osMessageQ(flash_queue), NULL);

  /* definition and creation of tcm300_tx_queue */
  osMessageQDef(tcm300_tx_queue, 8, uint32_t);
  tcm300_tx_queueHandle = osMessageCreate(osMessageQ(tcm300_tx_queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* sys_watcher_function function */
void sys_watcher_function(void const * argument)
{

  /* USER CODE BEGIN sys_watcher_function */
  InitW5500();
  osDelay(1000);
  vStartMQTTTasks(400,osPriorityAboveNormal);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END sys_watcher_function */
}

/* flash_server_function function */
void flash_server_function(void const * argument)
{
  /* USER CODE BEGIN flash_server_function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END flash_server_function */
}

/* serial_server_function function */
void serial_server_function(void const * argument)
{
  /* USER CODE BEGIN serial_server_function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END serial_server_function */
}

/* sys_core_function function */
void sys_core_function(void const * argument)
{
  /* USER CODE BEGIN sys_core_function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END sys_core_function */
}

/* tcm300_tx_server function */
void tcm300_tx_server(void const * argument)
{
  /* USER CODE BEGIN tcm300_tx_server */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tcm300_tx_server */
}

/* web_server_function function */
void web_server_function(void const * argument)
{
  /* USER CODE BEGIN web_server_function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END web_server_function */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
