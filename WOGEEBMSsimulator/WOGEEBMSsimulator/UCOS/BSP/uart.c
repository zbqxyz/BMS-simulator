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
uint8_t               RcvBufU0[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU0[UARTBUFLEN];                                      /*  ���ڷ������ݻ�����          */
uint32_t              GulNumU0;                                             		 /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU0;                                                  /*  ���ڽ������ݵĳ���          */
uint32_t              SndLenU0;                                                  /*  ���ڽ������ݵĳ���          */
uint8_t         			RcvNewU0;                                          		     /*  ���ڽ��������ݵı�־        */

/*********************************************************************************************************
** Function name:	    UART0Init
** Descriptions:	    UART0��ʼ��������Ϊ8λ����λ��1λֹͣλ����У�飬������ΪUART0_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART0Init (void)
{
    uint32_t Fdiv;

		LPC_SC->PCONP |= (1<<3);
    
    LPC_IOCON->P0_2  &= ~0x07;                                      /* ��IOCON����ѡ��Ĵ�������λ����    */
    LPC_IOCON->P0_2  |= 1;                                          /* ��IOCON ��Ϊѡ��Ĺ���       */
    LPC_IOCON->P0_3  &= ~0x07;                                      /* ��IOCON����ѡ��Ĵ�������λ����    */
    LPC_IOCON->P0_3  |= 1;                                          /* ��IOCON ��Ϊѡ��Ĺ���       */
    
    LPC_UART0->LCR = 0x83;                                          /* ѡ��У��λ                   */
    Fdiv = ( PeripheralClock / 16 ) / (UART0_BPS) ;                 /*  ����ʱ��                    */
    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;                                         
    LPC_UART0->FCR = 0x87;                                      

    NVIC_EnableIRQ(UART0_IRQn);
  
    LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;
}

/*********************************************************************************************************
** Function name:	    UART0SendByte
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART0SendByte (uint8_t ucDat)
{
    LPC_UART0->THR = ucDat;                                                 /* д������                     */
    while ((LPC_UART0->LSR & 0x20) == 0);                                		/* �ȴ����ݷ������             */
}

/*********************************************************************************************************
** Function name:	    UART0SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART0SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++)                                             /* ����ָ�����ֽ�����           */
		{                                      
        UART0SendByte (*pucStr++);
    }
}
/*********************************************************************************************************
** Function name:	    UART0RxbufClear
** Descriptions:    	��ս��ܻ���
** input parameters:    
**                      
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART0RxbufClear(void )
{
		if(RcvNewU0!=0||RcvLenU0!=0){
			RcvNewU0 = 0;                                                       		/* �����*/
			RcvLenU0 = 0;
			memset(RcvBufU0,0,sizeof(RcvBufU0));
		}
}

/*********************************************************************************************************
* Function Name  : UART0_IRQHandler
* Description    : UART0 �жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART0_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART0->IIR;
    uiIIRValue >>= 1;                                              /* �жϱ�־�Ĵ���ֵ����1λ    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* ������״̬�ж�             */
	 {                                       
        uiLSRValue = LPC_UART0->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART0->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* ����FIFO��Ϊ���ж�         */
				{                                   
            for (GulNumU0 = 0; GulNumU0 < 8; GulNumU0++)           /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_RDA )                          /* �������ݿ����ж�           */
	 {                                   
            for (GulNumU0 = 0; GulNumU0 < 8; GulNumU0++)       /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_CTI )                          /* �ַ���ʱ�ж�               */
		{                                   
          while ((LPC_UART0->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU0 >= UARTBUFLEN)
						{
							UART0RxbufClear();
						}                       								           /*  �ж������Ƿ�������       */ 
				    RcvBufU0[RcvLenU0] = LPC_UART0->RBR;
            RcvLenU0++;
					}
				  RcvNewU0 = 1; 											                 /* 	���յ���������֡ 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* ���ͱ��ּĴ������ж�        */
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
uint8_t               RcvBufU1[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU1[UARTBUFLEN];                                      /*  ���ڷ������ݻ�����          */
uint32_t              GulNumU1;                                             		 /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU1;                                                  /*  ���ڽ������ݵĳ���          */
uint32_t              SndLenU1;                                                  /*  ���ڽ������ݵĳ���          */
uint8_t         			RcvNewU1;                                          		     /*  ���ڽ��������ݵı�־        */

/*********************************************************************************************************
** Function name:	    UART1Init
** Descriptions:	    UART1��ʼ��������Ϊ8λ����λ��1λֹͣλ����У�飬������ΪUART1_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1SendByte (uint8_t ucDat)
{
    LPC_UART1->THR = ucDat;                                                 /* д������                     */
    while ((LPC_UART1->LSR & 0x20) == 0);                                		/* �ȴ����ݷ������             */
}


/*********************************************************************************************************
** Function name:	    UART1SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;

    for (i = 0; i < ulNum; i++)                   /* ����ָ�����ֽ�����           */
		{                                      
        UART1SendByte (*pucStr++);
    }
			
}
/*********************************************************************************************************
** Function name:	    UART1RxbufClear
** Descriptions:    	��ս��ܻ���
** input parameters:    
**                      
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1RxbufClear(void )
{
		if(RcvNewU1!=0||RcvLenU1!=0){
			RcvNewU1 = 0;                                              		/* �����־*/
			RcvLenU1 = 0;
			memset(RcvBufU1,0,sizeof(RcvBufU1));
		}
}

