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

/*任务的循环处理速度 是跟不上中断的速度，易造成数据丢失，如CAN,UART,SPI,ADC,以太网等
  所以可以通过做缓冲区的方法来处理防止数据丢失，
  即中断做为写数据往缓冲区内写数据，任务循环每次来读取数据，处理完丢弃  */
MessageDetail MsgCAN0BUF[CAN_MSG_NUMBER];                           /*定义CAN缓冲曲  用于先进先出处理数据，防止丢包              */
MessageDetail *pwMsgCAN0 = MsgCAN0BUF;                              /*定义两个指针，写指针和读指针，指向缓冲区                   */
MessageDetail *prMsgCAN0 = MsgCAN0BUF;                              /*定义两个指针，写指针和读指针，指向缓冲区                   */


MessageDetail MessageDetailT;                                       /* 引用发送通道数据变量         */
MessageDetail MessageCAN0;                                          /* 引用CAN0通道帧变量           */
MessageDetail MessageCAN1;                                          /* 引用CAN1通道帧变量           */


unsigned int  CANID   ;
unsigned char CAN_Data[8]; 
/*********************************************************************************************************
** Function name:       Enter_SWRst
** Descriptions:        向PC机发送显示字符。
** input parameters:    Ch:CAN控制器号，0表示第一路CAN
** Output parameters:   无
** Returned value:      0：          失败
**                      1:           成功
*********************************************************************************************************/
unsigned char Enter_SWRst(unsigned char Ch)
{
    unsigned long regaddr;
    REG_CANMOD regmod;
    regaddr = (unsigned long)(&LPC_CAN1->MOD)+Ch*CANOFFSET;            
    regmod.DWord = RGE(regaddr);                                        /* 读取LPC_CAN1->MOD寄存器      */
    regmod.Bits.RM = 1;                                                 /* RM位置"1"                    */
    RGE(regaddr) = regmod.DWord;                                        /* 回写LPC_CAN1->MOD寄存器      */
    regmod.DWord = RGE(regaddr);                                        /* 验证写入值                   */
    return (0 != regmod.Bits.RM)? 1:0; 
}  

/*********************************************************************************************************
** Functoin name:       Quit_SWRst
** Descriptions:        CAN控制器退出软件复位模式
** input paraments:     Ch:     CAN控制器号，0表示第一路CAN
** output paraments:    无    
** Returned values:     0：     失败
**                      1:      成功
*********************************************************************************************************/
unsigned char Quit_SWRst(unsigned char Ch)
{
    unsigned long regaddr;
    REG_CANMOD regmod;
    regaddr = (unsigned long)(&LPC_CAN1->MOD)+Ch*CANOFFSET;            
    regmod.DWord = RGE(regaddr);                                        /* 读取LPC_CAN1->MOD寄存器      */
    regmod.Bits.RM = 0;                                                 /* RM位置"0"                    */
    RGE(regaddr) = regmod.DWord;                                        /* 回写LPC_CAN1->MOD寄存器      */
    regmod.DWord = RGE(regaddr);                                        /* 验证写入值                   */
    return (0 != regmod.Bits.RM)? 0:1; 
}

