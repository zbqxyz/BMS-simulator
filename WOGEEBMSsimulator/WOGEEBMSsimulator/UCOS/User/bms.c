/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               bms.c
** Last modified Date:      2018.08.16
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <bms.h>

BMSMsg BMSMessage;
ChargeMsg ChargerMsg;
static void BMS_BHM(void);
static void BMS_BRM(void);
static void BMS_BRM_Msg01(void);
static void BMS_BRM_Msg02(void);
static void BMS_BRM_Msg03(void);
static void BMS_BRM_Msg04(void);
static void BMS_BRM_Msg05(void);
static void BMS_BRM_Msg06(void);
static void BMS_BRM_Msg07(void);
static void BMS_BCP(void);
static void BMS_BCP_DATA_01(void);
static void BMS_BCP_DATA_02(void);
static void BMS_BRO(uint8_t data);
static void BMS_BCL(void);
static void BMS_BCS(void);
static void BMS_BCS_Data_01(void);		
static void BMS_BCS_Data_02(void);
static void BMS_BSM(void);
static void BMS_BMV(void);
static void BMS_BMT(void);
static void BMS_BST(void);
static void BMS_BSD(void);
static void BMS_BEM(void);
//static void BMS_DATA_Packet_Analyse(void);
static void Charger_RECData_Pro(void);

void BMSParameterInit(void)
{	
		BMSMessage.RequestCurrent = 500;
		BMSMessage.RequestCurrent = 80 ;
		BMSMessage.BatteryVoltage = 320;
		BMSMessage.SOC            = 50;
		
		BMSMessage.CS = CheckSum((uint8_t *)&BMSMessage,(uint8_t *)&BMSMessage+sizeof(BMSMessage)-1);	
		FramWrite((uint8_t *)&BMSMessage,BMS_MSG_PARA_ADDR,sizeof(BMSMessage));       //存储参数到铁电						
}

void BMSMain (void)
{   
	 static uint16_t BROcnt,BCLcnt,BCScnt,BSMcnt;
	 static uint32_t BSTcnt = 0;
	 BMSMessage.ChargeStage=0;
	 BMSMessage.BMSStopFlag=0;
	
  while (1) 
 {						
    OSTimeDlyHMSM(0,0,0,1);	
		Charger_RECData_Pro();                                                       // 处理充电桩过来的数据 ，主要是被动处理函数  
	 
/*          充电阶段处理函数 ,主要是模拟器这边的主动处理函数                */
	switch (BMSMessage.ChargeStage)
	{ 		
		case 1:	 	                           //握手阶段
	       break; 				
				
	  case 2:			                         //配置阶段
			
					break;
	 
		case 3:	 	                           //充电阶段
         BCScnt++;
		     BSMcnt++;
		     BCLcnt++;
	     	if (BCScnt>=250)                 //电池充电总状态，250ms发送一次  
		    {       
					BMS_BCS();
					BCScnt=0;
		    }	
      	if (BSMcnt>=250)                 //电池充电总状态，250ms发送一次  
		    {
        	BMS_BSM(); 
					BSMcnt=0;
		    }
				else if (BCLcnt>=50)             //充电需求50ms发送一次
		    {
					BMS_BCL();	 
         	BCLcnt=0;				
		     }
/*                       负载开启条件                   */				
				if(BMSMessage.RequestVoltage*10>ChargerMsg.ChargeVoltage)
					{
			    	if(BMSMessage.RequestVoltage*10-ChargerMsg.ChargeVoltage<=200)  //低于需求电压20V则可以进行开启负载
			    	{  
							Battery_SWITCH_OFF();							
					    DC_SWITCH_ON();	
				    }
			  	else
				    {
				  	  DC_SWITCH_OFF();			
				    }
					}
				else if(ChargerMsg.ChargeVoltage>BMSMessage.RequestVoltage*10) 
					{
			    	if(ChargerMsg.ChargeVoltage-BMSMessage.RequestVoltage*10<=100)  //超过需求电压的10V以内可以开启负载
			    	{
              Battery_SWITCH_OFF();
					    DC_SWITCH_ON();								
				    } 
			  	else
				 {
				  	DC_SWITCH_OFF();			
				  }
					}
				
				if(((ChargedTime/60)>=Sys_PARA.ChargeLimitedTime)&&(Sys_PARA.TimeChargeMode==1))                      //时间充电模式，当时间到了,则停机
				{
					 BMSMessage.BMSStopFlag=1;
				  Sys_PARA.TimeChargeMode=0;
				}
				if((BMSMessage.SOC>=100)&&(Sys_PARA.SOCChargeMode==1))
				{
				  	BMSMessage.SOC=0;
				   BMSMessage.BMSStopFlag=1;
				}				
          break;		
				
		case 4:
			   BSTcnt++;
    	if((BMSMessage.BMSStopFlag==1)&&(BSTcnt%10==0))         //停机时 10ms发送一次
		  	 BMS_BST();			
				 DC_SWITCH_OFF();
	       break;
				 
		default:
         break;	 
	 }	
 }	
}
	
