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
** Descriptions:		��SSP����������Ϊ����
** input parameters:	��
** output parameters:	��
**********************************************************************************************************/
void  SSP0_Init(void)
{
  uint8_t uiRegVal = 0;
	
  LPC_SC->PCONP |= (1ul << 21);                                   /* ����SSP0����                 */

	/* 
	 * �趨�����Ƶֵ
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
	
    LPC_SSP0->CR0  = 0x0807;													          	/* �������ݳ���Ϊ8λ��֡��ʽSPI��SCK����Ч*/
    LPC_SSP0->CPSR = 0x08;			    		                          /* ����SSP��PCLK��õķ�Ƶ      */
                                                                  /* ֵ,��ģʽ����Ч,             */
                                                                  /* ��СֵΪ0x02	                */
    LPC_SSP0->CR1  = 0x02;		   			                            /* ����SSPΪ����ģʽ,           */
         		   			                     
}
/*********************************************************************************************************
** Function name:		SSP0_SendData
** Descriptions:		SSP�ӿ���SSP���߷�������
** input parameters:	data        �����͵�����
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
void SSP0_SendData(uint8_t data)
{  
    while ((LPC_SSP0->SR & 0x01) == 0);	    	                            /* �ȴ�����FIFO�����ռ�       */  
	        	LPC_SSP0->DR = data;	 
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 	                            /* �ȴ�����֡�������	        */ 
}
/*********************************************************************************************************
** Function name:		SSP0_SendData
** Descriptions:		SSP��������
** input parameters:	
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
uint8_t  SSP0_ReadData(void)
{  
    uint8_t tmp ;
    while ((LPC_SSP0->SR & 0x04) == 0x04)
    {
    	tmp = LPC_SSP0->DR ;
    }	    				                    						                     	/* ��ս���FIFO�����ռ�   	*/    
    while ((LPC_SSP0->SR & 0x01) == 0);	    				                    /* ��շ���FIFO�����ռ�   	*/
    	LPC_SSP0->DR = 0xff ;
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 				                    /* �ȴ�����֡�������		    */
    while ((LPC_SSP0->SR & 0x04) == 0x00);	 				                    /* �ȴ�����֡�������	    	*/		
	   tmp = LPC_SSP0->DR ;
     return tmp;
}



/*********************************************************************************************************
** Function name:		SSP1_Init
** Descriptions:		��SSP����������Ϊ����
** input parameters:	��
** output parameters:	��
**********************************************************************************************************/
void  SSP1_Init(void)
{
    uint8_t uiRegVal = 0;

    LPC_SC->PCONP |= (1ul<<10);                                     /* ����SSP1����                 */

    /* 
     * �趨�����Ƶֵ
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
		
	  LPC_SSP1->CR0  = 0x0807; 				/* �������ݳ���Ϊ8λ��֡��ʽSPI��SCK����Ч����һ��ʱ���ز�����λ����ΪĬ��ֵ 	*/
		LPC_SSP1->CPSR = 0x04;			    /* ����SSP��PCLK��õķ�Ƶֵ,��ģʽ����Ч, ��СֵΪ0x02 */
		LPC_SSP1->CR1  = 0x02;		   		/* ����SSPΪ����ģʽ,       */	 
}

/*********************************************************************************************************
** Function name:		SSP1_SendData
** Descriptions:		SSP�ӿ���SSP���߷�������
** input parameters:	data        �����͵�����
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
uint16_t  SSP1_SendData(uint8_t data)
{  
    while ((LPC_SSP1->SR & 0x02) == 0);	    	                            /* �ȴ�����FIFO�����ռ�         */   
		LPC_SSP1->DR = data;
    while ((LPC_SSP1->SR & 0x10) == 0x10);	 	                            /* �ȴ�����֡�������	        */ 
    return (LPC_SSP1->DR);
}

/*********************************************************************************************************
** Function name:		SSP1_SendData
** Descriptions:		SSP��������
** input parameters:	
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
uint8_t  SSP1_ReadData(void )
{  
	uint8_t tmp ;	
	while ((LPC_SSP1->SR & 0x04) == 0x04)
		tmp = LPC_SSP1->DR ;																			/* ��ս���FIFO�����ռ�   	*/   				                    							  
  while ((LPC_SSP1->SR & 0x01) == 0);	    				            /* ��շ���FIFO�����ռ�   	*/
	  LPC_SSP1->DR = 0xff ;
  while ((LPC_SSP1->SR & 0x10) == 0x10);	 				            /* �ȴ�����֡�������	    	*/
  while ((LPC_SSP1->SR & 0x04) == 0x00);	 				            /* �ȴ�����֡�������		    */
	  tmp = LPC_SSP1->DR ;
  return tmp;

}



/*********************************************************************************************************
** Function name:        SSP2_Init
** Descriptions:       	 SSP2��������               
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void  SSP2_Init(void)
{
  uint8_t uiRegVal = 0;
	
  LPC_SC->PCONP |= (1ul << 20);                             	/* ����SSP2����                 */

	uiRegVal = LPC_SC->PCLKSEL;																	//�趨�����Ƶֵ
	if (uiRegVal < 4) {
			//LPC_SC->PCLKSEL = 4;
	}
	
	LPC_IOCON->P1_0 &= ~0x07;
	LPC_IOCON->P1_0 |=  0x04;                                   /* SSP CLK                      */
	LPC_IOCON->P1_1 &= ~0x07;
	LPC_IOCON->P1_1 |=  0x04;                                   /* SSP MISO                     */
	LPC_IOCON->P1_4 &= ~0x07;    
	LPC_IOCON->P1_4 |=  0x04;                                   /* SSP MOSI                     */

	LPC_SSP2->CR0  = 0x0807;																		/* �������ݳ���Ϊ8λ��֡��ʽSPI��SCK����Ч*/
	LPC_SSP2->CPSR = 0x08;			    		                        /* ����SSP��PCLK��õķ�Ƶ  */
																															/* ֵ,��ģʽ����Ч,         */
																															/* ��СֵΪ0x02	            */
	LPC_SSP2->CR1  = 0x02;		   			                          /* ����SSPΪ����ģʽ,       */  /* ��ģʽ����Ч, ��СֵΪ0x02   */
}
/*********************************************************************************************************
** Function name:		SSP2_SendData
** Descriptions:		SSP�ӿ���SSP���߷�������
** input parameters:	data        �����͵�����
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
void  SSP2_SendData(uint8_t data)
{  
	while (!(LPC_SSP2->SR & 0x01));	    				                /* �ȴ�����FIFO�����ռ�   	*/
	LPC_SSP2->DR = data;
	while ((LPC_SSP2->SR & 0x10) == 0x10);	 				            /* �ȴ�����֡�������		    */
}
/*********************************************************************************************************
** Function name:		SSP2_SendData
** Descriptions:		SSP��������
** input parameters:	
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
uint8_t  SSP2_ReadData(void )
{  
	uint8_t tmp ;	
	while ((LPC_SSP2->SR & 0x04) == 0x04)
		tmp = LPC_SSP2->DR ;																			/* ��ս���FIFO�����ռ�   	*/   				                    							  
  while ((LPC_SSP2->SR & 0x01) == 0);	    				            /* ��շ���FIFO�����ռ�   	*/
	  LPC_SSP2->DR = 0xff ;
  while ((LPC_SSP2->SR & 0x10) == 0x10);	 				            /* �ȴ�����֡�������	    	*/
  while ((LPC_SSP2->SR & 0x04) == 0x00);	 				            /* �ȴ�����֡�������		    */
	  tmp = LPC_SSP2->DR ;
  return tmp;
}
