/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                    http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               ssp.c
** Last modified Date:      2017.05.18
** Last Version:            1.0
** Description:             
*********************************************************************************************************/

#include <ssp.h>

/*********************************************************************************************************
** Function name:		SSP0_Init
** Descriptions:		将SSP控制器设置为主机
** input parameters:	无
** output parameters:	无
**********************************************************************************************************/
void  SSP0_Init(void)
{
  uint8_t uiRegVal = 0;
	
  LPC_SC->PCONP |= (1ul << 21);                                   /* 开启SSP0外设                 */

	/* 
	 * 设定外设分频值
	 */
	uiRegVal = LPC_SC->PCLKSEL;
	if (uiRegVal < 4) {
		//	LPC_SC->PCLKSEL = 4;
	}
	
	LPC_IOCON->P0_15 &= ~0x07;
	LPC_IOCON->P0_15 |=  0x02;                                      /* SSP CLK                      */
	LPC_IOCON->P0_17 &= ~0x07;
	LPC_IOCON->P0_17 |=  0x02;                                      /* SSP MISO                     */
	LPC_IOCON->P0_18 &= ~0x07;    
	LPC_IOCON->P0_18 |=  0x02;                                      /* SSP MOSI                     */
	
    LPC_SSP0->CR0  = 0x0807;													          	/* 设置数据长度为8位，帧格式SPI，SCK高有效*/
    LPC_SSP0->CPSR = 0x08;			    		                          /* 设置SSP从PCLK获得的分频      */
                                                                  /* 值,主模式下有效,             */
                                                                  /* 最小值为0x02	                */
    LPC_SSP0->CR1  = 0x02;		   			                            /* 设置SSP为主机模式,           */
         		   			                     
}
/*********************************************************************************************************
** Function name:		SSP0_SendData
** Descriptions:		SSP接口向SSP总线发送数据
** input parameters:	data        待发送的数据
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
void SSP0_SendData(uint8_t data)
{  
    while ((LPC_SSP0->SR & 0x01) == 0);	    	                            /* 等待发送FIFO留出空间       */  
	        	LPC_SSP0->DR = data;	 
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 	                            /* 等待数据帧发送完毕	        */ 
}
/*********************************************************************************************************
** Function name:		SSP0_SendData
** Descriptions:		SSP接收数据
** input parameters:	
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
uint8_t  SSP0_ReadData(void)
{  
    uint8_t tmp ;
    while ((LPC_SSP0->SR & 0x04) == 0x04)
    {
    	tmp = LPC_SSP0->DR ;
    }	    				                    						                     	/* 清空接收FIFO留出空间   	*/    
    while ((LPC_SSP0->SR & 0x01) == 0);	    				                    /* 清空发送FIFO留出空间   	*/
    	LPC_SSP0->DR = 0xff ;
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 				                    /* 等待数据帧发送完毕		    */
    while ((LPC_SSP0->SR & 0x04) == 0x00);	 				                    /* 等待数据帧接收完毕	    	*/		
	   tmp = LPC_SSP0->DR ;
     return tmp;
}



/*********************************************************************************************************
** Function name:		SSP1_Init
** Descriptions:		将SSP控制器设置为主机
** input parameters:	无
** output parameters:	无
**********************************************************************************************************/
void  SSP1_Init(void)
{
    uint8_t uiRegVal = 0;

    LPC_SC->PCONP |= (1ul<<10);                                     /* 开启SSP1外设                 */

    /* 
     * 设定外设分频值
     */
    uiRegVal = LPC_SC->PCLKSEL;
    if ( uiRegVal < 4 ) {
       // LPC_SC->PCLKSEL = 4;
    }
	
		LPC_IOCON->P1_19&= ~0x07;
		LPC_IOCON->P1_19 |=  0x05;                                       /* SSP CLK                      */
 		LPC_IOCON->P1_26 &= ~0x07;    
 		LPC_IOCON->P1_26 |=  0x05;                                       /* SSP SSEL                     */
		LPC_IOCON->P1_18 &= ~0x07;
		LPC_IOCON->P1_18 |=  0x05;                                       /* SSP MISO                     */
		LPC_IOCON->P1_22 &= ~0x07;    
		LPC_IOCON->P1_22 |=  0x05;                                       /* SSP MOSI                     */
		
	  LPC_SSP1->CR0  = 0x0807; 				/* 设置数据长度为8位，帧格式SPI，SCK高有效，第一个时钟沿采样，位速率为默认值 	*/
		LPC_SSP1->CPSR = 0x04;			    /* 设置SSP从PCLK获得的分频值,主模式下有效, 最小值为0x02 */
		LPC_SSP1->CR1  = 0x02;		   		/* 设置SSP为主机模式,       */	 
}

