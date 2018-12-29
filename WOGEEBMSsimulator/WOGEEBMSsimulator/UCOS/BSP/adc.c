/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                    http://www.tomorn.cn
**                                        wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               adc.c
** Last modified Date:      2017.05.18
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <adc.h>

uint32_t ADstatus;

/*****************************************************************************
** Function name:       ADCInit
**
** Descriptions:       
**
** parameters:          ADC_Clk
** Returned value:      
** 
*****************************************************************************/
void ADCInit( uint32_t ADC_Clk )
{
  LPC_SC->PCONP |= (0x1<<12);       /* 使能ADC功率控制器                     */
  LPC_IOCON->P1_31 &= ~0x9F;
  LPC_IOCON->P1_31 |= 0x03;         /* ADC IN5                               */

  LPC_ADC->CR = ( 0x01 << ADC_CHN ) |               /* 选则ADC[5]为ADC输入                   */
                ((PeripheralClock/ADC_Clk-1)<<8) |  /* CLKDIV = Fpclk / ADC_Clk - 1  */ 
                ( 0 << 16 ) |                       /* BURST = 0, 软件控制转换               */
                ( 1 << 21 ) |                       /* PDN = 1,  处与正常模式                */
                ( 0 << 24 ) |                       /* START = 0  不启动AD转换               */
                ( 0 << 27 );                        /* EDGE = 0  上升沿启动                  */ 

  return;
}
/*****************************************************************************
** Function name:       ADCRead
**
** Descriptions:        Read ADC channel
**
** parameters:          Channel number
** Returned value:      Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead( uint8_t channelNum )
{
  uint32_t regVal, ADC_Data = 0;
/* 
 * channelNum大于ADC最大通道号 则清零channelNum 
 */
  if ( channelNum >= ADC_NUM )
	{      
    channelNum = 0;
    return 0xFFFFFF;                 /* 返回一个大于ADC采样最大值的数         */     
  }

  LPC_ADC->CR &= 0xF8FFFFFF;         /* 停止转换                             */
  regVal = LPC_ADC->DR[channelNum];  /* 读数据寄存器                         */

  if ( regVal & ADC_OVERRUN ) 
	{
	return ( 0xFFFFFF );
  }
   
#if ADC_12BITS
  ADC_Data = ( regVal >> 4 ) & 0xFFF;
#else
  ADC_Data = ( regVal >> 6 ) & 0x3FF;
#endif

  return ( ADC_Data );
}


/*****************************************************************************
** Function name:       ADC_GetStat
**
** Descriptions:        获取A/D转换的状态
**
** parameters:          ChannelNum
** Returned value:      
** 
*****************************************************************************/
uint32_t ADC_GetStat( uint8_t channelNum )
{
    uint32_t stat;

    if( channelNum >= ADC_NUM) 
		{
        channelNum = 0; 
    }
    stat = ( ( LPC_ADC->DR[channelNum] ) &ADC_DONE ) >>31;
    return (stat);
}
/*****************************************************************************
** Function name:       ADC_Start
**
** Descriptions:        启动ADC转换
**
** parameters:          ChannelNum
** Returned value:      Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
void ADC_Start( uint8_t channelNum )
{
    if( channelNum >= ADC_NUM)
		{
        channelNum = 0; 
    }
    LPC_ADC->CR |= (1<<channelNum)|
                   (1UL<<24);      /* START = 1  启动AD转换                 */
}

/*****************************************************************************
** Function name:       ADCScan
**
** Descriptions:       ADC转换
**
** parameters:          
** Returned value:      
** 
*****************************************************************************/

void ADCScan(void)
{	
		uint32_t i;
		uint32_t ADCSum,ADCVal;
		
		ADCSum=0;
	
		for(i=0;i<ADC_TIMES+1;i++)
	  {							  
				ADC_Start(ADC_CHN);                                         /* 打开A/D转换                    	*/
				while(ADC_GetStat(ADC_CHN));                                /* 等待转换完成                   	*/
				    if(i==0)
						continue;
				ADCVal=ADCRead(ADC_CHN); 																		/* 获取采样值                  			*/
				ADCSum+=ADCVal;
		}
		
		ADCSum=(ADCSum/ADC_TIMES);                                    		/* 采样ADC_TIM次进行虑波处理     		*/			
				
		if(ADCSum<MIN6V)
			ADstatus = ADC0V;																							//异常
		else if(ADCSum<MAX6V)
			ADstatus = ADC6V;																							//工作					
		else if(ADCSum<MAX9V)
			ADstatus = ADC9V;																							//准备好
		else if(ADCSum<MAX12V)
			ADstatus = ADC12V;																						//待机
		else						
			ADstatus = ADC12V;																						//待机				
}
