//****************************************Copyright (c)***************************************************
//**                                   上海丰宝贸易有限公司
//**                                        研究所(本部)
//**                                 http://www.linposh.com.cn
//**								 http://www.armnxp.com.cn
//**
//**--------------File Info--------------------------------------------------------------------------------
//** File name: 			        Target.c
//** Last modified Date:         	2009.05.15
//** Last Version: 		        	1.0
//** Descriptions: 		        	Linpo-PS-LPC17xx目标板初始化 
//**-------------------------------------------------------------------------------------------------------
//** Created By:                 	Magicoe Niu  牛晓东
//** Created date:   	        	2009.05.15
//** Version:			        	1.0
//** Descriptions: 	            	The original version
//**
//*********************************************************************************************************

#include "includes.h"

#define SYSTICK_ENABLE              0                                          /* Config-Bit to start or stop the SysTick Timer                         */
#define SYSTICK_TICKINT             1                                          /* Config-Bit to enable or disable the SysTick interrupt                 */
#define SYSTICK_CLKSOURCE           2                                          /* Clocksource has the offset 2 in SysTick Control and Status Register   */
#define SYSTICK_MAXCOUNT       ((1<<24) -1)                                    /* SysTick MaxCount                                                      */


/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a necessary 
**						place, change it as needed
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetInit(void)
{
	/* Add your codes here */
	return;
}

/******************************************************************************
** Function name:		GPIOResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**				function; User may change it as needed, but may not 
**				deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void GPIOResetInit(void)
{
	/* Reset all GPIO pins to default: primary function */
	PINSEL0 = 0x00000000;
	PINSEL1 = 0x00000000;
	PINSEL2 = 0x00000000;
	PINSEL3 = 0x00000000;
	PINSEL4 = 0x00000000;
	PINSEL5 = 0x00000000;
	PINSEL6 = 0x00000000;
	PINSEL7 = 0x00000000;
	PINSEL8 = 0x00000000;
	PINSEL9 = 0x00000000;
	PINSEL10 = 0x00000000;
	
	FIO0DIR = 0x00000000;
	FIO1DIR = 0x00000000;
	FIO2DIR = 0x00000000;
	FIO3DIR = 0x00000000;
	FIO4DIR = 0x00000000;
	
	FIO0SET = 0x00000000;
	FIO1SET = 0x00000000;
	FIO2SET = 0x00000000;
	FIO3SET = 0x00000000;
	FIO4SET = 0x00000000;
	return;        
}

/******************************************************************************
** Function name:		ConfigurePLL
**
** Descriptions:		Configure PLL switching to main OSC instead of IRC
**						at power up and wake up from power down. 
**						This routine is used in TargetResetInit() and those
**						examples using power down and wake up such as
**						USB suspend to resume, ethernet WOL, and power management
**						example
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ConfigurePLL(void)
{
	INT32U MValue, NValue, PValue;
	
	PValue = PValue;			/* Protect IDE for warning */
	
	if ( PLL0STAT & (1 << 25) )
	{
		PLL0CON = 1;			/* Enable PLL, disconnected */
		PLL0FEED = 0xaa;
		PLL0FEED = 0x55;
	}
	
	PLL0CON = 0;				/* Disable PLL, disconnected */
	PLL0FEED = 0xaa;
	PLL0FEED = 0x55;
	
	SCS |= 0x20;			/* Enable main OSC */
	while( !(SCS & 0x40) );	/* Wait until main OSC is usable */
	
	CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */
	
	PLL0CFG = PLL0_MValue | (PLL0_NValue << 16);
	PLL0FEED = 0xaa;
	PLL0FEED = 0x55;
	
	PLL0CON = 1;				/* Enable PLL, disconnected */
	PLL0FEED = 0xaa;
	PLL0FEED = 0x55;
	
	CCLKCFG = CCLKDivValue;	/* Set clock divider */
	
	while ( ((PLL0STAT & (1 << 26)) == 0) );	/* Check lock bit status */
	
	MValue = PLL0STAT & 0x00007FFF;
	NValue = (PLL0STAT & 0x00FF0000) >> 16;
	while ((MValue != PLL0_MValue) && ( NValue != PLL0_NValue) );
	
	PLL0CON = 0x03;				/* enable and connect */
	PLL0FEED = 0xaa;
	PLL0FEED = 0x55;
	while ( ((PLL0STAT & (1 << 25)) == 0) );	/* Check connect bit status */
	
	#if USE_USB
	if ( PLL1STAT & (1 << 9) )
	{
		PLL1CON = 1;			/* Enable PLL, disconnected */
		PLL1FEED = 0xaa;
		PLL1FEED = 0x55;
	}
	
	PLL1CON = 0;				/* Disable PLL, disconnected */
	PLL1FEED = 0xaa;
	PLL1FEED = 0x55;
	
	PLL1CFG = PLL1_MValue | (PLL1_PValue << 5);
	PLL1FEED = 0xaa;
	PLL1FEED = 0x55;
	
	PLL1CON = 1;				/* Enable PLL, disconnected */
	PLL1FEED = 0xaa;
	PLL1FEED = 0x55;
	while ( ((PLL1STAT & (1 << 10)) == 0) );	/* Check lock bit status */
	
	MValue = PLL1STAT & 0x0000001F;
	PValue = (PLL1STAT & 0x00000060) >> 5;
	while ((MValue != PLL1_MValue) && ( PValue != PLL1_PValue) );
	
	PLL1CON = 3;				/* enable and connect */
	PLL1FEED = 0xaa;
	PLL1FEED = 0x55;
	while ( ((PLL1STAT & (1 << 9)) == 0) );	/* Check connect bit status */
	#endif
	
	return;
}

