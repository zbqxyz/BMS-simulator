/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               uart.c
** Last modified Date:      2017.05.20
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <uart.h>
/*********************************************************************************************************
  
*********************************************************************************************************/
uint8_t               RcvBufU0[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               SndBufU0[UARTBUFLEN];                                      /*  串口发送数据缓冲区          */
uint32_t              GulNumU0;                                             		 /*  串口接收数据的个数          */
uint32_t              RcvLenU0;                                                  /*  串口接收数据的长度          */
uint32_t              SndLenU0;                                                  /*  串口接收数据的长度          */
uint8_t         			RcvNewU0;                                          		     /*  串口接收新数据的标志        */

/*********************************************************************************************************
** Function name:	    UART0Init
** Descriptions:	    UART0初始化，设置为8位数据位，1位停止位，无校验，波特率为UART0_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART0Init (void)
{
    uint32_t Fdiv;

		LPC_SC->PCONP |= (1<<3);
    
    LPC_IOCON->P0_2  &= ~0x07;                                      /* 将IOCON功能选择寄存器低三位清零    */
    LPC_IOCON->P0_2  |= 1;                                          /* 将IOCON 置为选择的功能       */
    LPC_IOCON->P0_3  &= ~0x07;                                      /* 将IOCON功能选择寄存器低三位清零    */
    LPC_IOCON->P0_3  |= 1;                                          /* 将IOCON 置为选择的功能       */
    
    LPC_UART0->LCR = 0x83;                                          /* 选择校验位                   */
    Fdiv = ( PeripheralClock / 16 ) / (UART0_BPS) ;                 /*  配置时钟                    */
    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;                                         
    LPC_UART0->FCR = 0x87;                                      

    NVIC_EnableIRQ(UART0_IRQn);
  
    LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;
}

/*********************************************************************************************************
** Function name:	    UART0SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART0SendByte (uint8_t ucDat)
{
    LPC_UART0->THR = ucDat;                                                 /* 写入数据                     */
    while ((LPC_UART0->LSR & 0x20) == 0);                                		/* 等待数据发送完毕             */
}

/*********************************************************************************************************
** Function name:	    UART0SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART0SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++)                                             /* 发送指定个字节数据           */
		{                                      
        UART0SendByte (*pucStr++);
    }
}
/*********************************************************************************************************
** Function name:	    UART0RxbufClear
** Descriptions:    	清空接受缓存
** input parameters:    
**                      
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART0RxbufClear(void )
{
		if(RcvNewU0!=0||RcvLenU0!=0){
			RcvNewU0 = 0;                                                       		/* 清除标*/
			RcvLenU0 = 0;
			memset(RcvBufU0,0,sizeof(RcvBufU0));
		}
}

/*********************************************************************************************************
* Function Name  : UART0_IRQHandler
* Description    : UART0 中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART0_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART0->IIR;
    uiIIRValue >>= 1;                                              /* 中断标志寄存器值右移1位    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* 接收线状态中断             */
	 {                                       
        uiLSRValue = LPC_UART0->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART0->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* 接收FIFO不为空中断         */
				{                                   
            for (GulNumU0 = 0; GulNumU0 < 8; GulNumU0++)           /*  连续接收8个字节             */
					 {               
							if ( GulNumU0+RcvLenU0 >= UARTBUFLEN ) 
							{
									UART0RxbufClear();
							}
							RcvBufU0[GulNumU0+RcvLenU0] = LPC_UART0->RBR;
            }
						RcvLenU0+=GulNumU0;
						RcvNewU0 = 1; 					
        }
    }
	 
    else if ( uiIIRValue == IIR_RDA )                          /* 接收数据可用中断           */
	 {                                   
            for (GulNumU0 = 0; GulNumU0 < 8; GulNumU0++)       /*  连续接收8个字节             */
		        {                 
							if ( GulNumU0+RcvLenU0 >= UARTBUFLEN )
							{
									UART0RxbufClear();
							}
							RcvBufU0[GulNumU0+RcvLenU0] = LPC_UART0->RBR;
            }
						RcvLenU0+=GulNumU0;
						RcvNewU0 = 1; 										
    }
	 
    else if ( uiIIRValue == IIR_CTI )                          /* 字符超时中断               */
		{                                   
          while ((LPC_UART0->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU0 >= UARTBUFLEN)
						{
							UART0RxbufClear();
						}                       								           /*  判断数据是否接收完毕       */ 
				    RcvBufU0[RcvLenU0] = LPC_UART0->RBR;
            RcvLenU0++;
					}
				  RcvNewU0 = 1; 											                 /* 	接收到完整数据帧 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* 发送保持寄存器空中断        */
		{                                 
        uiLSRValue = LPC_UART0->LSR;
        if ( uiLSRValue & LSR_THRE )
				{
            //UART0TxEmpty = 1;
        } else 
				{
            //UART0TxEmpty = 0;
        }
    }
}