void BMS_BHM(void)    		           //    握手    收到BMS 动数据传输协议TCPM（由于数据长度大于8，共41）传输电池组身份编码信息BRM
{
   CANID=0x182756F4 ;
	 CAN_Data[0]=(BMSMessage.MAXVoltage*10)&0xff ;                //辨识
	 CAN_Data[1]=(BMSMessage.MAXVoltage*10)>>4 ;                //第2个字节 充电机编号 00--100   ;               //充电机编号：1~100
	 WriteCAN0(2,1, CANID,CAN_Data);	 
}	

void BMS_BRM(void)    		           //充电机辨识报文    握手阶段 1步    收到BMS 动数据传输协议TCPM（由于数据长度大于8，共41）传输电池组身份编码信息BRM
{
   CANID=0x1cec56f4  ;
 		
	 CAN_Data[0]=0x10 ;                    //辨识
	 CAN_Data[1]=0x31 ;                               
	 CAN_Data[2]=0x00 ;                    //充电机充电站所在区域编码（标准ASCII）
	 CAN_Data[3]=0x07 ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0x00 ;
	 CAN_Data[6]=0x02 ;
	 CAN_Data[7]=0x00 ;
	 WriteCAN0(8,1, CANID,CAN_Data);
}	

void BMS_BRM_Msg01(void)
{
  CANID=0x1ceb56f4 ;	
	
	 CAN_Data[0]=0x01 ;               //辨识  01 00 01 00 04 34 08 70 
	 CAN_Data[1]=0x00 ;               //第2个字节 充电机编号 00--100   ;               //充电机编号：1~100
	 CAN_Data[2]=0x01 ;               //充电机充电站所在区域编码（标准ASCII）
	 CAN_Data[3]=0x00 ;
	 CAN_Data[4]=BMSMessage.BatteryType;               //电池类型
	 CAN_Data[5]=0xf0 ;
	 CAN_Data[6]=0x0a ;
	 CAN_Data[7]=0x70 ;
	 WriteCAN0(8,1, CANID,CAN_Data);	
}	

void BMS_BRM_Msg02(void)
{
  CANID=0x1ceb56f4 ;		
	 CAN_Data[0]=0x02 ;                 
	 CAN_Data[1]=0x17 ;                                  
	 CAN_Data[2]=0xff ;               
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ; 				
	 WriteCAN0(8,1, CANID,CAN_Data);
}	

void BMS_BRM_Msg03(void)
{
  CANID=0x1ceb56f4 ;	
	
	 CAN_Data[0]=0x03 ;                
	 CAN_Data[1]=0xff ;                                 
	 CAN_Data[2]=0xff ;                
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ;  		
	 WriteCAN0(8,1, CANID,CAN_Data);
	 OSTimeDly(10);
}	

void BMS_BRM_Msg04(void)
{
  CANID=0x1ceb56f4 ;	
	
	 CAN_Data[0]=0x04 ;                
	 CAN_Data[1]=0xff ;                                 
	 CAN_Data[2]=0xff ;                
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ;  			
	 WriteCAN0(8,1, CANID,CAN_Data);
}

void BMS_BRM_Msg05(void)
{
  CANID=0x1ceb56f4 ;	
	
	 CAN_Data[0]=0x05 ;                
	 CAN_Data[1]=0xff ;                                 
	 CAN_Data[2]=0xff ;                
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ;  		
	 WriteCAN0(8,1, CANID,CAN_Data);
}

void BMS_BRM_Msg06(void)
{
   CANID=0x1ceb56f4 ;		
	 CAN_Data[0]=0x06 ;                
	 CAN_Data[1]=0xff ;                                 
	 CAN_Data[2]=0xff ;                
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ;  	
	 WriteCAN0(8,1, CANID,CAN_Data);
}
void BMS_BRM_Msg07(void)
{  
	CANID=0x1ceb56f4 ;		
	 CAN_Data[0]=0x07 ;                
	 CAN_Data[1]=0xff ;                                 
	 CAN_Data[2]=0xff ;                
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ;  	
	 WriteCAN0(8,1, CANID,CAN_Data);
}
void BMS_BCP(void)		// 电池参数 动力蓄电池充电参数报文  10 0d 00 02 ff 00 06 00 
{
	 CANID=0x1cec56f4  ;
	 CAN_Data[0]=0x10 ;
	 CAN_Data[1]=0x0d ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0x02 ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0x00 ;
	 CAN_Data[6]=0x06 ;
	 CAN_Data[7]=0x00 ;		
	 WriteCAN0(8,1, CANID,CAN_Data);
}

