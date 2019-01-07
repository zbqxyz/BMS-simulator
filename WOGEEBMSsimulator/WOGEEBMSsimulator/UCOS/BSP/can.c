/****************************************Copyright (c)****************************************************
**                                Tomorn electronics Co.,LTD.
**
**                                 http://www.tomorn.com
**                                    wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:           can.c
** Last modified Date:  2017-06-08
** Last Version:        V1.0
** Descriptions:        
*********************************************************************************************************/
#include  "can.h"

/*�����ѭ�������ٶ� �Ǹ������жϵ��ٶȣ���������ݶ�ʧ����CAN,UART,SPI,ADC,��̫����
  ���Կ���ͨ�����������ķ����������ֹ���ݶ�ʧ��
  ���ж���Ϊд��������������д���ݣ�����ѭ��ÿ������ȡ���ݣ������궪��  */
MessageDetail MsgCAN0BUF[CAN_MSG_NUMBER];                           /*����CAN������  �����Ƚ��ȳ��������ݣ���ֹ����              */
MessageDetail *pwMsgCAN0 = MsgCAN0BUF;                              /*��������ָ�룬дָ��Ͷ�ָ�룬ָ�򻺳���                   */
MessageDetail *prMsgCAN0 = MsgCAN0BUF;                              /*��������ָ�룬дָ��Ͷ�ָ�룬ָ�򻺳���                   */


MessageDetail MessageDetailT;                                       /* ���÷���ͨ�����ݱ���         */
MessageDetail MessageCAN0;                                          /* ����CAN0ͨ��֡����           */
MessageDetail MessageCAN1;                                          /* ����CAN1ͨ��֡����           */


unsigned int  CANID   ;
unsigned char CAN_Data[8]; 
/*********************************************************************************************************
** Function name:       Enter_SWRst
** Descriptions:        ��PC��������ʾ�ַ���
** input parameters:    Ch:CAN�������ţ�0��ʾ��һ·CAN
** Output parameters:   ��
** Returned value:      0��          ʧ��
**                      1:           �ɹ�
*********************************************************************************************************/
unsigned char Enter_SWRst(unsigned char Ch)
{
    unsigned long regaddr;
    REG_CANMOD regmod;
    regaddr = (unsigned long)(&LPC_CAN1->MOD)+Ch*CANOFFSET;            
    regmod.DWord = RGE(regaddr);                                        /* ��ȡLPC_CAN1->MOD�Ĵ���      */
    regmod.Bits.RM = 1;                                                 /* RMλ��"1"                    */
    RGE(regaddr) = regmod.DWord;                                        /* ��дLPC_CAN1->MOD�Ĵ���      */
    regmod.DWord = RGE(regaddr);                                        /* ��֤д��ֵ                   */
    return (0 != regmod.Bits.RM)? 1:0; 
}  

/*********************************************************************************************************
** Functoin name:       Quit_SWRst
** Descriptions:        CAN�������˳������λģʽ
** input paraments:     Ch:     CAN�������ţ�0��ʾ��һ·CAN
** output paraments:    ��    
** Returned values:     0��     ʧ��
**                      1:      �ɹ�
*********************************************************************************************************/
unsigned char Quit_SWRst(unsigned char Ch)
{
    unsigned long regaddr;
    REG_CANMOD regmod;
    regaddr = (unsigned long)(&LPC_CAN1->MOD)+Ch*CANOFFSET;            
    regmod.DWord = RGE(regaddr);                                        /* ��ȡLPC_CAN1->MOD�Ĵ���      */
    regmod.Bits.RM = 0;                                                 /* RMλ��"0"                    */
    RGE(regaddr) = regmod.DWord;                                        /* ��дLPC_CAN1->MOD�Ĵ���      */
    regmod.DWord = RGE(regaddr);                                        /* ��֤д��ֵ                   */
    return (0 != regmod.Bits.RM)? 0:1; 
}