/*****************************************************************************************************************/
uint8_t               RcvBufU1[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               SndBufU1[UARTBUFLEN];                                      /*  串口发送数据缓冲区          */
uint32_t              GulNumU1;                                             		 /*  串口接收数据的个数          */
uint32_t              RcvLenU1;                                                  /*  串口接收数据的长度          */
uint32_t              SndLenU1;                                                  /*  串口接收数据的长度          */
uint8_t         			RcvNewU1;                                          		     /*  串口接收新数据的标志        */

/*********************************************************************************************************
** Function name:	    UART1Init
** Descriptions:	    UART1初始化，设置为8位数据位，1位停止位，无校验，波特率为UART1_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1Init (void)
{
    uint32_t Fdiv;

		LPC_SC->PCONP |= (1<<4);
    
    LPC_IOCON->P2_0  &= ~0x07;
    LPC_IOCON->P2_0  |= 2;                                       
    LPC_IOCON->P2_1  &= ~0x07;
    LPC_IOCON->P2_1  |= 2;                                          
    
    LPC_UART1->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) / (UART1_BPS) ;              
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                        
    LPC_UART1->FCR = 0x87;                                       

    NVIC_EnableIRQ(UART1_IRQn);
  
    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	
	
	
}

/*********************************************************************************************************
** Function name:	    UART1SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1SendByte (uint8_t ucDat)
{
    LPC_UART1->THR = ucDat;                                                 /* 写入数据                     */
    while ((LPC_UART1->LSR & 0x20) == 0);                                		/* 等待数据发送完毕             */
}


/*********************************************************************************************************
** Function name:	    UART1SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;

    for (i = 0; i < ulNum; i++)                   /* 发送指定个字节数据           */
		{                                      
        UART1SendByte (*pucStr++);
    }
			
}
/*********************************************************************************************************
** Function name:	    UART1RxbufClear
** Descriptions:    	清空接受缓存
** input parameters:    
**                      
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1RxbufClear(void )
{
		if(RcvNewU1!=0||RcvLenU1!=0){
			RcvNewU1 = 0;                                              		/* 清除标志*/
			RcvLenU1 = 0;
			memset(RcvBufU1,0,sizeof(RcvBufU1));
		}
}

