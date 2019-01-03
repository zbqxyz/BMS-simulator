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
		FramWrite((uint8_t *)&BMSMessage,BMS_MSG_PARA_ADDR,sizeof(BMSMessage));       //´æ´¢²ÎÊýµ½Ìúµç		
				
}

void BMSMain (void)
{   
  
		
		Charger_RECData_Pro();                                     // ´¦Àí³äµç×®¹ýÀ´µÄÊý¾Ý   
	
/*          ³äµç½×¶Î´¦Àíº¯Êý                 */
	switch (BMSMessage.ChargeStage)
	{
 		
		case 1:	 	
		 	  if((BMSMessage.BMSStopFlag==1)&&(SysTickCnt%10==0))    //Í£»úÊ± 10ms·¢ËÍÒ»´Î
			   BMS_BST(); 
	       break; 
				
				
	  case 2:			
	 		 	 if((BMSMessage.BMSStopFlag==1)&&(SysTickCnt%10==0))    //Í£»úÊ± 10ms·¢ËÍÒ»´Î
			   BMS_BST(); 
					break;
	 
		case 3:	 						    
	     	if (SysTickCnt%250==0)            //µç³Ø³äµç×Ü×´Ì¬£¬250ms·¢ËÍÒ»´Î  
		    {
					BMS_BCS();
					BMS_BSM();
		    }
				else if (SysTickCnt%50==0)       //³äµçÐèÇó50ms·¢ËÍÒ»´Î
		    {
					BMS_BCL();	                     
		     }
/*                       ¸ºÔØ¿ªÆôÌõ¼þ                   */				
				if(BMSMessage.RequestVoltage*10>ChargerMsg.ChargeVoltage)
					{
			    	if(BMSMessage.RequestVoltage*10-ChargerMsg.ChargeVoltage<=200)  //µÍÓÚÐèÇóµçÑ¹20VÔò¿ÉÒÔ½øÐÐ¿ªÆô¸ºÔØ
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
			    	if(ChargerMsg.ChargeVoltage-BMSMessage.RequestVoltage*10<=100)  //³¬¹ýÐèÇóµçÑ¹µÄ10VÒÔÄÚ¿ÉÒÔ¿ªÆô¸ºÔØ
			    	{
              Battery_SWITCH_OFF();
					    DC_SWITCH_ON();								
				    } 
			  	else
				 {
				  	DC_SWITCH_OFF();			
				  }
					}
				
				if(((ChargedTime/60)>=Sys_PARA.ChargeLimitedTime)&&(Sys_PARA.TimeChargeMode==1))                      //Ê±¼ä³äµçÄ£Ê½£¬µ±Ê±¼äµ½ÁË,ÔòÍ£»ú
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
		     if((BMSMessage.BMSStopFlag==1)&&(SysTickCnt%10==0))    //Í£»úÊ± 10ms·¢ËÍÒ»´Î
			   BMS_BST(); 
				 DC_SWITCH_OFF();
	       break;
				 
		default:
			  SysTickCnt= 0 ;
	    	BMSMessage.ChargeStage=0;
         break;	 
	 }		
}

	
void BMS_BHM(void)    		           //    ÎÕÊÖ    ÊÕµ½BMS ¶¯Êý¾Ý´«ÊäÐ­ÒéTCPM£¨ÓÉÓÚÊý¾Ý³¤¶È´óÓÚ8£¬¹²41£©´«Êäµç³Ø×éÉí·Ý±àÂëÐÅÏ¢BRM
{
   CANID=0x182756F4 ;
	 CAN_Data[0]=0x4c ;                //±æÊ¶
	 CAN_Data[1]=0x1d ;                //µÚ2¸ö×Ö½Ú ³äµç»ú±àºÅ 00--100   ;               //³äµç»ú±àºÅ£º1~100
	 WriteCAN0(2,1, CANID,CAN_Data);	 
}	

