#include "include.h"

int Cpu_Use_Rate_Tick ;

osThreadId IDlEhandle = NULL;
osThreadId monitor_handle = NULL;

S_SYS_Task_Info SYS_Task_Info;


void start_monitor_task(void)
{
	monitor_handle = xTaskGetCurrentTaskHandle();
}

void StartIdleMonitor(void)
{
	TaskHandle_t task_handle;
	task_handle = xTaskGetCurrentTaskHandle() ;
    	save_task_start_time(task_handle);
}


void EndIdleMonitor(void)
{
	TaskHandle_t task_handle;
	task_handle = xTaskGetCurrentTaskHandle() ;
	save_task_end_time(task_handle);
}

void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
	if(IDlEhandle == NULL)
		{
			IDlEhandle = xTaskGetCurrentTaskHandle();
			start_monitor_task();
			save_task_info();
		}
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
if(Cpu_Use_Rate_Tick ++ >CALCULATION_PERIOD)  //每CALCULATION_PERIOD个tick,刷新一次CPU使用率   
	{
		Cpu_Use_Rate_Tick = 0;
		count_task_run_time();
	}
}

void save_task_info(void)
{
	unsigned char loop20;
	TaskHandle_t task_handle;
	task_handle = xTaskGetCurrentTaskHandle();
	for(loop20 = 0;loop20 < MAX_SAVE_TASK_INFO_NUM; loop20++ )
		{
			if(task_handle == SYS_Task_Info.task[loop20].task_handle)
				{
					return;
				}
			continue;
		}
	for(loop20 = 0;loop20 < MAX_SAVE_TASK_INFO_NUM; loop20++ )
		{
			if(SYS_Task_Info.task[loop20].task_handle)
				{
				}
			else
				{
					break;
				}
		}
	/*SYS_Task_Info.task[SYS_Task_Info.save_number ].empty_stack = (uxTaskGetStackHighWaterMark(task_handle)*4);
	SYS_Task_Info.task[SYS_Task_Info.save_number ].task_handle = task_handle;
	SYS_Task_Info.task[SYS_Task_Info.save_number ].name = ((My_tskTCB*)task_handle)-> pcTaskName;
	SYS_Task_Info.task[SYS_Task_Info.save_number ].priority = (char)(((My_tskTCB*)task_handle)-> uxPriority);
	SYS_Task_Info.save_number ++;*/

	SYS_Task_Info.task[loop20].empty_stack = (uxTaskGetStackHighWaterMark(task_handle)*4);
	SYS_Task_Info.task[loop20].task_handle = task_handle;
	SYS_Task_Info.task[loop20].name = ((My_tskTCB*)task_handle)-> pcTaskName;
	SYS_Task_Info.task[loop20].priority = (char)(((My_tskTCB*)task_handle)-> uxPriority);
	SYS_Task_Info.save_number ++;

}


void save_task_start_time(osThreadId task_handle)
{
	unsigned loopx = 0;
	for(;loopx<MAX_SAVE_TASK_INFO_NUM;loopx++)
		{
			if(task_handle == SYS_Task_Info.task[loopx].task_handle)
				{
					SYS_Task_Info.task[loopx].run_time.start_time=xTaskGetTickCountFromISR();
					return;
				}
		}
}


void save_task_end_time(osThreadId task_handle)
{
	unsigned loopx = 0;
	for(;loopx<MAX_SAVE_TASK_INFO_NUM;loopx++)
		{
			if(task_handle == SYS_Task_Info.task[loopx].task_handle)
				{
					SYS_Task_Info.task[loopx].run_time.spent_time = xTaskGetTickCountFromISR() - SYS_Task_Info.task[loopx].run_time.start_time ;
					SYS_Task_Info.task[loopx].run_time.totall_time += SYS_Task_Info.task[loopx].run_time.spent_time;
					return;
				}
		}
}
void delete_task_info(TaskHandle_t target_task_handle)
{
	unsigned char loopx;
	for(loopx = 0; loopx < MAX_SAVE_TASK_INFO_NUM; loopx ++)
		{
			if(SYS_Task_Info.task[loopx].task_handle == target_task_handle)
				{
					SYS_Task_Info.task[loopx].task_handle = NULL;
					SYS_Task_Info.save_number --;
					break;
				}
		}
}


void count_task_run_time(void)
{
	unsigned loopx = 0;
	for(;loopx<MAX_SAVE_TASK_INFO_NUM;loopx++)
		{
			SYS_Task_Info.task[loopx].run_time.curr_run_time = (SYS_Task_Info.task[loopx].run_time.totall_time > 1000? 1000 : SYS_Task_Info.task[loopx].run_time.totall_time);			
			SYS_Task_Info.task[loopx].cur_task_cpu_use_rate = (SYS_Task_Info.task[loopx].run_time.curr_run_time * 100) /CALCULATION_PERIOD;
			if(SYS_Task_Info.task[loopx].task_handle == IDlEhandle)
				{
					if( SYS_Task_Info.task[loopx].cur_task_cpu_use_rate <=100)
						{
							SYS_Task_Info.cur_cpu_use_rate = 100 - SYS_Task_Info.task[loopx].cur_task_cpu_use_rate;
						}					
				}
			if(!SYS_Task_Info.get_stack_step)
				{
					SYS_Task_Info.task[loopx].empty_stack = (uxTaskGetStackHighWaterMark(SYS_Task_Info.task[loopx].task_handle)*4);
				}
			SYS_Task_Info.task[loopx].run_time.totall_time = 0;
		}
	SYS_Task_Info.get_stack_step++;
}

int test_cpu_speed(unsigned int loop_outside,unsigned int loop_inside)
{
	static int loop_out,loop_in,sum;
	
	for(loop_out = 0 ; loop_out < loop_outside ; loop_out++)
		{
			for(loop_in = 0 ; loop_in < loop_inside ; loop_in ++)
				{
					sum ++;
				}
		}
	return sum;
}