/*********************************************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : UART1中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART1_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART1->IIR;
    uiIIRValue >>= 1;                                              /* 中断标志寄存器值右移1位    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* 接收线状态中断             */
	 {                                       
        uiLSRValue = LPC_UART1->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART1->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* 接收FIFO不为空中断         */
				{                                   
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++)           /*  连续接收8个字节             */
					 {               
							if ( GulNumU1+RcvLenU1 >= UARTBUFLEN ) 
							{
									UART1RxbufClear();
							}
							RcvBufU1[GulNumU1+RcvLenU1] = LPC_UART1->RBR;
            }
						RcvLenU1+=GulNumU1;
						RcvNewU1 = 1; 					
        }
    }
	 
    else if ( uiIIRValue == IIR_RDA )                          /* 接收数据可用中断           */
	 {                                   
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++)       /*  连续接收8个字节             */
		        {                 
							if ( GulNumU1+RcvLenU1 >= UARTBUFLEN )
							{
									UART1RxbufClear();
							}
							RcvBufU1[GulNumU1+RcvLenU1] = LPC_UART1->RBR;
            }
						RcvLenU1+=GulNumU1;
						RcvNewU1 = 1; 										
    }
	 
    else if ( uiIIRValue == IIR_CTI )                          /* 字符超时中断               */
		{                                   
          while ((LPC_UART1->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU1 >= UARTBUFLEN)
						{
							UART1RxbufClear();
						}                       								           /*  判断数据是否接收完毕       */ 
				    RcvBufU1[RcvLenU1] = LPC_UART1->RBR;
            RcvLenU1++;
					}
				  RcvNewU1 = 1; 											                 /* 	接收到完整数据帧 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* 发送保持寄存器空中断        */
		{                                 
        uiLSRValue = LPC_UART1->LSR;
        if ( uiLSRValue & LSR_THRE )
				{
            //UART1TxEmpty = 1;
        } else 
				{
            //UART1TxEmpty = 0;
        }
    }
}


/*****************************************************************************************************************/
uint8_t               RcvBufU2[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               SndBufU2[UARTBUFLEN];                                      /*  串口发送数据缓冲区          */
uint32_t              GulNumU2;                                             		 /*  串口接收数据的个数          */
uint32_t              RcvLenU2;                                                  /*  串口接收数据的长度          */
uint32_t              SndLenU2;                                                  /*  串口接收数据的长度          */
uint8_t         			RcvNewU2;                                          		     /*  串口接收新数据的标志        */

/*********************************************************************************************************
** Function name:	    UART2Init
** Descriptions:	    UART3初始化，设置为8位数据位，1位停止位，无校验，波特率为UART2_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART2Init (void)
{
    uint32_t Fdiv;

		LPC_SC->PCONP |= (1<<24);
    
    LPC_IOCON->P0_10  &= ~0x07;
    LPC_IOCON->P0_10  |= 1;                                         
    LPC_IOCON->P0_11  &= ~0x07;
    LPC_IOCON->P0_11  |= 1;                                         
    
    LPC_UART2->LCR = 0x9b;
    Fdiv = ( PeripheralClock / 16 ) / (UART2_BPS) ;              
    LPC_UART2->DLM = Fdiv / 256;
    LPC_UART2->DLL = Fdiv % 256;
    LPC_UART2->LCR = 0x1b;                                         
    LPC_UART2->FCR = 0x87;                                         

    NVIC_EnableIRQ(UART2_IRQn);
  
    LPC_UART2->IER = IER_RBR | IER_THRE | IER_RLS;
    ENRD485_U1();
}

/*********************************************************************************************************
** Function name:	    UART2SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART2SendByte (uint8_t ucDat)
{
    LPC_UART2->THR = ucDat;                                                 /* 写入数据                     */
    while ((LPC_UART2->LSR & 0x20) == 0);                                		/* 等待数据发送完毕             */

}
/*********************************************************************************************************
** Function name:	    UART2SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART2SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
	 
    uint32_t i;
	  ENWR485_U1();
    for (i = 0; i < ulNum; i++)                   /* 发送指定个字节数据           */
		{                                      
        UART2SendByte (*pucStr++);
    }
		OSTimeDlyHMSM(0,0,0,10);		                  /*等待足够的时间使发送彻底完成  */
		ENRD485_U1();	
}
/*********************************************************************************************************
** Function name:	    UART2RxbufClear
** Descriptions:    	清空接受缓存
** input parameters:    
**                      
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART2RxbufClear(void )
{
		if(RcvNewU2!=0||RcvLenU2!=0){
			RcvNewU2 = 0;                                              		/* 清除标志*/
			RcvLenU2 = 0;
			memset(RcvBufU2,0,sizeof(RcvBufU2));
		}
}

