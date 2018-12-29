/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Main.c
** Last modified Date:      2007.01.18
** Last Version:            1.0
** Description:             The main function example template  ����������ģ��
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Steven Zhou ���ܸ�
** Created date:            2007.01.18
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao
** Modified date:           2007.10.29
** Version:                 1.1
** Description:             The second version �ڶ���
**
*********************************************************************************************************/

#ifndef __MAIN_H 
#define __MAIN_H 

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  TASK PRIORITIES �������ȼ�
*********************************************************************************************************/		
#define  TASK_START_PRIO              0
#define  TASK_LED1_PRIO			   	  1
#define  TASK_LED2_PRIO			   	  2
#define  TASK_LED3_PRIO			   	  3
#define  TASK_LED4_PRIO			   	  4
							          
/*********************************************************************************************************
  TASK STACK SIZES  �����ջ��С
*********************************************************************************************************/
#define  TASK_START_STK_SIZE          50
#define  TASK_LED1_STK_SIZE			  50
#define  TASK_LED2_STK_SIZE			  50
#define  TASK_LED3_STK_SIZE			  50
#define  TASK_LED4_STK_SIZE			  50

#ifdef __cplusplus
    }
#endif


#endif
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

