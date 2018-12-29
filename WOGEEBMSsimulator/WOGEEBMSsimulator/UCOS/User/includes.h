/****************************************Copyright (c)***************************************************
;**                                     深圳市聚电科技有限公司
;**                                          研究所(本部)
;**                                     http://www.ueee.cn
;**                                          wujikun
;**--------------File Info--------------------------------------------------------------------------------
;** File name: 			        includes.h
;** Last modified Date:     2017.05.15
;** Last Version: 		      1.0
;** Descriptions: 		      uCOS-II's include file   uCOS-II的头文件 
*********************************************************************************************************/
#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  Date types(Compiler specific)  数据类型（和编译器相关）                
*********************************************************************************************************/
typedef unsigned char   uint8;                                          /* Unsigned  8 bit quantity     */
typedef signed   char   int8;                                           /* Signed    8 bit quantity     */
typedef unsigned short  uint16;                                         /* Unsigned  16 bit quantity    */
typedef signed   short  int16;                                          /* Signed    16 bit quantity    */
typedef unsigned int    uint32;                                         /* Unsigned  32 bit quantity    */
typedef signed   int    int32;                                          /* Signed    32 bit quantity    */
typedef float           fp32;                                           /* Single    precision          */
                                                                        /* floating  point              */
typedef double          fp64;                                           /* Double    precision          */
                                                                        /* floating  point              */

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

/*********************************************************************************************************
  Standard header files 标准头文件
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/*********************************************************************************************************
  Driver's header files  驱动库头文件 
*********************************************************************************************************/


/*********************************************************************************************************
  Port's header files 移植头文件
*********************************************************************************************************/
#include <os_cfg.h>
#include <os_cpu.h>
#include <ucos_ii.h>
#include <main.h>

#ifdef __cplusplus
    }
#endif

#endif

#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_clkpwr.h"
#include "gpio.h"
#include <adc.h>
#include <timer.h>
#include <uart.h>
#include <display.h>
#include <meter.h>
#include <can.h>
#include <bms.h>
#include <ssp.h>
#include <rtc.h>
#include <welink.h>		
#include "bsp.h"
#include "print.h"

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
