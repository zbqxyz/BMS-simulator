/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                 http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               GPIO.c
** Last modified Date:      2017.05.18
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <gpio.h>

void GPIOInit(void )
{
	     	
  SYSLED_OFF();
	BEEP_OFF();	
	FRAM_CS_H(); 	
	W5500_ResetOFF();
  DI1_INT_Init();
	DI2_INT_Init();
	DI3_INT_Init();
	DI4_INT_Init();
}