/******************************************************************************
** Function name:		TargetResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**						function; User may change it as needed, but may not 
**						deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetResetInit(void)
{
#if USE_USB
//	PCONP |= 0x80000000;		/* Turn On USB PCLK */
#endif
	FLASHCTRL = 0x00;
#if Fcclk < 60000000
	FLASHCTRL = 0x0000201A;//  FLASHTIM = 0x3;
#else
#if Fcclk < 72000000
	FLASHCTRL = 0x0000301A;//  FLASHTIM = 0x4;
#else
	FLASHCTRL = 0x0000401A;//  FLASHTIM = 0x5;
#endif
#endif
//  FLASHCTRL = 0xB;

/* Configure PLL, switch from IRC to Main OSC */
	ConfigurePLL();

/* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
	PCLKSEL0 = 0x00000000;	/* PCLK is 1/4 CCLK */
	PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
	PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
	PCLKSEL1 = 0xAAAAAAAA;	 
#endif
#if (Fpclk / (Fcclk / 4)) == 4
	PCLKSEL0 = 0x55555555;	/* PCLK is the same as CCLK */
	PCLKSEL1 = 0x55555555;	
#endif

//	GPIOResetInit();
//void NVIC_DeInit(void){
	NVIC_DISABLE0 = 0xFFFFFFFF;
	NVIC_DISABLE1 = 0x00000001;
	NVIC_UNPEND0 = 0xFFFFFFFF;
	NVIC_UNPEND1 = 0x00000001;

	NVIC_PRI0 = 0x00000000;
	NVIC_PRI1 = 0x00000000;
	NVIC_PRI2 = 0x00000000;
	NVIC_PRI3 = 0x00000000;
	NVIC_PRI4 = 0x00000000;
	NVIC_PRI5 = 0x00000000;
	NVIC_PRI6 = 0x00000000;
	NVIC_PRI7 = 0x00000000;
	NVIC_PRI8 = 0x00000000;
//}
//void NVIC_SCBDeInit(void){
	NVIC_INT_CTRL = 0x0A000000;
	NVIC_VECT_TABLE = 0x00000000;
	NVIC_AP_INT_RST = 0x05FA0000;
	NVIC_SYS_CTRL = 0x00000000;
	NVIC_CFG_CTRL = 0x00000000;
	NVIC_SYS_H_PRI4 = 0x00000000;
	NVIC_SYS_H_PRI8 = 0x00000000;
	NVIC_SYS_H_PRI12 = 0x00000000;
	NVIC_SYS_H_CTRL = 0x00000000;
	NVIC_FAULT_STA = 0xFFFFFFFF;
	NVIC_HARD_F_STA = 0xFFFFFFFF;
	NVIC_DBG_F_STA = 0xFFFFFFFF;

	NVIC_AP_INT_RST = 0x05FA0000 | 0x600;

	NVIC_VECT_TABLE = 0x00000000 | (0x00 & 0x1FFFFF80);
//}
	SysTick_Config(); 

	GLCD_init();                                    /* Initialize the GLCD */
    GLCD_clear(White);                              /* Clear the GLCD */

	return;
}

/*********************************************************************************************************
** Function name:			SysTick_Config
** Descriptions:			Initialize uC/OS-II's tick source(system timer)，
                            初始化uC/OS-II的时钟源（系统定时器）
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无        
** Created by:				Magicoe Niu  牛晓东
** Created Date:			2009.05.15
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:          
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static  void  SysTick_Config (void)
{
//	if (ticks > SYSTICK_MAXCOUNT)  return (1);  
	NVIC_ST_RELOAD = ( (100000000UL / OS_TICKS_PER_SEC)) - 1;

	NVIC_SYS_H_PRI15 = 0x48;
	NVIC_ST_CURRENT = 0x00;
	NVIC_ST_CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (1<<SYSTICK_TICKINT);
}


/*********************************************************************************************************
** Function name:			tickISRHandler
** Descriptions:			Timeout interrupt handler of system timer 系统定时器超时中断
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无        
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.11.02
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  tickISRHandler (void)
{
    #if OS_CRITICAL_METHOD == 3
        OS_CPU_SR cpu_sr;
    #endif 

    OS_ENTER_CRITICAL();                         
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                                       /*  Call uC/OS-II's OSTimeTick()*/

    OSIntExit();                                 
}
/******************************************************************************
**                            End Of File
******************************************************************************/