/*********************************************************************************************************
** Functoin name:       CAN_Init
** Descriptions:        CAN�������˳������λģʽ
** input paraments:     Ch:     CAN�������ţ�0��ʾ��һ·CAN
**                      Baud:   CAN������ֵ
** output paraments:    ��    
** Returned values:     ��
*********************************************************************************************************/
void CAN_Init(unsigned char Ch, unsigned long Baud)
{
    unsigned long regaddr;
    uint16_t i;
	
	
    i = i;

    switch(Ch){                                                         /* ����CAN����������            */
    case 0:
        LPC_SC->PCONP   |= 0x01L<<13;                                   /* ��CAN��������Դ            */
        LPC_IOCON->P0_21 &= ~0x07;
        LPC_IOCON->P0_21|= 0x04;                                        /* ѡ��RD1                      */
        LPC_IOCON->P0_22 &= ~0x07;
        LPC_IOCON->P0_22 |= 0x04;                                       /* ѡ��TD1                      */
        break;
    case 1:
        LPC_SC->PCONP   |= 0x01L<<14;                                   /* ��CAN��������Դ            */
        LPC_IOCON->P2_7 &= ~0x07;
        LPC_IOCON->P2_7 |= 0x01;                                        /* ѡ��RD2                      */
                                                                       
        LPC_IOCON->P2_8 &= ~0x07;
        LPC_IOCON->P2_8 |= 0x01;                                        /* ѡ��TD2                      */
                                                                       
        break;
    default:
        break;
    }
    
    Enter_SWRst(Ch);

    regaddr = (unsigned long)(&LPC_CAN1->BTR)+Ch*CANOFFSET;
    RGE(regaddr) = Baud;
   
    LPC_CANAF->AFMR = 0x01;                                             // ����Ϊ�ر�ģʽ               
    if(Ch == 0){                                                                        
         LPC_CAN1->CMR  |= (1 << 1) | (1 << 2) | (1 << 3);              // �ͷŽ��ջ�����                                             
         LPC_CAN1->IER |= (1<<0);                                       // �����ж�ʹ��                  
         LPC_CAN1->GSR |= (0<<0);
         i = LPC_CAN1->ICR;                                             // ���жϱ�־                  
    } else{
         LPC_CAN2->CMR |= (1 << 1) | (1 << 2) | (1 << 3);
         LPC_CAN2->IER |= (1<<0);
         LPC_CAN2->GSR |= (0<<0);
         i = LPC_CAN2->ICR;
						
    }

    LPC_CANAF->AFMR = 0x02;                                             // ����Ϊ��·ģʽ               
    Quit_SWRst(Ch);

    NVIC_EnableIRQ(CAN_IRQn);                                           /* ʹCAN�ж�                    */
}
 