void BMS_BCP_DATA_01(void)		// 电池参数 动力蓄电池充电参数报文  01 77 01 e8 03 23 02 7C
{
	 CANID=0x1ceb56f4  ;
	 CAN_Data[0]=0x01 ;
	 CAN_Data[1]=0x77 ;
	 CAN_Data[2]=0x01 ;
	 CAN_Data[3]=0xA8 ;
	 CAN_Data[4]=0x03 ;
	 CAN_Data[5]=0x23 ;
	 CAN_Data[6]=0x02 ;
	 CAN_Data[7]=0x4c ;		
	 WriteCAN0(8,1, CANID,CAN_Data);

}

void BMS_BCP_DATA_02(void)		// 电池参数 动力蓄电池充电参数报文 02 15 78 9B 01 7C 12 ff 
{
	
	 CANID=0x1ceb56f4 ;
	 CAN_Data[0]=0x02 ;
	 CAN_Data[1]=0x1d ;
	 CAN_Data[2]=0x78 ;
	 CAN_Data[3]=BMSMessage.SOC*10%256;
	 CAN_Data[4]=BMSMessage.SOC*10>>8;
	 CAN_Data[5]=BMSMessage.BatteryVoltage*10%256;          //电池电压默认328V
	 CAN_Data[6]=BMSMessage.BatteryVoltage*10>>8;  
	 CAN_Data[7]=0xff ;			
	 WriteCAN0(8,1, CANID,CAN_Data);

}

void BMS_BRO(uint8_t data)	           	// 电池参数 动力蓄电池充电参数报文 aa
{
	 CANID=0x100956f4  ;
	if(data==00)
	{
	 CAN_Data[0]=0x00 ;
	}
	else
	{
	 CAN_Data[0]=0xaa ;
	}
	 WriteCAN0(1,1, CANID,CAN_Data);
	OSTimeDlyHMSM(0,0,0,250);
}

void BMS_BCL(void)		
{	
	 CANID=0x181056f4  ;
	if((BMSMessage.RequestVoltage*10<0xbb8)||(BMSMessage.RequestVoltage*10>0x1d4c))
	{
	  CAN_Data[0]=0xb8 ;
	  CAN_Data[1]=0x0b ;
	}else
	{
		CAN_Data[0]=(BMSMessage.RequestVoltage*10)%256;
  	CAN_Data[1]=(BMSMessage.RequestVoltage*10)>>8;
	}
	
	if(((4000-BMSMessage.RequestCurrent*10)>0xfa0)||((4000-BMSMessage.RequestCurrent*10)<0x5dc))
	{
	  CAN_Data[2]=0x80 ;
	  CAN_Data[3]=0x0c ;
	}else
	{
  	CAN_Data[2]=(4000-BMSMessage.RequestCurrent*10)%256;
	  CAN_Data[3]=(4000-BMSMessage.RequestCurrent*10)>>8;
	}
	if(0<<BMSMessage.ChargeMode<<3)
   CAN_Data[4]=BMSMessage.ChargeMode;               //充电模式;
	else
	 CAN_Data[4]=0x02 ;                               //默认恒流模式
	
	 WriteCAN0(5,1, CANID,CAN_Data);
}
void BMS_BCS(void)		
{
	 CANID=0x1cec56f4 ;
	 CAN_Data[0]=0x10 ;
	 CAN_Data[1]=0x09 ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0x02 ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0x00 ;
	 CAN_Data[6]=0x11 ;
	 CAN_Data[7]=0x00 ;  			
	 WriteCAN0(8,1, CANID,CAN_Data);
}
void BMS_BCS_Data_01(void)		
{
	 CANID=0x1ceb56f4 ;
	 CAN_Data[0]=0x01 ;	
	 CAN_Data[1]= ChargerMsg.ChargeVoltage*10%256;
	 CAN_Data[2]=(ChargerMsg.ChargeVoltage*10>>8)&0xff;
	 CAN_Data[3]=(4000-ChargerMsg.ChargeCurrent*10)%256 ;
	 CAN_Data[4]=((4000-ChargerMsg.ChargeCurrent*10)>>8)&0xff ;
	 CAN_Data[5]=(BMSMessage.MAXSingleVoltage&0xf00)>>8;
	 CAN_Data[6]=BMSMessage.MAXSingleVoltage%256;	
	 CAN_Data[7]=BMSMessage.SOC;	 			
	 WriteCAN0(8,1, CANID,CAN_Data);
 
}