/*********************************************************************************************************
** Function name:		SSP1_SendData
** Descriptions:		SSP接口向SSP总线发送数据
** input parameters:	data        待发送的数据
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
uint16_t  SSP1_SendData(uint8_t data)
{  
    while ((LPC_SSP1->SR & 0x02) == 0);	    	                            /* 等待发送FIFO留出空间         */   
		LPC_SSP1->DR = data;
    while ((LPC_SSP1->SR & 0x10) == 0x10);	 	                            /* 等待数据帧发送完毕	        */ 
    return (LPC_SSP1->DR);
}

/*********************************************************************************************************
** Function name:		SSP1_SendData
** Descriptions:		SSP接收数据
** input parameters:	
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
uint8_t  SSP1_ReadData(void )
{  
	uint8_t tmp ;	
	while ((LPC_SSP1->SR & 0x04) == 0x04)
		tmp = LPC_SSP1->DR ;																			/* 清空接收FIFO留出空间   	*/   				                    							  
  while ((LPC_SSP1->SR & 0x01) == 0);	    				            /* 清空发送FIFO留出空间   	*/
	  LPC_SSP1->DR = 0xff ;
  while ((LPC_SSP1->SR & 0x10) == 0x10);	 				            /* 等待数据帧发送完毕	    	*/
  while ((LPC_SSP1->SR & 0x04) == 0x00);	 				            /* 等待数据帧接收完毕		    */
	  tmp = LPC_SSP1->DR ;
  return tmp;

}



/*********************************************************************************************************
** Function name:        SSP2_Init
** Descriptions:       	 SSP2主机例程               
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void  SSP2_Init(void)
{
  uint8_t uiRegVal = 0;
	
  LPC_SC->PCONP |= (1ul << 20);                             	/* 开启SSP2外设                 */

	uiRegVal = LPC_SC->PCLKSEL;																	//设定外设分频值
	if (uiRegVal < 4) {
			//LPC_SC->PCLKSEL = 4;
	}
	
	LPC_IOCON->P1_0 &= ~0x07;
	LPC_IOCON->P1_0 |=  0x04;                                   /* SSP CLK                      */
	LPC_IOCON->P1_1 &= ~0x07;
	LPC_IOCON->P1_1 |=  0x04;                                   /* SSP MISO                     */
	LPC_IOCON->P1_4 &= ~0x07;    
	LPC_IOCON->P1_4 |=  0x04;                                   /* SSP MOSI                     */

	LPC_SSP2->CR0  = 0x0807;																		/* 设置数据长度为8位，帧格式SPI，SCK高有效*/
	LPC_SSP2->CPSR = 0x08;			    		                        /* 设置SSP从PCLK获得的分频  */
																															/* 值,主模式下有效,         */
																															/* 最小值为0x02	            */
	LPC_SSP2->CR1  = 0x02;		   			                          /* 设置SSP为主机模式,       */  /* 主模式下有效, 最小值为0x02   */
}
/*********************************************************************************************************
** Function name:		SSP2_SendData
** Descriptions:		SSP接口向SSP总线发送数据
** input parameters:	data        待发送的数据
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
void  SSP2_SendData(uint8_t data)
{  
	while (!(LPC_SSP2->SR & 0x01));	    				                /* 等待发送FIFO留出空间   	*/
	LPC_SSP2->DR = data;
	while ((LPC_SSP2->SR & 0x10) == 0x10);	 				            /* 等待数据帧发送完毕		    */
}
/*********************************************************************************************************
** Function name:		SSP2_SendData
** Descriptions:		SSP接收数据
** input parameters:	
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
uint8_t  SSP2_ReadData(void )
{  
	uint8_t tmp ;	
	while ((LPC_SSP2->SR & 0x04) == 0x04)
		tmp = LPC_SSP2->DR ;																			/* 清空接收FIFO留出空间   	*/   				                    							  
  while ((LPC_SSP2->SR & 0x01) == 0);	    				            /* 清空发送FIFO留出空间   	*/
	  LPC_SSP2->DR = 0xff ;
  while ((LPC_SSP2->SR & 0x10) == 0x10);	 				            /* 等待数据帧发送完毕	    	*/
  while ((LPC_SSP2->SR & 0x04) == 0x00);	 				            /* 等待数据帧接收完毕		    */
	  tmp = LPC_SSP2->DR ;
  return tmp;
}