/*********************************************************************************************************
* Function Name  : UART0_IRQHandler
* Description    : UART0 中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART2_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART2->IIR;
    uiIIRValue >>= 1;                                              /* 中断标志寄存器值右移1位    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* 接收线状态中断             */
	 {                                       
        uiLSRValue = LPC_UART2->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART2->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* 接收FIFO不为空中断         */
				{                                   
            for (GulNumU2 = 0; GulNumU2 < 8; GulNumU2++)           /*  连续接收8个字节             */
					 {               
							if ( GulNumU2+RcvLenU2 >= UARTBUFLEN ) 
							{
									UART2RxbufClear();
							}
							RcvBufU2[GulNumU2+RcvLenU2] = LPC_UART2->RBR;
            }
						RcvLenU2+=GulNumU2;
						RcvNewU2 = 1; 					
        }
    }
	 
    else if ( uiIIRValue == IIR_RDA )                          /* 接收数据可用中断           */
	 {                                   
            for (GulNumU2 = 0; GulNumU2 < 8; GulNumU2++)       /*  连续接收8个字节             */
		        {                 
							if ( GulNumU2+RcvLenU2 >= UARTBUFLEN )
							{
									UART2RxbufClear();
							}
							RcvBufU2[GulNumU2+RcvLenU2] = LPC_UART2->RBR;
            }
						RcvLenU2+=GulNumU2;
						RcvNewU2 = 1; 										
    }
	 
    else if ( uiIIRValue == IIR_CTI )                          /* 字符超时中断               */
		{                                   
          while ((LPC_UART2->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU2 >= UARTBUFLEN)
						{
							UART2RxbufClear();
						}                       								           /*  判断数据是否接收完毕       */ 
				    RcvBufU2[RcvLenU2] = LPC_UART2->RBR;
            RcvLenU2++;
					}
				  RcvNewU2 = 1; 											                 /* 	接收到完整数据帧 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* 发送保持寄存器空中断        */
		{                                 
        uiLSRValue = LPC_UART2->LSR;
        if ( uiLSRValue & LSR_THRE )
				{
            //UART2TxEmpty = 1;
        } else 
				{
            //UART2TxEmpty = 0;
        }
    }
}

/*****************************************************************************************************************/
uint8_t               RcvBufU3[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               SndBufU3[UARTBUFLEN];                                      /*  串口发送数据缓冲区          */
uint32_t              GulNumU3;                                             		 /*  串口接收数据的个数          */
uint32_t              RcvLenU3;                                                  /*  串口接收数据的长度          */
uint32_t              SndLenU3;                                                  /*  串口接收数据的长度          */
uint8_t         			RcvNewU3;                                          		     /*  串口接收新数据的标志        */

/*********************************************************************************************************
** Function name:	    UART3Init
** Descriptions:	    UART3初始化，设置为8位数据位，1位停止位，无校验，波特率为UART3_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART3Init (void)
{
    uint32_t Fdiv;

		LPC_SC->PCONP |= (1<<25);
    
    LPC_IOCON->P4_28  &= ~0x07;
    LPC_IOCON->P4_28 |=  2;                                       
    LPC_IOCON->P4_29  &= ~0x07;
    LPC_IOCON->P4_29  |= 2;                                         
    
    LPC_UART3->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) / (UART3_BPS) ;              
    LPC_UART3->DLM = Fdiv / 256;
    LPC_UART3->DLL = Fdiv % 256;
    LPC_UART3->LCR = 0x03;                                        
    LPC_UART3->FCR = 0x87;                                         

    NVIC_EnableIRQ(UART3_IRQn);
  
    LPC_UART3->IER = IER_RBR | IER_THRE | IER_RLS;
	
}

/*********************************************************************************************************
** Function name:	    UART3SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART3SendByte (uint8_t ucDat)
{
    LPC_UART3->THR = ucDat;                                                      /* 写入数据                     */
    while ((LPC_UART3->LSR & 0x20) == 0);                                		/* 等待数据发送完毕             */
}


