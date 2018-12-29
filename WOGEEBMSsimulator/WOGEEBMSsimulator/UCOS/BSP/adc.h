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


#define ADC_CHN				  5		    /*	转换用通道	  */
#define ADC_TIMES				100	  	/*	采样次数	  */


#define 	CNT_OPN	 0x2C0	 						//充电端口悬空,对应阻值无穷大
#define 	CNT_ETH	 0x000				 			//充电端口接地,对应阻值0
#define 	CNT_CON	 0x200	 						//充电端口连接充电枪（汽车端S2未关闭,对应分压值2740欧）
#define 	CNT_WRK	 0x140				 			//检测端口充电机准备好（汽车端S2关闭,对应分压值883欧）
#define 	MIN6V	 	(CNT_WRK-0x60)			//开启充电CP电压最小值
#define 	MAX6V	 	(CNT_WRK+0x60)			//开启充电CP电压最大值
#define 	MIN9V	 	(CNT_CON-0x60)	 		//检测端口连接（汽车端S2未关闭,对应分压值2740欧）
#define 	MAX9V	 	(CNT_CON+0x60)	 		//检测端口连接（汽车端S2未关闭,对应分压值2740欧）
#define 	MIN12V	(CNT_CON+0x60)			//检测端口悬空,对应阻值无穷大
#define 	MAX12V	(CNT_OPN+0x60)			//检测端口悬空,对应阻值无穷大


#define 	ADC0V					1
#define		ADC4V					2
#define		ADC6V					3
#define		ADC9V					4
#define		ADC12V				5

extern uint32_t ADstatus;
extern void ADCInit( uint32_t ADC_Clk );



extern void ADCScan(void);






























#endif
