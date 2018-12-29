//****************************************Copyright (c)***************************************************
//**                                   上海丰宝贸易有限公司
//**                                        研究所(本部)
//**                                 http://www.linposh.com.cn
//**								 http://www.armnxp.com.cn
//**
//**--------------File Info--------------------------------------------------------------------------------
//** File name: 			        Target.h
//** Last modified Date:         	2009.05.15
//** Last Version: 		        	1.0
//** Descriptions: 		        	Linpo-PS-LPC17xx目标板初始化头文件 
//**-------------------------------------------------------------------------------------------------------
//** Created By:                 	Magicoe Niu  牛晓东
//** Created date:   	        	2009.05.15
//** Version:			        	1.0
//** Descriptions: 	            	The original version
//**
//*********************************************************************************************************

#ifndef __TARGET_H
#define __TARGET_H

#ifdef __cplusplus
extern "C" {
#endif

#define	USE_USB					0

//This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define PLL0_MValue			49
#define PLL0_NValue			2
#define CCLKDivValue		3

#define Fosc	12000000
#define Fcclk	100000000
#define Fcco	400000000

#if USE_USB
/* Fusb_clk = 48Mhz, Fusb_cco = 192Mhz */
#define PLL1_MValue			3
#define PLL1_PValue			1
#endif

#define Fpclk	(Fcclk / 2)			   

/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a 
**				necessary place, change it as needed
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern void TargetInit(void);
extern void ConfigurePLL(void);
extern void TargetResetInit(void);
static  void  SysTick_Config (void);
extern void  SysTick_Handler (void);

#ifdef __cplusplus
   }
#endif
 
#endif /* end __TARGET_H */
/******************************************************************************
**                            End Of File
******************************************************************************/
