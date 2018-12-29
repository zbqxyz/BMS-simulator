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
** Descriptions:	    ��ʱ��0��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Timer0Init (void)
{
    LPC_SC->PCONP  |= (1 << 1);                                              /* �򿪶�ʱ��1�Ĺ��ʿ���λ      */
	
		LPC_TIM0->TCR  = 0x02;
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;
    LPC_TIM0->MR0  = OSC_CLK*2/10;                                           /* 0.1S�ж�1��                  */
    LPC_TIM0->MCR  = 0x03;                                                   /* ƥ�������ж�               */
    
		NVIC_EnableIRQ(TIMER0_IRQn);                                      
    NVIC_SetPriority(TIMER0_IRQn, 3);
    LPC_TIM0->TCR  = 0x01;                                                   /*������ʱ��                    */    

}
/*********************************************************************************************************
* Function Name:        TIMER0_IRQHandler
* Description:          TIMER0 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER0_IRQHandler(void)
{
	static uint8_t WDTCnt;
	
    	LPC_TIM0->IR         = 0x01;					                                     /* ����жϱ�־	                */ 
		
		   WDTCnt++;                                                                /* ���Ź���ʱ��0.2��ι��         */ 
	if(WDTCnt%2==0)
	{		
		WDT_ON();                                                                   //�ⲿ���Ź�ι�����ڲ����Ź�δʹ��
	}
	else
	{  
		WDT_OFF();
	}
	
}