/*********************************************************************************************************
** Function name:	    UART3SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART3SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++)                   /* 发送指定个字节数据           */
		{                                      
        UART3SendByte (*pucStr++);
    }
}
/*********************************************************************************************************
** Function name:	    UART3RxbufClear
** Descriptions:    	清空接受缓存
** input parameters:    
**                      
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART3RxbufClear(void )
{
		if(RcvNewU3!=0||RcvLenU3!=0){
			RcvNewU3 = 0;                                              		/* 清除标志*/
			RcvLenU3 = 0;
			memset(RcvBufU3,0,sizeof(RcvBufU3));
		}
}

/*********************************************************************************************************
* Function Name  : UART3_IRQHandler
* Description    : UART3 中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART3_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART3->IIR;
    uiIIRValue >>= 1;                                              /* 中断标志寄存器值右移1位    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* 接收线状态中断             */
	 {                                       
        uiLSRValue = LPC_UART3->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART3->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* 接收FIFO不为空中断         */
				{                                   
            for (GulNumU3 = 0; GulNumU3 < 8; GulNumU3++)           /*  连续接收8个字节             */
					 {               
							if ( GulNumU3+RcvLenU3 >= UARTBUFLEN ) 
							{
									UART3RxbufClear();
							}
							RcvBufU3[GulNumU3+RcvLenU3] = LPC_UART3->RBR;
            }
						RcvLenU3+=GulNumU3;
						RcvNewU3 = 1; 					
        }
    }
	 
    else if ( uiIIRValue == IIR_RDA )                          /* 接收数据可用中断           */
	 {                                   
            for (GulNumU3 = 0; GulNumU3 < 8; GulNumU3++)       /*  连续接收8个字节             */
		        {                 
							if ( GulNumU3+RcvLenU3 >= UARTBUFLEN )
							{
									UART3RxbufClear();
							}
							RcvBufU3[GulNumU3+RcvLenU3] = LPC_UART3->RBR;
            }
						RcvLenU3+=GulNumU3;
						RcvNewU3 = 1; 										
    }
	 
    else if ( uiIIRValue == IIR_CTI )                          /* 字符超时中断               */
		{                                   
          while ((LPC_UART3->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU3 >= UARTBUFLEN)
						{
							UART3RxbufClear();
						}                       								           /*  判断数据是否接收完毕       */ 
				    RcvBufU3[RcvLenU3] = LPC_UART3->RBR;
            RcvLenU3++;
					}
				  RcvNewU3 = 1; 											                 /* 	接收到完整数据帧 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* 发送保持寄存器空中断        */
		{                                 
        uiLSRValue = LPC_UART3->LSR;
        if ( uiLSRValue & LSR_THRE )
				{
            //UART3TxEmpty = 1;
        } else 
				{
            //UART3TxEmpty = 0;
        }
    }
}