void BMS_BRM(void)    		           //³äµç»ú±æÊ¶±¨ÎÄ    ÎÕÊÖ½×¶Î 1²½    ÊÕµ½BMS ¶¯Êý¾Ý´«ÊäÐ­ÒéTCPM£¨ÓÉÓÚÊý¾Ý³¤¶È´óÓÚ8£¬¹²41£©´«Êäµç³Ø×éÉí·Ý±àÂëÐÅÏ¢BRM
{
   CANID=0x1cec56f4  ;
 		
	 CAN_Data[0]=0x10 ;                    //±æÊ¶
	 CAN_Data[1]=0x31 ;                               
	 CAN_Data[2]=0x00 ;                    //³äµç»ú³äµçÕ¾ËùÔÚÇøÓò±àÂë£¨±ê×¼ASCII£©
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
	
	 CAN_Data[0]=0x01 ;               //±æÊ¶  01 00 01 00 04 34 08 70 
	 CAN_Data[1]=0x00 ;               //µÚ2¸ö×Ö½Ú ³äµç»ú±àºÅ 00--100   ;               //³äµç»ú±àºÅ£º1~100
	 CAN_Data[2]=0x01 ;               //³äµç»ú³äµçÕ¾ËùÔÚÇøÓò±àÂë£¨±ê×¼ASCII£©
	 CAN_Data[3]=0x00 ;
	if(0<<BMSMessage.BatteryType<<9)
	 CAN_Data[4]=BMSMessage.BatteryType;               //µç³ØÀàÐÍ
	else
	 CAN_Data[4]=BMSMessage.BatteryType;               //ÆäËûµç³Ø
	 CAN_Data[5]=0xf0 ;
	 CAN_Data[6]=0x0a ;
	 CAN_Data[7]=0x70 ;
	 WriteCAN0(8,1, CANID,CAN_Data);	
	 OSTimeDlyHMSM(0, 0, 0, 5);           //¹ÒÆð5ms£¬ÒÔ±ãÆäËûÏß³Ì
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
	 OSTimeDlyHMSM(0, 0, 0, 5);           //¹ÒÆð5ms£¬ÒÔ±ãÆäËûÏß³Ì
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
	 OSTimeDlyHMSM(0, 0, 0, 5);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³Ì
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
	 OSTimeDlyHMSM(0, 0, 0, 5);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³Ì
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
	 OSTimeDlyHMSM(0, 0, 0, 5);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³Ì
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
	 OSTimeDlyHMSM(0, 0, 0, 5);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³Ì
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
void BMS_BCP(void)		// µç³Ø²ÎÊý ¶¯Á¦Ðîµç³Ø³äµç²ÎÊý±¨ÎÄ  10 0d 00 02 ff 00 06 00 
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

void BMS_BCP_DATA_01(void)		// µç³Ø²ÎÊý ¶¯Á¦Ðîµç³Ø³äµç²ÎÊý±¨ÎÄ  01 77 01 e8 03 23 02 7C
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
	OSTimeDlyHMSM(0, 0, 0, 500);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³ÌÔËÐÐ

}

void BMS_BCP_DATA_02(void)		// µç³Ø²ÎÊý ¶¯Á¦Ðîµç³Ø³äµç²ÎÊý±¨ÎÄ 02 15 78 9B 01 7C 12 ff 
{
	
	 CANID=0x1ceb56f4 ;
	 CAN_Data[0]=0x02 ;
	 CAN_Data[1]=0x1d ;
	 CAN_Data[2]=0x78 ;
	 CAN_Data[3]=BMSMessage.SOC*10%256;
	 CAN_Data[4]=BMSMessage.SOC*10>>8;
	 CAN_Data[5]=BMSMessage.BatteryVoltage*10%256;          //µç³ØµçÑ¹Ä¬ÈÏ328V
	 CAN_Data[6]=BMSMessage.BatteryVoltage*10>>8;  
	 CAN_Data[7]=0xff ;			
	 WriteCAN0(8,1, CANID,CAN_Data);

}

void BMS_BRO(uint8_t data)	           	// µç³Ø²ÎÊý ¶¯Á¦Ðîµç³Ø³äµç²ÎÊý±¨ÎÄ aa
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
   OSTimeDlyHMSM(0, 0, 0, 250);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³ÌÔËÐÐ
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
   CAN_Data[4]=BMSMessage.ChargeMode;               //³äµçÄ£Ê½;
	else
	 CAN_Data[4]=0x02 ;                               //Ä¬ÈÏºãÁ÷Ä£Ê½
	
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
	 CAN_Data[5]=0x49 ;
	 CAN_Data[6]=0x11 ;	
	 CAN_Data[7]=BMSMessage.SOC;	 			
	 WriteCAN0(8,1, CANID,CAN_Data);
	OSTimeDlyHMSM(0, 0, 0, 250);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³ÌÔËÐÐ
}

void BMS_BCS_Data_02(void)		
{
	 CANID=0x1ceb56f4  ;
	 CAN_Data[0]=0x02 ;
	 CAN_Data[1]=0x3c ;
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
	 CAN_Data[0]=0x00 ;
	 CAN_Data[1]=0x32 ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0x32 ;
	 CAN_Data[4]=0x00 ;
	 CAN_Data[5]=0x00 ;
	 CAN_Data[6]=0x10 ;  
			
	 WriteCAN0(8,1, CANID,CAN_Data);
}

