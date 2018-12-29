/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                 http://www.tomorn.cn
**                                      wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               timer.c
** Last modified Date:      2017.05.18
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <timer.h>
#include <gpio.h>
/*********************************************************************************************************
** Function name:	    Timer0Init
** Descriptions:	    定时器0初始化程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Timer0Init (void)
{
    LPC_SC->PCONP  |= (1 << 1);                                              /* 打开定时器1的功率控制位      */
	
		LPC_TIM0->TCR  = 0x02;
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;
    LPC_TIM0->MR0  = OSC_CLK*2/10;                                           /* 0.1S中断1次                  */
    LPC_TIM0->MCR  = 0x03;                                                   /* 匹配后产生中断               */
    
		NVIC_EnableIRQ(TIMER0_IRQn);                                      
    NVIC_SetPriority(TIMER0_IRQn, 3);
    LPC_TIM0->TCR  = 0x01;                                                   /*启动定时器                    */    

}
/*********************************************************************************************************
* Function Name:        TIMER0_IRQHandler
* Description:          TIMER0 中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void TIMER0_IRQHandler(void)
{
	static uint8_t WDTCnt;
	
    	LPC_TIM0->IR         = 0x01;					                                     /* 清除中断标志	                */ 
		
		   WDTCnt++;                                                                /* 看门狗定时，0.2秒喂狗         */ 
	if(WDTCnt%2==0)
	{		
		WDT_ON();                                                                   //外部看门狗喂狗，内部看门狗未使能
	}
	else
	{  
		WDT_OFF();
	}
	
}