/*********************************************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : UART1�жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART1_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART1->IIR;
    uiIIRValue >>= 1;                                              /* �жϱ�־�Ĵ���ֵ����1λ    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* ������״̬�ж�             */
	 {                                       
        uiLSRValue = LPC_UART1->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART1->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* ����FIFO��Ϊ���ж�         */
				{                                   
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++)           /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_RDA )                          /* �������ݿ����ж�           */
	 {                                   
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++)       /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_CTI )                          /* �ַ���ʱ�ж�               */
		{                                   
          while ((LPC_UART1->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU1 >= UARTBUFLEN)
						{
							UART1RxbufClear();
						}                       								           /*  �ж������Ƿ�������       */ 
				    RcvBufU1[RcvLenU1] = LPC_UART1->RBR;
            RcvLenU1++;
					}
				  RcvNewU1 = 1; 											                 /* 	���յ���������֡ 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* ���ͱ��ּĴ������ж�        */
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
uint8_t               RcvBufU2[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU2[UARTBUFLEN];                                      /*  ���ڷ������ݻ�����          */
uint32_t              GulNumU2;                                             		 /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU2;                                                  /*  ���ڽ������ݵĳ���          */
uint32_t              SndLenU2;                                                  /*  ���ڽ������ݵĳ���          */
uint8_t         			RcvNewU2;                                          		     /*  ���ڽ��������ݵı�־        */

/*********************************************************************************************************
** Function name:	    UART2Init
** Descriptions:	    UART3��ʼ��������Ϊ8λ����λ��1λֹͣλ����У�飬������ΪUART2_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART2SendByte (uint8_t ucDat)
{
    LPC_UART2->THR = ucDat;                                                 /* д������                     */
    while ((LPC_UART2->LSR & 0x20) == 0);                                		/* �ȴ����ݷ������             */

}
/*********************************************************************************************************
** Function name:	    UART2SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART2SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
	 
    uint32_t i;
	  ENWR485_U1();
    for (i = 0; i < ulNum; i++)                   /* ����ָ�����ֽ�����           */
		{                                      
        UART2SendByte (*pucStr++);
    }
		OSTimeDlyHMSM(0,0,0,10);		                  /*�ȴ��㹻��ʱ��ʹ���ͳ������  */
		ENRD485_U1();	
}
/*********************************************************************************************************
** Function name:	    UART2RxbufClear
** Descriptions:    	��ս��ܻ���
** input parameters:    
**                      
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART2RxbufClear(void )
{
		if(RcvNewU2!=0||RcvLenU2!=0){
			RcvNewU2 = 0;                                              		/* �����־*/
			RcvLenU2 = 0;
			memset(RcvBufU2,0,sizeof(RcvBufU2));
		}
}

/*********************************************************************************************************
* Function Name  : UART0_IRQHandler
* Description    : UART0 �жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART2_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART2->IIR;
    uiIIRValue >>= 1;                                              /* �жϱ�־�Ĵ���ֵ����1λ    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* ������״̬�ж�             */
	 {                                       
        uiLSRValue = LPC_UART2->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART2->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* ����FIFO��Ϊ���ж�         */
				{                                   
            for (GulNumU2 = 0; GulNumU2 < 8; GulNumU2++)           /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_RDA )                          /* �������ݿ����ж�           */
	 {                                   
            for (GulNumU2 = 0; GulNumU2 < 8; GulNumU2++)       /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_CTI )                          /* �ַ���ʱ�ж�               */
		{                                   
          while ((LPC_UART2->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU2 >= UARTBUFLEN)
						{
							UART2RxbufClear();
						}                       								           /*  �ж������Ƿ�������       */ 
				    RcvBufU2[RcvLenU2] = LPC_UART2->RBR;
            RcvLenU2++;
					}
				  RcvNewU2 = 1; 											                 /* 	���յ���������֡ 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* ���ͱ��ּĴ������ж�        */
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
uint8_t               RcvBufU3[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU3[UARTBUFLEN];                                      /*  ���ڷ������ݻ�����          */
uint32_t              GulNumU3;                                             		 /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU3;                                                  /*  ���ڽ������ݵĳ���          */
uint32_t              SndLenU3;                                                  /*  ���ڽ������ݵĳ���          */
uint8_t         			RcvNewU3;                                          		     /*  ���ڽ��������ݵı�־        */

/*********************************************************************************************************
** Function name:	    UART3Init
** Descriptions:	    UART3��ʼ��������Ϊ8λ����λ��1λֹͣλ����У�飬������ΪUART3_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART3SendByte (uint8_t ucDat)
{
    LPC_UART3->THR = ucDat;                                                      /* д������                     */
    while ((LPC_UART3->LSR & 0x20) == 0);                                		/* �ȴ����ݷ������             */
}


/*********************************************************************************************************
** Function name:	    UART3SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART3SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++)                   /* ����ָ�����ֽ�����           */
		{                                      
        UART3SendByte (*pucStr++);
    }
}
/*********************************************************************************************************
** Function name:	    UART3RxbufClear
** Descriptions:    	��ս��ܻ���
** input parameters:    
**                      
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART3RxbufClear(void )
{
		if(RcvNewU3!=0||RcvLenU3!=0){
			RcvNewU3 = 0;                                              		/* �����־*/
			RcvLenU3 = 0;
			memset(RcvBufU3,0,sizeof(RcvBufU3));
		}
}