/*********************************************************************************************************
** Functoin name:       CAN_Init
** Descriptions:        CAN控制器退出软件复位模式
** input paraments:     Ch:     CAN控制器号，0表示第一路CAN
**                      Baud:   CAN波特率值
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_Init(unsigned char Ch, unsigned long Baud)
{
    unsigned long regaddr;
    uint16_t i;
	
	
    i = i;

    switch(Ch){                                                         /* 配置CAN控制器引脚            */
    case 0:
        LPC_SC->PCONP   |= 0x01L<<13;                                   /* 打开CAN控制器电源            */
        LPC_IOCON->P0_21 &= ~0x07;
        LPC_IOCON->P0_21|= 0x04;                                        /* 选择RD1                      */
        LPC_IOCON->P0_22 &= ~0x07;
        LPC_IOCON->P0_22 |= 0x04;                                       /* 选择TD1                      */
        break;
    case 1:
        LPC_SC->PCONP   |= 0x01L<<14;                                   /* 打开CAN控制器电源            */
        LPC_IOCON->P2_7 &= ~0x07;
        LPC_IOCON->P2_7 |= 0x01;                                        /* 选择RD2                      */
                                                                       
        LPC_IOCON->P2_8 &= ~0x07;
        LPC_IOCON->P2_8 |= 0x01;                                        /* 选择TD2                      */
                                                                       
        break;
    default:
        break;
    }
    
    Enter_SWRst(Ch);

    regaddr = (unsigned long)(&LPC_CAN1->BTR)+Ch*CANOFFSET;
    RGE(regaddr) = Baud;
   
    LPC_CANAF->AFMR = 0x01;                                             // 设置为关闭模式               
    if(Ch == 0){                                                                        
         LPC_CAN1->CMR  |= (1 << 1) | (1 << 2) | (1 << 3);              // 释放接收缓冲区                                             
         LPC_CAN1->IER |= (1<<0);                                       // 接收中断使能                  
         LPC_CAN1->GSR |= (0<<0);
         i = LPC_CAN1->ICR;                                             // 清中断标志                  
    } else{
         LPC_CAN2->CMR |= (1 << 1) | (1 << 2) | (1 << 3);
         LPC_CAN2->IER |= (1<<0);
         LPC_CAN2->GSR |= (0<<0);
         i = LPC_CAN2->ICR;
						
    }

    LPC_CANAF->AFMR = 0x02;                                             // 设置为旁路模式               
    Quit_SWRst(Ch);

    NVIC_EnableIRQ(CAN_IRQn);                                           /* 使CAN中断                    */
}
 
