/****************************************Copyright (c)****************************************************
**                              ueee electronics Co.,LTD.
**                                     
**                                 http://www.ueee.com
**                                    wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Main.c
** Last modified Date:      2017.05.18
** Last Version:            1.0
** Description:             The main function 
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              wujikun 
** Created date:            2017.05.18
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾
**
*********************************************************************************************************/

#ifndef __MAIN_H 
#define __MAIN_H 

#include "lpc_types.h"


#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  TASK PRIORITIES �������ȼ�
*********************************************************************************************************/		
#define  TASK_START_PRIO          0
#define  TASK_GPIO_PRIO			   	  1
#define  TASK_ADC_PRIO			   	  2
#define  TASK_DISPLAY_PRIO			  3
#define  TASK_METER_PRIO			    4
#define  TASK_BMS_PRIO			   	  5	
#define  TASK_Welink_PRIO			   	6
#define  TASK_QUEsend_PRIO			  16
#define  TASK_QUEreceive_PRIO			  17
/*********************************************************************************************************
  TASK STACK SIZES  �����ջ��С
*********************************************************************************************************/
#define  TASK_START_STK_SIZE      100
#define  TASK_GPIO_STK_SIZE			  100
#define  TASK_ADC_STK_SIZE			  100
#define  TASK_DISPLAY_STK_SIZE	  500
#define  TASK_METER_STK_SIZE		  300
#define  TASK_BMS_STK_SIZE			  600
#define  TASK_Welink_STK_SIZE			1000
#define  TASK_QUEsend_STK_SIZE			    200
#define  TASK_QUEreceive_STK_SIZE			  200


typedef struct
{	
	uint8_t 	MeterNO[6];              //�����
	uint8_t   TimeChargeMode;          //ʱ����ģʽ
	uint16_t  ChargeLimitedTime;			 //�������ʱ��
	uint8_t   SOCChargeMode;	         //SOC���ģʽ
	uint16_t  SOCIncreaseTime;		     //SOC����ʱ���ٶ�
	uint8_t   CS;
	
}SYS_PARA;

extern  SYS_PARA   Sys_PARA;

//extern volatile unsigned long BCLCnt;
extern volatile unsigned long CAN0TickCnt;


#ifdef __cplusplus
    }
#endif


#endif
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/


