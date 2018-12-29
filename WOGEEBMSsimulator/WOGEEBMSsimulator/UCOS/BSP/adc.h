#ifndef __ADC_H_
#define __ADC_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"


#define ADC_12BITS          1

#define ADC_DONE            0x80000000
#define ADC_OVERRUN         0x40000000
#define ADC_ADINT           0x00010000

#define ADC_NUM             10


#define ADC_CHN				  5		    /*	ת����ͨ��	  */
#define ADC_TIMES				100	  	/*	��������	  */


#define 	CNT_OPN	 0x2C0	 						//���˿�����,��Ӧ��ֵ�����
#define 	CNT_ETH	 0x000				 			//���˿ڽӵ�,��Ӧ��ֵ0
#define 	CNT_CON	 0x200	 						//���˿����ӳ��ǹ��������S2δ�ر�,��Ӧ��ѹֵ2740ŷ��
#define 	CNT_WRK	 0x140				 			//���˿ڳ���׼���ã�������S2�ر�,��Ӧ��ѹֵ883ŷ��
#define 	MIN6V	 	(CNT_WRK-0x60)			//�������CP��ѹ��Сֵ
#define 	MAX6V	 	(CNT_WRK+0x60)			//�������CP��ѹ���ֵ
#define 	MIN9V	 	(CNT_CON-0x60)	 		//���˿����ӣ�������S2δ�ر�,��Ӧ��ѹֵ2740ŷ��
#define 	MAX9V	 	(CNT_CON+0x60)	 		//���˿����ӣ�������S2δ�ر�,��Ӧ��ѹֵ2740ŷ��
#define 	MIN12V	(CNT_CON+0x60)			//���˿�����,��Ӧ��ֵ�����
#define 	MAX12V	(CNT_OPN+0x60)			//���˿�����,��Ӧ��ֵ�����


#define 	ADC0V					1
#define		ADC4V					2
#define		ADC6V					3
#define		ADC9V					4
#define		ADC12V				5

extern uint32_t ADstatus;
extern void ADCInit( uint32_t ADC_Clk );



extern void ADCScan(void);






























#endif