/*********************************************************************************************************
** Functoin name:       CANRCV
** Descriptions:        接受CAN帧放在指定的结构体中
** input paraments:     Ch:           CAN控制器号，0表示第一路CAN
**                      *MessageCAN:  存放CAN帧信息的结构体
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
unsigned char CANRCV(unsigned char Ch, MessageDetail *MessageCAN)
{
	  
    unsigned long mes;
    unsigned long regaddr;
    regaddr = (unsigned long)(&LPC_CAN1->RFS)+Ch*CANOFFSET;             /* 处理DLC、RTR、FF             */
    mes = RGE(regaddr);
    MessageCAN->LEN =  (mes >>16) & 0x0F;                               /* 获取帧长度                   */
    MessageCAN->FF  =  (mes >>31);                                      /* 获取FF                       */
   
    regaddr = (unsigned long)(&LPC_CAN1->RID)+Ch*CANOFFSET;             /* 处理ID                       */
    mes = RGE(regaddr);
    if(MessageCAN->FF) 
		{                                                                   /* FF为1，ID为29位              */
   
        MessageCAN->CANID = mes & 0x1fffffff;
    } else 
		{                                                                   /* FF为0 ，ID为11位             */
        MessageCAN->CANID = mes & 0x000007ff;
    }
		
   if (Ch==0)
	 {
    regaddr = (unsigned long)(&LPC_CAN1->RDA)+Ch*CANOFFSET;             /* 处理 数据A                   */
    mes = RGE(regaddr);
    MessageCAN->DATAA = mes; 
   
    regaddr = (unsigned long)(&LPC_CAN1->RDB)+Ch*CANOFFSET;             /* 处理 数据B                   */
    mes = RGE(regaddr);
    MessageCAN->DATAB = mes;
		 
   }else if(Ch==1)
	 {
	  regaddr = (unsigned long)(&LPC_CAN1->RDA)+Ch*CANOFFSET;             /* 处理 数据A                   */
    mes = RGE(regaddr);
    MessageCAN->DATAA = mes; 
   
    regaddr = (unsigned long)(&LPC_CAN1->RDB)+Ch*CANOFFSET;             /* 处理 数据B                   */
    mes = RGE(regaddr);
    MessageCAN->DATAB = mes;

   }
		 
       return(1);                                        
}
/*********************************************************************************************************
** Functoin name:               CANSend
** Descriptions:  	            CAN控制器退出软件复位模式
** input paraments:	            Ch:             CAN控制器号，0表示第一路CAN
**		                        BUFNum:         选择缓冲区
** output paraments:	    	MessageDetailT: 报文的特征信息及数据	
** Returned values:	   			0：             失败
**							          	1:              成功
*********************************************************************************************************/
unsigned char CANSend(unsigned char Ch, unsigned char BUFNum)
{
    unsigned long CAN32reg;
    unsigned long regaddr;
    unsigned char FFflag;
	
    BUFNum-=1;		                                                            /* 计算地址方便                 */
	
    regaddr = (unsigned long)(&LPC_CAN1->TFI1)+Ch*CANOFFSET+BUFNum*0X10;	    /* 3缓冲区间地址差 0x10         */
    CAN32reg = RGE(regaddr);
    CAN32reg &= ~((0x0fL<<16) | (0x01L<<30) | (0x80000000));                  /* 清 DLC,RTR.FF位	            */
    CAN32reg |= ((unsigned long)MessageDetailT.LEN<<16) | ((unsigned long)MessageDetailT.FF<<31);
    RGE(regaddr)=CAN32reg;
	
    FFflag = MessageDetailT.FF;
	
    regaddr = (unsigned long)(&LPC_CAN1->TID1)+Ch*CANOFFSET+BUFNum*0X10;	    /* 写帧ID                       */
    CAN32reg = RGE(regaddr);
    if(FFflag){								                                                /* FF为1，ID为29位              */
        CAN32reg &=0x70000000;
        CAN32reg |= (MessageDetailT.CANID & 0x1fffffff);
    }  else{
        CAN32reg &= 0xfffff800;	                                              /* FF为0 ，ID为11位             */
        CAN32reg |= (MessageDetailT.CANID & 0x000007ff);
    }
    RGE(regaddr)=CAN32reg;
    regaddr = (unsigned long)(&LPC_CAN1->TDA1)+Ch*CANOFFSET+BUFNum*0X10;      /* 写帧数据A                    */
    RGE(regaddr) = MessageDetailT.DATAA;
	
    regaddr = (unsigned long)(&LPC_CAN1->TDB1)+Ch*CANOFFSET+BUFNum*0X10;	    /* 写帧数据B                    */
    RGE(regaddr) = MessageDetailT.DATAB;
	
    regaddr = (unsigned long)(&LPC_CAN1->CMR)+Ch*CANOFFSET;                   /* 写控制寄存器，发送           */
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
    regaddr = (unsigned long)(&LPC_CAN1->GSR)+Ch*CANOFFSET;                   /* 查询发送状态                 */
    CAN32reg = RGE(regaddr);

    if(CAN32reg&(1<<3))	{                                               /* 所有的请求已成功完成         */
        return(1);                                                      /* 发送成功返回 1               */
    } else { 
        return (0);                                                     /* 发送失败返回 0               */
    }	
}
/*********************************************************************************************************
** Functoin name:               WriteCAN0
** Descriptions:  	            用户填写发送 帧信息
** input paraments:	            LEN:    数据长度
**				                      *data:  发送数据所在的数组 
**                             	FF:
**							              	ID:
** output paraments:	        	无			
** Returned values:	   		    	0：     失败
**								              1:      成功
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
        MessageDetailT.DATAA=0;			                                /* 先清零                                                                */
        MessageDetailT.DATAB=0;
	
        for(i=0;i<LEN;i++) {
            if(i<4){
                MessageDetailT.DATAA |= (*(data+i)<<(i*8)); 
            }  else {
                MessageDetailT.DATAB |= (*(data+i)<<((i-4)*8));
            }
        }
				CANSend(0, 1);
       OSTimeDlyHMSM(0, 0, 0, 5);                                   /*  防止发送过快 ,缓冲区未发送完毕又写入下一帧数据，不能发送               */
	 return(1);
    }
		
}