void BMS_BCS_Data_02(void)		
{
	 CANID=0x1ceb56f4  ;
	 CAN_Data[0]=Sys_PARA.ChargeLimitedTime%256 ;
	 CAN_Data[1]=(Sys_PARA.ChargeLimitedTime>>8)&0xff ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0xff ;
	 CAN_Data[4]=0xff ;
	 CAN_Data[5]=0xff ;
	 CAN_Data[6]=0xff ;
	 CAN_Data[7]=0xff ; 			
	 WriteCAN0(8,1, CANID,CAN_Data);       
}

void BMS_BSM(void)		
{
	 CANID=0x181356f4 ;
	 CAN_Data[0]=0x08 ;
	 CAN_Data[1]=BMSMessage.MAXSingleVoltageTemp+50 ;
	 CAN_Data[2]=0x12 ;
	 CAN_Data[3]=BMSMessage.MINSingleVoltageTemp+50 ;
	 CAN_Data[4]=0x20 ;
	 CAN_Data[5]=0x00 ;
	 CAN_Data[6]=0x10 ;  
			
	 WriteCAN0(8,1, CANID,CAN_Data);
}

void BMS_BMV(void)		//可选报文
{

}

void BMS_BMT(void)		//可选报文
{

}

void BMS_BST(void)		
{
	 CANID=0x101956f4  ;
	 CAN_Data[0]=0x00 ;
	 CAN_Data[1]=0x00 ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0x00 ;
	 WriteCAN0(4,1, CANID,CAN_Data);
}

 
void BMS_BSD(void)		
{
	 CANID=0x181c56f4  ;
	 CAN_Data[0]=0x41 ;
	 CAN_Data[1]=0x95 ;
	 CAN_Data[2]=0x01 ;
	 CAN_Data[3]=0x99 ;
	 CAN_Data[4]=0x01 ;
	 CAN_Data[5]=0x3d ;
	 CAN_Data[6]=0x3e ;
	 WriteCAN0(8,1, CANID,CAN_Data);

}


void BMS_BEM(void)	
{
	 CANID=0x081e56f4  ;
	 CAN_Data[0]=0x00 ;
	 CAN_Data[1]=0x00 ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0x00 ;
	 WriteCAN0(4,1, CANID,CAN_Data);

}	
	
void CHM_Analyse(void)	
{
	     if(PRINT_STRING)
	     Print("%s\n", "chargestate 1 ,  handshake !");  
	     BMSMessage.ChargeStage= 1 ;            
	     BMS_BHM();
}		
	   
void CRM_Analyse(void)	
{
	         	                  
			    if((MessageCAN0.DATAA&0xff)==0x00)
			     {	    
						      BMSMessage.ChargeStage= 1 ; 
			             BMS_BRM();
			      }					 
	    		else if(((MessageCAN0.DATAA&0xff)==0xaa)&&( BMSMessage.ChargeStage==1))
			      {							 
							    BMS_BCP();
							    if(PRINT_STRING)
					      	Print("%s\n", "chargestate 2 ,   configure !"); 
		              BMSMessage.ChargeStage= 2 ;
		       	}
}		
	
void CTS_Analyse(void)	
{
}			
	
void CML_Analyse(void)	
{
     if(BMSMessage.ChargeStage==2)
		 {
             Battery_SWITCH_ON();
	           BMS_BRO(0xAA) ;
		 }
}			
	
void CRO_Analyse(void)	
{	
	   if(BMSMessage.ChargeStage==2)
		 {
	        	if((MessageCAN0.DATAA&0xff)==0x00)
			      {
						   BMS_BRO(0xAA) ;                                          //闭合K5,K6,绝缘检测开启后 BRO发AA
							 BMSMessage.ChargeStage= 2 ;
					
			       }
		        else if((MessageCAN0.DATAA&0xff)==0xaa)
		       	{							 
			         BMS_BRO(0xAA) ;                                          //闭合K5,K6,绝缘检测开启后 BRO发AA
							 if(PRINT_STRING)
							 Print("%s\n", "chargestate 3 ,   charging !");
		           BMSMessage.ChargeStage= 3 ;
							 ChargedTime=0;							 
							 SOChargedTime=0;
							 BMS_BCL();
					     BMS_BCS();
							 FAN_SWITCH_ON();
			       }	
			}
}	
void CCS_Analyse(void)	
{ 
 
}	

