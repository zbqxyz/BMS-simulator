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
  LPC_SC->PCONP |= (0x1<<12);       /* ʹ��ADC���ʿ�����                     */
  LPC_IOCON->P1_31 &= ~0x9F;
  LPC_IOCON->P1_31 |= 0x03;         /* ADC IN5                               */

  LPC_ADC->CR = ( 0x01 << ADC_CHN ) |               /* ѡ��ADC[5]ΪADC����                   */
                ((PeripheralClock/ADC_Clk-1)<<8) |  /* CLKDIV = Fpclk / ADC_Clk - 1  */ 
                ( 0 << 16 ) |                       /* BURST = 0, �������ת��               */
                ( 1 << 21 ) |                       /* PDN = 1,  ��������ģʽ                */
                ( 0 << 24 ) |                       /* START = 0  ������ADת��               */
                ( 0 << 27 );                        /* EDGE = 0  ����������                  */ 

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
 * channelNum����ADC���ͨ���� ������channelNum 
 */
  if ( channelNum >= ADC_NUM )
	{      
    channelNum = 0;
    return 0xFFFFFF;                 /* ����һ������ADC�������ֵ����         */     
  }

  LPC_ADC->CR &= 0xF8FFFFFF;         /* ֹͣת��                             */
  regVal = LPC_ADC->DR[channelNum];  /* �����ݼĴ���                         */

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
** Descriptions:        ��ȡA/Dת����״̬
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
** Descriptions:        ����ADCת��
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
                   (1UL<<24);      /* START = 1  ����ADת��                 */
}

/*****************************************************************************
** Function name:       ADCScan
**
** Descriptions:       ADCת��
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
				ADC_Start(ADC_CHN);                                         /* ��A/Dת��                    	*/
				while(ADC_GetStat(ADC_CHN));                                /* �ȴ�ת�����                   	*/
				    if(i==0)
						continue;
				ADCVal=ADCRead(ADC_CHN); 																		/* ��ȡ����ֵ                  			*/
				ADCSum+=ADCVal;
		}
		
		ADCSum=(ADCSum/ADC_TIMES);                                    		/* ����ADC_TIM�ν����ǲ�����     		*/			
				
		if(ADCSum<MIN6V)
			ADstatus = ADC0V;																							//�쳣
		else if(ADCSum<MAX6V)
			ADstatus = ADC6V;																							//����					
		else if(ADCSum<MAX9V)
			ADstatus = ADC9V;																							//׼����
		else if(ADCSum<MAX12V)
			ADstatus = ADC12V;																						//����
		else						
			ADstatus = ADC12V;																						//����				
}