/*********************************************************************************************************
** Functoin name:               WriteCAN1
** Descriptions:  	            用户填写发送 帧信息
** input paraments:	            LEN:    数据长度
**				                      *data:  发送数据所在的数组 
**                             	FF:
**							              	ID:
** output paraments:	        	无	
** Returned values:	   		    	0：     失败
**								              1:      成功
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
        MessageDetailT.DATAA=0;			                                /* 先清零                       */
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
** Descriptions:  	            CAN控制器中断处理函数
** input paraments:	            无
** output paraments:	    	无	
** Returned values:	   			无
*********************************************************************************************************/
void CAN_IRQHandler (void)
{
    unsigned char j;
    unsigned int CAN32reg,mes;
    unsigned int regaddr;

    /*
     * 最大CAN通道数为2，分别检测两个通道的中断   
     */
    for(j=0;j<2;j++){				
	
		regaddr = (unsigned long)(&LPC_CAN1->ICR)+j*CANOFFSET;	
		CAN32reg=RGE(regaddr);	
		
		if((CAN32reg&(1<<0))!= 0)	                                      /* RI 接收中断                  */
		{
			CAN32reg &= ~(0x01);
			RGE(regaddr) = CAN32reg;

       if(j==0)
       {					 
						CANRCV(j, pwMsgCAN0);                                          /* 收到CAN0中断,接收帧 放进缓冲区等待任务取出去处理               */         			 
	          if(pwMsgCAN0 == &MsgCAN0BUF[CAN_MSG_NUMBER-2])                
					  { 
                 pwMsgCAN0 = MsgCAN0BUF;							                     /* 调整指针指向                                                   */
						}	
				   else                                                            /* 留出一个存储单元，防止读指针在指向最后一个存储单元又做加法     */
					  { 
                 pwMsgCAN0++;                                              /* 指针递增必须放在最后，防止指针递增后进行重复的处理             */
						}	
						
											
		         /* 填写接收帧信息*/
		       MessageDetailT.LEN=MessageCAN0.LEN;
		       MessageDetailT.FF=MessageCAN0.FF;
		       MessageDetailT.CANID=MessageCAN0.CANID;
		     
 
//         CANRCV(j, &MessageCAN0); 
//			   CAN0_RECData_Pro() ;				 
			   regaddr = (unsigned long)(&LPC_CAN1->CMR)+j*CANOFFSET;				
			   mes=RGE(regaddr);
			   mes |= (1<<2);										                          /* 释放接收缓冲区               */
			   RGE(regaddr)=mes;
			}
			
			if(j==1)
      {
			   CANRCV(j, &MessageCAN1);                      		        	/* 收到CAN1中断,接收帧          */
			        /* 填写接收帧信息*/
//			     MessageDetailT.LEN=MessageCAN1.LEN;
//		       MessageDetailT.FF=MessageCAN1.FF;
//		       MessageDetailT.CANID=MessageCAN1.CANID;
//               /* 中断服务函数*/
//						CAN1_RECData_Pro();
			   regaddr = (unsigned long)(&LPC_CAN1->CMR)+j*CANOFFSET;				
			   mes=RGE(regaddr);
			   mes |= (1<<2);								            /* 释放接收缓冲区               */
			   RGE(regaddr)=mes;
			}	
		}                
		
	}		
}

/*********************************************************************************************************
** Functoin name:               CAN0_RECData_Pro
** Descriptions:  	            CAN控制器中断处理函数
** input paraments:	            无
** output paraments:	    	无	
** Returned values:	   			无
*********************************************************************************************************/
void CAN0_RECData_Pro(void)
{
	 
	 //移到  BMSMain任务下的   Charger_RECData_Pro  函数中
}

/*********************************************************************************************************
** Functoin name:               CAN_IRQHandler
** Descriptions:  	            CAN控制器中断处理函数
** input paraments:	            无
** output paraments:	    	无	
** Returned values:	   			无
*********************************************************************************************************/
void CAN1_RECData_Pro(void)
{
	
}
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
