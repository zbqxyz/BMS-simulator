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
** Descriptions:        ��ʼ��˫����PWM���
** input parameters:    ��
** output parameters:   �� 
** Returned value:      ��
*********************************************************************************************************/
void PWMInit (uint8_t duty)
{
	
	uint32_t	pwmrval;
	
//	duty = BCDtoHEX(DutyCycle);						                           	/* ����ռ�ձ�                  */

	LPC_SC->PCONP |= (0x01 << 6);                                       /* ��PWMģ�鹦�ʿ���λ       */  
	LPC_IOCON->P3_25  &= ~0x07;										                      /* ����ѡ��                    */
	LPC_IOCON->P3_25  |= 2;                                             /* P3.25��PWM1[2]���          */
	pwmrval  =   PeripheralClock/10000/2;
	
	LPC_PWM1->PR  = 0x09;                                               /* FPCLK_FREQ��Ƶ��PWM����Ƶ��  */
	                                                                    /* ΪFPCLK_FREQ/10000           */
	LPC_PWM1->MCR = 0x02;                                               /* PWMMR0��PWMTCƥ��ʱ��λPWMTC */
	LPC_PWM1->PCR = (1 << 10);                                        	/* ʹ��PWM2���                 */
	LPC_PWM1->MR0 =  pwmrval;                                           /* PWM���ʿ���/������ֵ         */
	LPC_PWM1->MR1 = (pwmrval / 100) * duty;                                           
	LPC_PWM1->MR2 = (pwmrval / 100) * duty;    
	                                                                    /* ���ñ�������������λ��     */
	LPC_PWM1->LER = 0x7F;                                               /* ��������PWMƥ��ֵ            */
	LPC_PWM1->TCR = 0x02;                                               /* ��λPWMTC                    */
	LPC_PWM1->TCR = 0x09;                                               /* ʹ��PWM                      */
	
}                                                        
