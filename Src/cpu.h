#ifndef __CPU_H__
#define __CPU_H__

#include "include.h"

#define CALCULATION_PERIOD 1000 //计数基数
#define MAX_SAVE_TASK_INFO_NUM	20

typedef struct
{
	unsigned int start_time;
	unsigned int spent_time;
	unsigned int totall_time;
	unsigned int curr_run_time;
}S_Task_Run_Time,*P_S_Task_Run_Time;

typedef struct 
{
	char* name;
	unsigned char cur_task_cpu_use_rate;
	unsigned long empty_stack;
	char priority;
	osThreadId task_handle;
	S_Task_Run_Time run_time;
}S_Task_Info,*P_S_Task_Info;

typedef struct 
{
	unsigned char save_number;
	unsigned char cur_cpu_use_rate;
	S_Task_Info task[MAX_SAVE_TASK_INFO_NUM];
	unsigned char get_stack_step:3;//每8s 获取一次空闲堆栈容量
}S_SYS_Task_Info,*P_S_SYS_Task_Info;

typedef struct 
{
	volatile StackType_t	*pxTopOfStack;	/*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

	#if ( portUSING_MPU_WRAPPERS == 1 )
		xMPU_SETTINGS	xMPUSettings;		/*< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
		BaseType_t		xUsingStaticallyAllocatedStack; /* Set to pdTRUE if the stack is a statically allocated array, and pdFALSE if the stack is dynamically allocated. */
	#endif

	ListItem_t			xGenericListItem;	/*< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
	ListItem_t			xEventListItem;		/*< Used to reference a task from an event list. */
	UBaseType_t			uxPriority;			/*< The priority of the task.  0 is the lowest priority. */
	StackType_t			*pxStack;			/*< Points to the start of the stack. */
	char				pcTaskName[ configMAX_TASK_NAME_LEN ];/*< Descriptive name given to the task when created.  Facilitates debugging only. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

	#if ( portSTACK_GROWTH > 0 )
		StackType_t		*pxEndOfStack;		/*< Points to the end of the stack on architectures where the stack grows up from low memory. */
	#endif

	#if ( portCRITICAL_NESTING_IN_TCB == 1 )
		UBaseType_t 	uxCriticalNesting; 	/*< Holds the critical section nesting depth for ports that do not maintain their own count in the port layer. */
	#endif

	#if ( configUSE_TRACE_FACILITY == 1 )
		UBaseType_t		uxTCBNumber;		/*< Stores a number that increments each time a TCB is created.  It allows debuggers to determine when a task has been deleted and then recreated. */
		UBaseType_t  	uxTaskNumber;		/*< Stores a number specifically for use by third party trace code. */
	#endif

	#if ( configUSE_MUTEXES == 1 )
		UBaseType_t 	uxBasePriority;		/*< The priority last assigned to the task - used by the priority inheritance mechanism. */
		UBaseType_t 	uxMutexesHeld;
	#endif

	#if ( configUSE_APPLICATION_TASK_TAG == 1 )
		TaskHookFunction_t pxTaskTag;
	#endif

	#if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
		void *pvThreadLocalStoragePointers[ configNUM_THREAD_LOCAL_STORAGE_POINTERS ];
	#endif

	#if ( configGENERATE_RUN_TIME_STATS == 1 )
		uint32_t		ulRunTimeCounter;	/*< Stores the amount of time the task has spent in the Running state. */
	#endif

	#if ( configUSE_NEWLIB_REENTRANT == 1 )
		/* Allocate a Newlib reent structure that is specific to this task.
		Note Newlib support has been included by popular demand, but is not
		used by the FreeRTOS maintainers themselves.  FreeRTOS is not
		responsible for resulting newlib operation.  User must be familiar with
		newlib and must provide system-wide implementations of the necessary
		stubs. Be warned that (at the time of writing) the current newlib design
		implements a system-wide malloc() that must be provided with locks. */
		struct 	_reent xNewLib_reent;
	#endif

	#if ( configUSE_TASK_NOTIFICATIONS == 1 )
		volatile uint32_t ulNotifiedValue;
		volatile char eNotifyState; //eNotifyValue
	#endif

} My_tskTCB;



/*
INCLUDE_xTaskGetIdleTaskHandle  ->1
#define configUSE_IDLE_HOOK  ->1               
#define configUSE_TICK_HOOK  ->1

#define traceTASK_SWITCHED_IN()  extern void StartIdleMonitor(void); \
                                 StartIdleMonitor()
#define traceTASK_SWITCHED_OUT() extern void EndIdleMonitor(void); \
                                EndIdleMonitor()

//放到freertosconfig.h 中               
*/




void StartIdleMonitor(void);
void EndIdleMonitor(void);
void vApplicationIdleHook( void );
void vApplicationTickHook( void );



void start_monitor_task(void); //统计CPU利用率

void save_task_info(void);
void delete_task_info(TaskHandle_t target_task_handle);
void save_task_start_time(osThreadId task_handle);
void save_task_end_time(osThreadId task_handle);
void count_task_run_time(void);
int test_cpu_speed(unsigned int loop_outside,unsigned int loop_inside);

#endif