void BMS_BMV(void)		//¿ÉÑ¡±¨ÎÄ
{

}

void BMS_BMT(void)		//¿ÉÑ¡±¨ÎÄ
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
	 OSTimeDlyHMSM(0, 0, 0, 250);           //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³ÌÔËÐ
}


void BMS_BEM(void)	
{
	 CANID=0x081e56f4  ;
	 CAN_Data[0]=0x00 ;
	 CAN_Data[1]=0x00 ;
	 CAN_Data[2]=0x00 ;
	 CAN_Data[3]=0x00 ;
	 WriteCAN0(4,1, CANID,CAN_Data);
	 OSTimeDlyHMSM(0, 0, 0, 250);                 //¹ÒÆð100ms£¬ÒÔ±ãÆäËûÏß³ÌÔËÐ
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
						   BMS_BRO(0xAA) ;                                          //±ÕºÏK5,K6,¾øÔµ¼ì²â¿ªÆôºó BRO·¢AA
							 BMSMessage.ChargeStage= 2 ;
					
			       }
		        else if((MessageCAN0.DATAA&0xff)==0xaa)
		       	{
							 
			         BMS_BRO(0xAA) ;                                          //±ÕºÏK5,K6,¾øÔµ¼ì²â¿ªÆôºó BRO·¢AA
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
//		 if(BMSMessage.ChargeStage==2)
//		 {		 
//	      BMSMessage.ChargeStage= 3 ;
//		 }	 
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
							 //´Ë´¦¿ÉÔö¼ÓÒýµ¼µçÔ´µÄÐ¹·Å  ´Ë´¦¿ªÆô  È»ºóÔÚÎÕÊÖ½×¶Î½øÐÐ¹Ø±Õ
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
*	ÈÎÎñÑ­»·µÄ´¦ÀíËÙ¶È¸ú²»ÉÏÖÐ¶ÏµÄ´¦ÀíËÙ¶È£¬Òò´Ë×öÒ»¸ö»º³åÇø£¬ÓÉÖÐ¶ÏÐ´ÈëÊý¾Ý£¬ÈÎÎñÑ­»·´¦ÀíÊý¾Ý£¬Ã¿´¦ÀíÍêÒ»Ö¡Êý¾Ý£¬Ôò°Ñ¸ÃÕëÊý¾Ý¶ªµô¡£
* ÓÉÓÚÊý¾Ý»º³åÇøµÄÊý¾Ý³¤¶È´óÐ¡ÓÐÏÞ£¬µ±ÖÐ¶ÏµÄÐ´Ö¸ÕëÔ¶Ô¶¿ì¹ýÈÎÎñµÄ¶ÁÖ¸ÕëËÙ¶È£¬Ôò»¹ÊÇ»á³öÏÖ¶ª°üÏÖÏó¡£
***************************************************************************************************************************************/
  	if(prMsgCAN0==&MsgCAN0BUF[CAN_MSG_NUMBER-1])     /* Ô¤ÁôÒ»¸ö´æ´¢µ¥Ôª²»´æ´¢£¬·ÀÖ¹Ð´Ö¸ÕëÖ¸Ïò×îºóÒ»¸ö´æ´¢µ¥ÔªÊ±£¬¶ÁÖ¸ÕëµÝÔöÊ±Òç³ö    */
	  {
			MessageCAN0 = *prMsgCAN0;
			prMsgCAN0=MsgCAN0BUF;			                     /* µ÷ÕûÖ¸ÕëÖ¸Ïò £¬ÓÉÎ²°ÍÖ¸Ïò¿ªÊ¼                                                 */
	  }
	  else                                             
		{
	     MessageCAN0 = *prMsgCAN0;
		   memset(prMsgCAN0,0,sizeof(*prMsgCAN0));       /*´¦ÀíÍêÒ»Ö¡Êý¾Ý£¬¾Í¶ªµô¸Ã°üÊý¾Ý                                                 */
		   prMsgCAN0 ++;                                 /*Ö¸ÕëµÝÔö±ØÐë·ÅÔÚ×îºó£¬·ÀÖ¹Ö¸ÕëµÝÔöºó½øÐÐÖØ¸´µÄif´¦Àí                           */
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
	 MessageCAN0.CANID=0;  //Çå¿ÕCANµÄÊý¾Ý£¬ ·ÀÖ¹ÖØ¸´´¦Àí
}