/*********************************************************************************************************
** Functoin name:       CANRCV
** Descriptions:        ����CAN֡����ָ���Ľṹ����
** input paraments:     Ch:           CAN�������ţ�0��ʾ��һ·CAN
**                      *MessageCAN:  ���CAN֡��Ϣ�Ľṹ��
** output paraments:    ��    
** Returned values:     ��
*********************************************************************************************************/
unsigned char CANRCV(unsigned char Ch, MessageDetail *MessageCAN)
{
	  
    unsigned long mes;
    unsigned long regaddr;
    regaddr = (unsigned long)(&LPC_CAN1->RFS)+Ch*CANOFFSET;             /* ����DLC��RTR��FF             */
    mes = RGE(regaddr);
    MessageCAN->LEN =  (mes >>16) & 0x0F;                               /* ��ȡ֡����                   */
    MessageCAN->FF  =  (mes >>31);                                      /* ��ȡFF                       */
   
    regaddr = (unsigned long)(&LPC_CAN1->RID)+Ch*CANOFFSET;             /* ����ID                       */
    mes = RGE(regaddr);
    if(MessageCAN->FF) 
		{                                                                   /* FFΪ1��IDΪ29λ              */
   
        MessageCAN->CANID = mes & 0x1fffffff;
    } else 
		{                                                                   /* FFΪ0 ��IDΪ11λ             */
        MessageCAN->CANID = mes & 0x000007ff;
    }
		
   if (Ch==0)
	 {
    regaddr = (unsigned long)(&LPC_CAN1->RDA)+Ch*CANOFFSET;             /* ���� ����A                   */
    mes = RGE(regaddr);
    MessageCAN->DATAA = mes; 
   
    regaddr = (unsigned long)(&LPC_CAN1->RDB)+Ch*CANOFFSET;             /* ���� ����B                   */
    mes = RGE(regaddr);
    MessageCAN->DATAB = mes;
		 
   }else if(Ch==1)
	 {
	  regaddr = (unsigned long)(&LPC_CAN1->RDA)+Ch*CANOFFSET;             /* ���� ����A                   */
    mes = RGE(regaddr);
    MessageCAN->DATAA = mes; 
   
    regaddr = (unsigned long)(&LPC_CAN1->RDB)+Ch*CANOFFSET;             /* ���� ����B                   */
    mes = RGE(regaddr);
    MessageCAN->DATAB = mes;

   }
		 
       return(1);                                        
}
/*********************************************************************************************************
** Functoin name:               CANSend
** Descriptions:  	            CAN�������˳������λģʽ
** input paraments:	            Ch:             CAN�������ţ�0��ʾ��һ·CAN
**		                        BUFNum:         ѡ�񻺳���
** output paraments:	    	MessageDetailT: ���ĵ�������Ϣ������	
** Returned values:	   			0��             ʧ��
**							          	1:              �ɹ�
*********************************************************************************************************/
unsigned char CANSend(unsigned char Ch, unsigned char BUFNum)
{
    unsigned long CAN32reg;
    unsigned long regaddr;
    unsigned char FFflag;
	
    BUFNum-=1;		                                                            /* �����ַ����                 */
	
    regaddr = (unsigned long)(&LPC_CAN1->TFI1)+Ch*CANOFFSET+BUFNum*0X10;	    /* 3���������ַ�� 0x10         */
    CAN32reg = RGE(regaddr);
    CAN32reg &= ~((0x0fL<<16) | (0x01L<<30) | (0x80000000));                  /* �� DLC,RTR.FFλ	            */
    CAN32reg |= ((unsigned long)MessageDetailT.LEN<<16) | ((unsigned long)MessageDetailT.FF<<31);
    RGE(regaddr)=CAN32reg;
	
    FFflag = MessageDetailT.FF;
	
    regaddr = (unsigned long)(&LPC_CAN1->TID1)+Ch*CANOFFSET+BUFNum*0X10;	    /* д֡ID                       */
    CAN32reg = RGE(regaddr);
    if(FFflag){								                                                /* FFΪ1��IDΪ29λ              */
        CAN32reg &=0x70000000;
        CAN32reg |= (MessageDetailT.CANID & 0x1fffffff);
    }  else{
        CAN32reg &= 0xfffff800;	                                              /* FFΪ0 ��IDΪ11λ             */
        CAN32reg |= (MessageDetailT.CANID & 0x000007ff);
    }
    RGE(regaddr)=CAN32reg;
    regaddr = (unsigned long)(&LPC_CAN1->TDA1)+Ch*CANOFFSET+BUFNum*0X10;      /* д֡����A                    */
    RGE(regaddr) = MessageDetailT.DATAA;
	
    regaddr = (unsigned long)(&LPC_CAN1->TDB1)+Ch*CANOFFSET+BUFNum*0X10;	    /* д֡����B                    */
    RGE(regaddr) = MessageDetailT.DATAB;
	
    regaddr = (unsigned long)(&LPC_CAN1->CMR)+Ch*CANOFFSET;                   /* д���ƼĴ���������           */
    CAN32reg = RGE(regaddr);
	
    CAN32reg &= ~0x03;
    CAN32reg |= 0x03;
    CAN32reg &= ~((1<<5)|(1<<6)|(1<<7));
    switch(BUFNum){
    case 0: CAN32reg |= 01<<5;break;
    case 1: CAN32reg |= 01<<6;break;
    case 2: CAN32reg |= 01<<7;break;
    default: break;
    }
    RGE(regaddr)=CAN32reg;
    regaddr = (unsigned long)(&LPC_CAN1->GSR)+Ch*CANOFFSET;                   /* ��ѯ����״̬                 */
    CAN32reg = RGE(regaddr);

    if(CAN32reg&(1<<3))	{                                               /* ���е������ѳɹ����         */
        return(1);                                                      /* ���ͳɹ����� 1               */
    } else { 
        return (0);                                                     /* ����ʧ�ܷ��� 0               */
    }	
}
/*********************************************************************************************************
** Functoin name:               WriteCAN0
** Descriptions:  	            �û���д���� ֡��Ϣ
** input paraments:	            LEN:    ���ݳ���
**				                      *data:  �����������ڵ����� 
**                             	FF:
**							              	ID:
** output paraments:	        	��			
** Returned values:	   		    	0��     ʧ��
**								              1:      �ɹ�
*********************************************************************************************************/
unsigned char WriteCAN0(unsigned char LEN,unsigned char FF, unsigned int ID,unsigned char *data)
{
    unsigned char i;
    if(LEN>8){
        return(0);
    } else {
        MessageDetailT.LEN=LEN;
        MessageDetailT.FF=FF;
        MessageDetailT.CANID=ID; 
        MessageDetailT.DATAA=0;			                                /* ������                                                                */
        MessageDetailT.DATAB=0;
	
        for(i=0;i<LEN;i++) {
            if(i<4){
                MessageDetailT.DATAA |= (*(data+i)<<(i*8)); 
            }  else {
                MessageDetailT.DATAB |= (*(data+i)<<((i-4)*8));
            }
        }
				CANSend(0, 1);
       OSTimeDlyHMSM(0, 0, 0, 5);                                   /*  ��ֹ���͹��� ,������δ���������д����һ֡���ݣ����ܷ���               */
	 return(1);
    }
		
}

