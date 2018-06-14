#ifndef __INCLUDE_H__
#define __INCLUDE_H__
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;

#define EFFECT 	0X55
#define INVALID 	0XAA

typedef int (*Int_Function_Int) (int arg);
typedef void (*Void_Function_Int) (int arg);
typedef void (*Void_Function_Void) (void);
typedef void* (*PVoid_Function_PVoid) (void* arg);

typedef unsigned char (*UChar_Function_Void) (void);
typedef unsigned char (*UChar_Function_PUChar_UShort) (unsigned char* data,unsigned short data_length);
typedef void (*Void_Function_PChar_PChar_UShort) (char*from,char* to,unsigned short data_length);
typedef unsigned short(*UShort_Function_PUChar_UShort)(unsigned char*  data,unsigned short date_length);

#define MSEC		1
#define SECOND	(1000*(MSEC))
#define MINUTE	(60*(SECOND))
#define HOUR	(60*(MINUTE))
#define DAY		(24*(HOUR))
#define WEEK	(7*(DAY))


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>
#include <stddef.h>
//#include "core_cmFunc.h"

//#include "event_groups.h"
//#include "StackMacros.h"


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"



#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
//#include "my_it.h"

//#include "flash.h"
//#include "save_data.h"
//#include "TCM300.h"
//#include "queue.h"
//#include "semphr.h"
#include "w5500.h"
#include "socket.h"
#include "w5500_driver.h"
#include "wizchip_conf.h"
#include "cpu.h"
//#include "CRC.h"
//#include "bsp.h"

//#include "gateway.h"
//#include "iap.h"

//#include "ult.h" 
//#include "httpd.h"
//#include "httpult.h"
//#include "json.h"






//#include "object.h"
#endif



