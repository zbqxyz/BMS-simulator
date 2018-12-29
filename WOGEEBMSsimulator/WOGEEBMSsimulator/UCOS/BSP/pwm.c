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
#include <pwm.h>
/*********************************************************************************************************
** Function name:       PWMInit
** Descriptions:        初始化双边沿PWM输出
** input parameters:    无
** output parameters:   无 
** Returned value:      无
*********************************************************************************************************/
void PWMInit (uint8_t duty)
{
	
	uint32_t	pwmrval;
	
//	duty = BCDtoHEX(DutyCycle);						                           	/* 导入占空比                  */

	LPC_SC->PCONP |= (0x01 << 6);                                       /* 打开PWM模块功率控制位       */  
	LPC_IOCON->P3_25  &= ~0x07;										                      /* 功能选择                    */
	LPC_IOCON->P3_25  |= 2;                                             /* P3.25作PWM1[2]输出          */
	pwmrval  =   PeripheralClock/10000/2;
	
	LPC_PWM1->PR  = 0x09;                                               /* FPCLK_FREQ分频，PWM计数频率  */
	                                                                    /* 为FPCLK_FREQ/10000           */
	LPC_PWM1->MCR = 0x02;                                               /* PWMMR0与PWMTC匹配时复位PWMTC */
	LPC_PWM1->PCR = (1 << 10);                                        	/* 使能PWM2输出                 */
	LPC_PWM1->MR0 =  pwmrval;                                           /* PWM速率控制/计数初值         */
	LPC_PWM1->MR1 = (pwmrval / 100) * duty;                                           
	LPC_PWM1->MR2 = (pwmrval / 100) * duty;    
	                                                                    /* 设置边沿脉冲的脉宽和位置     */
	LPC_PWM1->LER = 0x7F;                                               /* 锁存所有PWM匹配值            */
	LPC_PWM1->TCR = 0x02;                                               /* 复位PWMTC                    */
	LPC_PWM1->TCR = 0x09;                                               /* 使能PWM                      */
	
}                                                        