/*********************************************************************************************************
* Function Name  : UART3_IRQHandler
* Description    : UART3 �жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART3_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART3->IIR;
    uiIIRValue >>= 1;                                              /* �жϱ�־�Ĵ���ֵ����1λ    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* ������״̬�ж�             */
	 {                                       
        uiLSRValue = LPC_UART3->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART3->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* ����FIFO��Ϊ���ж�         */
				{                                   
            for (GulNumU3 = 0; GulNumU3 < 8; GulNumU3++)           /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_RDA )                          /* �������ݿ����ж�           */
	 {                                   
            for (GulNumU3 = 0; GulNumU3 < 8; GulNumU3++)       /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_CTI )                          /* �ַ���ʱ�ж�               */
		{                                   
          while ((LPC_UART3->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU3 >= UARTBUFLEN)
						{
							UART3RxbufClear();
						}                       								           /*  �ж������Ƿ�������       */ 
				    RcvBufU3[RcvLenU3] = LPC_UART3->RBR;
            RcvLenU3++;
					}
				  RcvNewU3 = 1; 											                 /* 	���յ���������֡ 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* ���ͱ��ּĴ������ж�        */
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
uint8_t               RcvBufU4[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU4[UARTBUFLEN];                                      /*  ���ڷ������ݻ�����          */
uint32_t              GulNumU4;                                             		 /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU4;                                                  /*  ���ڽ������ݵĳ���          */
uint32_t              SndLenU4;                                                  /*  ���ڽ������ݵĳ���          */
uint8_t         			RcvNewU4;                                          		     /*  ���ڽ��������ݵı�־        */

/*********************************************************************************************************
** Function name:	    UART4Init
** Descriptions:	    UART4��ʼ��������Ϊ8λ����λ��1λֹͣλ����У�飬������ΪUART3_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART4SendByte (uint8_t ucDat)
{
    LPC_UART4->THR = ucDat;                                                      /* д������                     */
    while ((LPC_UART4->LSR & 0x20) == 0);                                		/* �ȴ����ݷ������             */
}


/*********************************************************************************************************
** Function name:	    UART4SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART4SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;

    for (i = 0; i < ulNum; i++)                   /* ����ָ�����ֽ�����           */
		{                                      
        UART4SendByte (*pucStr++);
    }

}
/*********************************************************************************************************
** Function name:	    UART4RxbufClear
** Descriptions:    	��ս��ܻ���
** input parameters:    
**                      
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART4RxbufClear(void )
{
		if(RcvNewU4!=0||RcvLenU4!=0){
			RcvNewU4 = 0;                                              		/* �����־*/
			RcvLenU4 = 0;
			memset(RcvBufU4,0,sizeof(RcvBufU4));
		}
}

/*********************************************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : UART4 �жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART4_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;


  	uiIIRValue = LPC_UART4->IIR;
    uiIIRValue >>= 1;                                              /* �жϱ�־�Ĵ���ֵ����1λ    */
    uiIIRValue &= 0x07;

   if ( uiIIRValue == IIR_RLS )                                    /* ������״̬�ж�             */
	 {                                       
        uiLSRValue = LPC_UART4->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
				{
            uiDummy = LPC_UART4->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR )                                /* ����FIFO��Ϊ���ж�         */
				{                                   
            for (GulNumU4 = 0; GulNumU4 < 8; GulNumU4++)           /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_RDA )                          /* �������ݿ����ж�           */
	 {                                   
            for (GulNumU4 = 0; GulNumU4 < 8; GulNumU4++)       /*  ��������8���ֽ�             */
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
	 
    else if ( uiIIRValue == IIR_CTI )                          /* �ַ���ʱ�ж�               */
		{                                   
          while ((LPC_UART4->LSR & 0x01) == 0x01)
				 {  
						if(RcvLenU4 >= UARTBUFLEN)
						{
							UART4RxbufClear();
						}                       								           /*  �ж������Ƿ�������       */ 
				    RcvBufU4[RcvLenU4] = LPC_UART4->RBR;
            RcvLenU4++;
					}
				  RcvNewU4 = 1; 											                 /* 	���յ���������֡ 		     	*/
    }
		
    else if ( uiIIRValue == IIR_THRE )                         /* ���ͱ��ּĴ������ж�        */
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