/*****************************************************************************************************************/
uint8_t               RcvBufU4[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               SndBufU4[UARTBUFLEN];                                      /*  串口发送数据缓冲区          */
uint32_t              GulNumU4;                                             		 /*  串口接收数据的个数          */
uint32_t              RcvLenU4;                                                  /*  串口接收数据的长度          */
uint32_t              SndLenU4;                                                  /*  串口接收数据的长度          */
uint8_t         			RcvNewU4;                                          		     /*  串口接收新数据的标志        */

/*********************************************************************************************************
** Function name:	    UART4Init
** Descriptions:	    UART4初始化，设置为8位数据位，1位停止位，无校验，波特率为UART3_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART4Init (void)
{
    uint32_t Fdiv;

		LPC_SC->PCONP |= (1<<8);
    
    LPC_IOCON->P5_3  &= ~0x07;
    LPC_IOCON->P5_3  |= 4;                                         
    LPC_IOCON->P5_4  &= ~0x07;
    LPC_IOCON->P5_4  |= 4;                                         
    
    LPC_UART4->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) / (UART4_BPS) ;              
    LPC_UART4->DLM = Fdiv / 256;
    LPC_UART4->DLL = Fdiv % 256;
    LPC_UART4->LCR = 0x03;                                         
    LPC_UART4->FCR = 0x87;                                         

    NVIC_EnableIRQ(UART4_IRQn);
  
    LPC_UART4->IER = IER_RBR | IER_THRE | IER_RLS;
	
	
}

/*********************************************************************************************************
** Function name:	    UART4SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART4SendByte (uint8_t ucDat)
{
    LPC_UART4->THR = ucDat;                                                      /* 写入数据                     */
    while ((LPC_UART4->LSR & 0x20) == 0);                                		/* 等待数据发送完毕             */
}


/*********************************************************************************************************
** Function name:	    UART4SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART4SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;

    for (i = 0; i < ulNum; i++)                   /* 发送指定个字节数据           */
		{                                      
        UART4SendByte (*pucStr++);
    }

}
/*********************************************************************************************************
** Function name:	    UART4RxbufClear
** Descriptions:    	清空接受缓存
** input parameters:    
**                      
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART4RxbufClear(void )
{
		if(RcvNewU4!=0||RcvLenU4!=0){
			RcvNewU4 = 0;                                              		/* 清除标志*/
			RcvLenU4 = 0;
			memset(RcvBufU4,0,sizeof(RcvBufU4));
		}
}

/*********************************************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : UART4 中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART4_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART4->IIR;
    uiIIRValue >>= 1;                                              /* 中断标志寄存器值右移1位    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* 接收线状态中断             */
	 {                                       
        uiLSRValue = LPC_UART4->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART4->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* 接收FIFO不为空中断         */
				{                                   
            for (GulNumU4 = 0; GulNumU4 < 8; GulNumU4++)           /*  连续接收8个字节             */
					 {               
							if ( GulNumU4+RcvLenU4 >= UARTBUFLEN ) 
							{
									UART4RxbufClear();
							}
							RcvBufU4[GulNumU4+RcvLenU4] = LPC_UART4->RBR;
            }
						RcvLenU4+=GulNumU4;
						RcvNewU4 = 1; 					
        }
    }
	 
    else if ( uiIIRValue == IIR_RDA )                          /* 接收数据可用中断           */
	 {                                   
            for (GulNumU4 = 0; GulNumU4 < 8; GulNumU4++)       /*  连续接收8个字节             */
		        {                 
							if ( GulNumU4+RcvLenU4 >= UARTBUFLEN )
							{
									UART4RxbufClear();
							}
							RcvBufU4[GulNumU4+RcvLenU4] = LPC_UART4->RBR;
            }
						RcvLenU4+=GulNumU4;
						RcvNewU4 = 1; 										
    }
	 
    else if ( uiIIRValue == IIR_CTI )                          /* 字符超时中断               */
		{                                   
          while ((LPC_UART4->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU4 >= UARTBUFLEN)
						{
							UART4RxbufClear();
						}                       								           /*  判断数据是否接收完毕       */ 
				    RcvBufU4[RcvLenU4] = LPC_UART4->RBR;
            RcvLenU4++;
					}
				  RcvNewU4 = 1; 											                 /* 	接收到完整数据帧 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* 发送保持寄存器空中断        */
		{                                 
        uiLSRValue = LPC_UART4->LSR;
        if ( uiLSRValue & LSR_THRE )
				{
            //UART4TxEmpty = 1;
        } else 
				{
            //UART4TxEmpty = 0;
        }
    }
}