void CST_Analyse(void)	
{
               BMS_BST(); 	   
               BMS_BSD();		
		           Battery_SWITCH_OFF();
				  	   DC_SWITCH_OFF();	
	             FAN_SWITCH_OFF();
							 BMSMessage.ChargeStage= 4 ;	  
	             BMSMessage.BMSStopFlag=0;	
	             if(PRINT_STRING)	
	             Print("%s\n", "chargestate 4 ,   Charger Stop !");
							 //此处可增加引导电源的泄放  此处开启  然后在握手阶段进行关闭
}	

void CSD_Analyse(void)	
{
						    BMS_BSD();
	              if(PRINT_STRING)
                Print("%s\n", "Charged   !");					
                BMS_BEM();	
	              BMSMessage.BMSStopFlag=1;
								BMSMessage.ChargeStage= 0;
}	

void CEM_Analyse(void)	
{					
                BMS_BEM();
                BMSMessage.ChargeStage= 0;	
}	

void BMS_DATA_Packet_Analyse(void)	
{								
	             if(PRINT_STRING)
                Print("%s\n", "Debug  mark  1 ");									
						if(((MessageCAN0.DATAA&0xff)==0x11)&&((MessageCAN0.DATAB&0xff0000)==0x020000)&&(BMSMessage.ChargeStage= 1))
						{		
               if(PRINT_STRING)
                Print("%s\n", "Debug  mark  2 ");								
							BMS_BRM_Msg01() ;
						  BMS_BRM_Msg02() ;
						  BMS_BRM_Msg03() ;
						  BMS_BRM_Msg04() ;
						  BMS_BRM_Msg05() ;
						  BMS_BRM_Msg06() ;					
					    BMS_BRM_Msg07() ;
					
						}
						if(((MessageCAN0.DATAA&0xff)==0x11)&&((MessageCAN0.DATAB&0xff0000)==0x060000)&&(BMSMessage.ChargeStage= 2))
						{
							BMS_BCP_DATA_01();
						  BMS_BCP_DATA_02();
					
						}
			     		
						if(((MessageCAN0.DATAA&0xff)==0x11)&&((MessageCAN0.DATAB&0xff0000)==0x110000)&&(BMSMessage.ChargeStage= 3))
						{
						BMS_BCS_Data_01();
						BMS_BCS_Data_02();
				
						}
			    					
}	

void Charger_RECData_Pro(void)
{
/***************************************************************************************************************************************
*	任务循环的处理速度跟不上中断的处理速度，因此做一个缓冲区，由中断写入数据，任务循环处理数据，每处理完一帧数据，则把该针数据丢掉。
* 由于数据缓冲区的数据长度大小有限，当中断的写指针远远快过任务的读指针速度，则还是会出现丢包现象。
***************************************************************************************************************************************/
  	if(prMsgCAN0==&MsgCAN0BUF[CAN_MSG_NUMBER-1])     /* 预留一个存储单元不存储，防止写指针指向最后一个存储单元时，读指针递增时溢出    */
	  {
			MessageCAN0 = *prMsgCAN0;
			prMsgCAN0=MsgCAN0BUF;			                     /* 调整指针指向 ，由尾巴指向开始                                                 */
	  }
	  else                                             
		{
	     MessageCAN0 = *prMsgCAN0;
		   memset(prMsgCAN0,0,sizeof(*prMsgCAN0));       /*处理完一帧数据，就丢掉该包数据                                                 */
		   prMsgCAN0 ++;                                 /*指针递增必须放在最后，防止指针递增后进行重复的if处理                           */
	  }
			
	switch(MessageCAN0.CANID)
	 {
		 case 0x1826f456:			      
        CHM_Analyse();	       		 
		    break;
		 case 0x1801f456:
        CRM_Analyse();
			  break;
		 case 0x1807f456:	
        CTS_Analyse();			 
			  break;
		 case 0x1808f456:
			   CML_Analyse();
			  break;
		 case 0x100af456:	       
		     CRO_Analyse();
			 break;
		 case 0x1812f456:
			   CCS_Analyse();
			   break;
		 case 0x1cecf456:
			 	 	BMS_DATA_Packet_Analyse();			 
			 break;		 
		 case 0x101af456:
         CST_Analyse();
			 break;	
		 case 0x181df456:
         CSD_Analyse();	
		    break;
		 case 0x081ff456:	
         CEM_Analyse();	
         break;		 
		 default:
			 break ;		 				 
	 }
	 MessageCAN0.CANID=0;  //清空CAN的数据， 防止重复处理
}