/*********************************************************************************************************
** Functoin name:               WriteCAN1
** Descriptions:  	            �û���д���� ֡��Ϣ
** input paraments:	            LEN:    ���ݳ���
**				                      *data:  �����������ڵ����� 
**                             	FF:
**							              	ID:
** output paraments:	        	��	
** Returned values:	   		    	0��     ʧ��
**								              1:      �ɹ�
*********************************************************************************************************/
unsigned char WriteCAN1(unsigned char LEN,unsigned char FF, unsigned int ID,unsigned char *data)
{
    unsigned char i;
    if(LEN>8){
        return(0);
    } else {
        MessageDetailT.LEN=LEN;
        MessageDetailT.FF=FF;
        MessageDetailT.CANID=ID;
        MessageDetailT.DATAA=0;			                                /* ������                       */
        MessageDetailT.DATAB=0;
	
        for(i=0;i<LEN;i++) {
            if(i<4){
                MessageDetailT.DATAA |= (*(data+i)<<(i*8)); 
            }  else {
                MessageDetailT.DATAB |= (*(data+i)<<((i-4)*8));
            }
        }
				CANSend(1, 1);
	return(1);
    }
		
}

/*********************************************************************************************************
** Functoin name:               CAN_IRQHandler
** Descriptions:  	            CAN�������жϴ�����
** input paraments:	            ��
** output paraments:	    	��	
** Returned values:	   			��
*********************************************************************************************************/
void CAN_IRQHandler (void)
{
    unsigned char j;
    unsigned int CAN32reg,mes;
    unsigned int regaddr;

    /*
     * ���CANͨ����Ϊ2���ֱ�������ͨ�����ж�   
     */
    for(j=0;j<2;j++){				
	
		regaddr = (unsigned long)(&LPC_CAN1->ICR)+j*CANOFFSET;	
		CAN32reg=RGE(regaddr);	
		
		if((CAN32reg&(1<<0))!= 0)	                                      /* RI �����ж�                  */
		{
			CAN32reg &= ~(0x01);
			RGE(regaddr) = CAN32reg;

       if(j==0)
       {					 
						CANRCV(j, pwMsgCAN0);                                          /* �յ�CAN0�ж�,����֡ �Ž��������ȴ�����ȡ��ȥ����               */         			 
	          if(pwMsgCAN0 == &MsgCAN0BUF[CAN_MSG_NUMBER-2])                
					  { 
                 pwMsgCAN0 = MsgCAN0BUF;							                     /* ����ָ��ָ��                                                   */
						}	
				   else                                                            /* ����һ���洢��Ԫ����ֹ��ָ����ָ�����һ���洢��Ԫ�����ӷ�     */
					  { 
                 pwMsgCAN0++;                                              /* ָ��������������󣬷�ָֹ�����������ظ��Ĵ���             */
						}	
						
											
		         /* ��д����֡��Ϣ*/
		       MessageDetailT.LEN=MessageCAN0.LEN;
		       MessageDetailT.FF=MessageCAN0.FF;
		       MessageDetailT.CANID=MessageCAN0.CANID;
		     
 
//         CANRCV(j, &MessageCAN0); 
//			   CAN0_RECData_Pro() ;				 
			   regaddr = (unsigned long)(&LPC_CAN1->CMR)+j*CANOFFSET;				
			   mes=RGE(regaddr);
			   mes |= (1<<2);										                          /* �ͷŽ��ջ�����               */
			   RGE(regaddr)=mes;
			}
			
			if(j==1)
      {
			   CANRCV(j, &MessageCAN1);                      		        	/* �յ�CAN1�ж�,����֡          */
			        /* ��д����֡��Ϣ*/
//			     MessageDetailT.LEN=MessageCAN1.LEN;
//		       MessageDetailT.FF=MessageCAN1.FF;
//		       MessageDetailT.CANID=MessageCAN1.CANID;
//               /* �жϷ�����*/
//						CAN1_RECData_Pro();
			   regaddr = (unsigned long)(&LPC_CAN1->CMR)+j*CANOFFSET;				
			   mes=RGE(regaddr);
			   mes |= (1<<2);								            /* �ͷŽ��ջ�����               */
			   RGE(regaddr)=mes;
			}	
		}                
		
	}		
}

/*********************************************************************************************************
** Functoin name:               CAN0_RECData_Pro
** Descriptions:  	            CAN�������жϴ�����
** input paraments:	            ��
** output paraments:	    	��	
** Returned values:	   			��
*********************************************************************************************************/
void CAN0_RECData_Pro(void)
{
	 
	 //�Ƶ�  BMSMain�����µ�   Charger_RECData_Pro  ������
}

/*********************************************************************************************************
** Functoin name:               CAN_IRQHandler
** Descriptions:  	            CAN�������жϴ�����
** input paraments:	            ��
** output paraments:	    	��	
** Returned values:	   			��
*********************************************************************************************************/
void CAN1_RECData_Pro(void)
{
	
}
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
