/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               display.c
** Last modified Date:      2018.08.21
** Last Version:            1.0
** Description:             
*********************************************************************************************************/

#include <display.h>
#include <main.h>

DwinMsg RDwinMessage;
DwinMsg WDwinMessage;
DisplayMsg DisplayMessage;

uint8_t Power_RHT[]={"有"};
uint8_t Power_ERR[]={"没有"};
uint8_t pChargeMode1[]={"恒压"};
uint8_t pChargeMode2[]={"恒流"};
uint8_t pBatteryType1[]={"铅酸电池"};
uint8_t pBatteryType2[]={"镍氢电池"};
uint8_t pBatteryType3[]={"磷酸铁锂"};
uint8_t pBatteryType4[]={"锰酸锂电"};
uint8_t pBatteryType5[]={"钴酸锂电"};
uint8_t pBatteryType6[]={"三元锂电"};
uint8_t pBatteryType7[]={"聚合离子"};
uint8_t pBatteryType8[]={"钛酸锂电"};
uint8_t pBatteryType9[]={"其他电池"};

static void ReadDataProcess(void);
static void DisplayPage(void);
static void DisplayVariableValue(void);
static void Dwin_ChangePage_Frame(unsigned short addr);
static void Dwin_ChangeVariableValue_Frame1(unsigned short addr,uint16_t dat );
static void Dwin_ChangeVariableValue_Frame2(unsigned short addr,uint32_t dat );
static void Dwin_ChangeVariableValue_FrameN(unsigned short addr,uint8_t* data,uint8_t n);
static void DisplayText(uint16_t addr,uint8_t *text,uint8_t len);
/*********************************************************************************************************
** Function name:       DisplayMain
** Descriptions:      
** input parameters:   
** output parameters:   
** Returned value:      
*********************************************************************************************************/

void DisplayMain (void)
{  		     
	    ReadDataProcess();	     
	    DisplayVariableValue();	
}	

/*********************************************************************************************************
**	函数：Dwin屏返回数据处理
*********************************************************************************************************/
static void ReadDataProcess(void)
{
	  uint8_t i;
	  
	  uint8_t ascbuf[34];
	
		OSTimeDly(OS_TICKS_PER_SEC/100); 
		
	  memset(&RDwinMessage.Data[0],0,sizeof(RDwinMessage.Data));
		for(i=0;i<RcvLenU1+1;i++)
		{									
			if(RcvBufU1[i]==0x5a&&RcvBufU1[i+1]==0xa5&&RcvBufU1[i+3]==0x83)               //收到数据帧解析
			{
				RDwinMessage.Head1=RcvBufU1[i];
				RDwinMessage.Head2=RcvBufU1[i+1];
				RDwinMessage.Lenth=RcvBufU1[i+2];
				RDwinMessage.Order=RcvBufU1[i+3];
				RDwinMessage.Addr=RcvBufU1[i+4];
				RDwinMessage.Addr <<=8;
				RDwinMessage.Addr +=RcvBufU1[i+5];
				RDwinMessage.Datalen=RcvBufU1[i+6];
				memcpy(&RDwinMessage.Data[0],&RcvBufU1[i+7],RDwinMessage.Datalen*2);			       				

				switch (RDwinMessage.Addr)                                                  //ASK码输入数据地址解析
				{
					
					case DW_ADDR_REQUEST_VOLTAGE_INPUT_A:							                          //需求电压
					     BMSMessage.RequestVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);       
               if	((BMSMessage.RequestVoltage>750)||(BMSMessage.RequestVoltage<350))	
								  BMSMessage.RequestVoltage=450;
						break;
					
					case DW_ADDR_REQUEST_CURRENT_INPUT_A:
						   BMSMessage.RequestCurrent=ASCII_To_HEX(RDwinMessage.Data,3,0);       //需求电流
					     if(BMSMessage.RequestCurrent>=250)
							  	BMSMessage.RequestCurrent=250;
						break;	
					
					case DW_ADDR_SOC_INPUT:                                                   //车端SOC
						  BMSMessage.SOC=ASCII_To_HEX(RDwinMessage.Data,2,0);
						break;
					
					case DW_ADDR_BATTERY_VOLTAGE_INPUT:                                       //电池电压						
						  BMSMessage.BatteryVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);
					     if	((BMSMessage.BatteryVoltage>750)||(BMSMessage.BatteryVoltage<200))	
								  BMSMessage.BatteryVoltage=325;
						break;
							 
					case DW_ADDR_BATTERY_TYPE_INPUT:                                          //电池类型
						
						  BMSMessage.BatteryType=ASCII_To_HEX(RDwinMessage.Data,1,0);
						break;
					
					case DW_ADDR_CHARGE_MODE_INPUT:                                           //充电模式						 
						  BMSMessage.ChargeMode=ASCII_To_HEX(RDwinMessage.Data,1,0);
						break;	
					
					case DW_ADDR_SINGLE_VOLTAGE_INPUT:                                            //单体电压						
						  BMSMessage.SingleVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);
					     if	((BMSMessage.SingleVoltage>500)||(BMSMessage.SingleVoltage<100))	
								  BMSMessage.SingleVoltage=360;						
						break;
					
					case DW_ADDR_MAXSingleVoltage_INPUT:                                          //单体电压						
						  BMSMessage.MAXSingleVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);	
					     if	((BMSMessage.MAXSingleVoltage>500)||(BMSMessage.MAXSingleVoltage<100))	
								  BMSMessage.MAXSingleVoltage=420;					
						break;					
					case DW_ADDR_MINSingleVoltage_INPUT:                                          //单体电压						
						  BMSMessage.MINSingleVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);
					     if	((BMSMessage.MINSingleVoltage>500)||(BMSMessage.MINSingleVoltage<100))	
								  BMSMessage.MINSingleVoltage=240;					
						break;					
				
					case DW_ADDR_SINGLE_TEMPERATURE_INPUT:                                        //单体电压温度						 
						  BMSMessage.SingleVoltageTemp=ASCII_To_HEX(RDwinMessage.Data,2,0); 
					     if	((BMSMessage.SingleVoltageTemp>99)||(BMSMessage.SingleVoltageTemp<1))	
								  BMSMessage.SingleVoltageTemp=25;						
						break;

					case DW_ADDR_MAXSingleVoltageTemp_INPUT:                                      //单体电压温度						 
						  BMSMessage.MAXSingleVoltageTemp=ASCII_To_HEX(RDwinMessage.Data,2,0);     
					     if	((BMSMessage.MAXSingleVoltageTemp>99)||(BMSMessage.MAXSingleVoltageTemp<1))	
								  BMSMessage.MAXSingleVoltageTemp=70;						
						break;			
					
					case DW_ADDR_MINSingleVoltageTemp_INPUT:                                      //单体电压温度						 
						  BMSMessage.MINSingleVoltageTemp=ASCII_To_HEX(RDwinMessage.Data,2,0); 
					     if	((BMSMessage.MINSingleVoltageTemp>99)||(BMSMessage.MINSingleVoltageTemp<1))	
								  BMSMessage.MINSingleVoltageTemp=1;						
						break;

					case DW_ADDR_BMSSoftVer_INPUT:                                                //软件版本						 
						  BMSMessage.SoftVersion=ASCII_To_HEX(RDwinMessage.Data,2,0);  
						if	((BMSMessage.SoftVersion>30)||(BMSMessage.SoftVersion<10))	
							 BMSMessage.SoftVersion=11;					
						break;
					
					case DW_ADDR_RESET_INPUT:                                                 //复位
						  BMSMessage.ChargeStage=0;
					    BMSMessage.BMSStopFlag=0;
					    Battery_SWITCH_OFF();
					    DC_SWITCH_OFF();
					    FAN_SWITCH_OFF();
						break;	
					
					case DW_ADDR_STOP_INPUT:                                                  //停止
						  BMSMessage.BMSStopFlag=1;					  
					  break;
					
					case DW_ADDR_MAX_VOLTAGE_INPUT:                                           
						  BMSMessage.MAXVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);            // 最高允许电压
						if	((BMSMessage.MAXVoltage>750)||(BMSMessage.MAXVoltage<200))	
							 BMSMessage.MAXVoltage=500;
					  break;
					
					case DW_ADDR_MAX_CURRENT_INPUT:                                           
						  BMSMessage.MAXCurrent=ASCII_To_HEX(RDwinMessage.Data,3,0);            // 最高允许电流		
						if	((BMSMessage.MAXCurrent>250)||(BMSMessage.MAXCurrent<10))	
							 BMSMessage.MAXCurrent=250;					
            break;
					
					case DW_ADDR_RatedBatteryVolume_INPUT:                                           
						  BMSMessage.RatedBatteryVolume=ASCII_To_HEX(RDwinMessage.Data,3,0);            // 额定电池容量
					  break;
					
					case DW_ADDR_RatedBatteryVoltage_INPUT:                                           
						  BMSMessage.RatedBatteryVoltage=ASCII_To_HEX(RDwinMessage.Data,3,0);            // 额定电池电压	
						if	((BMSMessage.RatedBatteryVoltage>750)||(BMSMessage.RatedBatteryVoltage<200))	
							 BMSMessage.RatedBatteryVoltage=500;					
					  break;
					
					case DW_ADDR_ChargeLimitedTime_INPUT:                                           
						  Sys_PARA.ChargeLimitedTime=ASCII_To_HEX(RDwinMessage.Data,3,0);          //充电限制时间
					  if ((Sys_PARA.ChargeLimitedTime>600)||(Sys_PARA.ChargeLimitedTime<1))
					     Sys_PARA.ChargeLimitedTime=5;
					  break;					

					case DW_ADDR_TimeChargeModeStart_INPUT:                                           
						  Sys_PARA.TimeChargeMode=1;                                               //充电限制时间启用
					  break;

					case DW_ADDR_TimeChargeModeStop_INPUT:                                           
						  Sys_PARA.TimeChargeMode=0;                                               //充电限制时间不启用
					  break;					
					
					case DW_ADDR_SOCIncreaseTime_INPUT:                                           
						  Sys_PARA.SOCIncreaseTime=ASCII_To_HEX(RDwinMessage.Data,2,0);            //SOC增加时间速度					
					  break;
					
					case DW_ADDR_SOCChargeModeStart_INPUT:                                           
						  Sys_PARA.SOCChargeMode=1;                                               // SOC充电模式启用
					  break;

					case DW_ADDR_SOCChargeModeStop_INPUT:                                           
						  Sys_PARA.SOCChargeMode=0;                                               // SOC充电模式不启用
					  break;		
					
					case DW_ADDR_MeterNO_INPUT:                                                //电表编号									
						   AscToHexStr(RDwinMessage.Data,Sys_PARA.MeterNO,12);			
						break;							
					
					case DW_ADDR_VIN_INPUT:                                                    //车辆识别码			
              AscToHexStr(RDwinMessage.Data,BMSMessage.VIN,34);										
						break;										
				
          default:
						break;													
				}
				UART1RxbufClear();	
			//	BMSMessage.CS = CheckSum((uint8_t *)&BMSMessage,(uint8_t *)&BMSMessage+sizeof(BMSMessage)-1);
				FramWrite((uint8_t *)&BMSMessage,BMS_MSG_PARA_ADDR,sizeof(BMSMessage));       //存储参数到铁电
				FramWrite((uint8_t *)&Sys_PARA,SYS_PARA_ADDR,sizeof(Sys_PARA));               //存储参数到铁电								
			}
   }  
			
}
/*********************************************************************************************************
**	函数：显示不同页面
*********************************************************************************************************/
static void DisplayPage(void)
{

	   switch (DisplayMessage.PageFlag)
		 {
				case DW_ADDR_PAGE0:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE0);
				break;
				case DW_ADDR_PAGE1:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE1);
				break;
				case DW_ADDR_PAGE2:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE2);
				break;
				case DW_ADDR_PAGE3:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE3);
				 break;
				case DW_ADDR_PAGE4:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE4);
				break;
				case DW_ADDR_PAGE5:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE5);
				break;
				case DW_ADDR_PAGE6:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE6);
				break;
				case DW_ADDR_PAGE7:
					Dwin_ChangePage_Frame(DW_ADDR_PAGE7);
				break;				
				default:
					break;
			}
			
}

/*********************************************************************************************************
**	函数：显示变量值
*********************************************************************************************************/
static void DisplayVariableValue(void)
{
		uint8_t ascbuf[34];
	// 第2页面信息显示
	 Dwin_ChangeVariableValue_Frame1(DW_ADDR_CHARGE_VOLTAGE,ChargerMsg.ChargeVoltage);
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_CHARGE_CURRENT,ChargerMsg.ChargeCurrent/100);
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_REQUEST_VOLTAGE,BMSMessage.RequestVoltage*10);
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_REQUEST_CURRENT,BMSMessage.RequestCurrent*10);
	 Dwin_ChangeVariableValue_Frame1(DW_ADDR_BATTERY_SOC,BMSMessage.SOC);

	// 第3页面信息显示		
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_BATTERY_VOLTAGE,BMSMessage.BatteryVoltage*10);		
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_SINGLE_VOLTAGE,BMSMessage.SingleVoltage);	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_SINGLE_TEMPERATURE,BMSMessage.SingleVoltageTemp);
	
	// 第4页面信息显示		
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_MAX_VOLTAGE_DISPLAY,BMSMessage.MAXVoltage*10);	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_MAX_CURRENT_DISPLAY,BMSMessage.MAXCurrent*10);	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_MAXSingleVoltage_DISPLAY,BMSMessage.MAXSingleVoltage);	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_MINSingleVoltage_DISPLAY,BMSMessage.MINSingleVoltage);	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_MAXSingleVoltageTemp_DISPLAY,BMSMessage.MAXSingleVoltageTemp);	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_MINSingleVoltageTemp_DISPLAY,BMSMessage.MINSingleVoltageTemp);	
	 Dwin_ChangeVariableValue_Frame1(DW_ADDR_BMSSoftVer_DISPLAY,BMSMessage.SoftVersion);	
	 
	// 第5页面信息显示	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_RatedBatteryVolume_DISPLAYT,BMSMessage.RatedBatteryVolume*10);	
	 Dwin_ChangeVariableValue_Frame1(DW_ADDR_RatedBatteryVoltage_DISPLAY,BMSMessage.RatedBatteryVoltage*10);	
	 HexToAscStr(Sys_PARA.MeterNO,ascbuf,6);	 
	 Dwin_ChangeVariableValue_FrameN(DW_ADDR_MeterNO_DISPLAY,ascbuf,12);
	 
	// 第6页面信息显示	
   Dwin_ChangeVariableValue_Frame1(DW_ADDR_ChargeLimitedTime_DISPLAY,Sys_PARA.ChargeLimitedTime);	
	 Dwin_ChangeVariableValue_Frame1(DW_ADDR_SOCIncreaseTime_DISPLAY,Sys_PARA.SOCIncreaseTime);	
   HexToAscStr(BMSMessage.VIN,ascbuf,9);	 
	 Dwin_ChangeVariableValue_FrameN(DW_ADDR_VIN_DISPLAY,ascbuf,17);		
	
	// 显示文本文档
	if(!DI2_INT_Status())                                                                  //辅源状态 
	{
  	DisplayText(DW_ADDR_Auxiliary_POWER,Power_ERR,sizeof(Power_ERR)-1);
	}
	else
	{
		DisplayText(DW_ADDR_Auxiliary_POWER,0,4);                                            //清除显示
	  DisplayText(DW_ADDR_Auxiliary_POWER,Power_RHT,sizeof(Power_RHT)-1);
	}
	switch(BMSMessage.ChargeMode)                                                          //充电模式
	{
		case 1:
			 DisplayText(DW_ADDR_CHARGE_MODE,pChargeMode1,sizeof(pChargeMode1)-1);
		  break;
		case 2:
		 	 DisplayText(DW_ADDR_CHARGE_MODE,pChargeMode2,sizeof(pChargeMode2)-1);
		  break;	
    default:
      break;			
	}	
		switch(BMSMessage.BatteryType)                                                       //电池类型
	{
		case 1:
			 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType1,sizeof(pBatteryType1)-1);
		  break;
		case 2:
		 	 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType2,sizeof(pBatteryType2)-1);
		  break;	
		case 3:
			 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType3,sizeof(pBatteryType3)-1);
		  break;
		case 4:
		 	 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType4,sizeof(pBatteryType4)-1);
		  break;		
		case 5:
			 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType5,sizeof(pBatteryType5)-1);
		  break;
		case 6:
		 	 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType6,sizeof(pBatteryType6)-1);
		  break;		
		case 7:
			 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType7,sizeof(pBatteryType7)-1);
		  break;
		case 8:
		 	 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType8,sizeof(pBatteryType8)-1);
		  break;		
		case 0xff:
			 DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType9,sizeof(pBatteryType9)-1);
		  break;		
    default:
			DisplayText(DW_ADDR_BATTERY_TYPE,pBatteryType9,sizeof(pBatteryType9)-1);
      break;			
	}	
		
}

/*********************************************************************************************************
**	函数：显示不同页面指令
*********************************************************************************************************/
static void Dwin_ChangePage_Frame(unsigned short addr)
{
	   uint8_t pdata[7];
	   uint8_t *pdat;
	   pdat= pdata;
		*pdat++ = 0x5a;
		*pdat++ = 0xa5;
		*pdat++ = 0x04;
		*pdat++ = 0x80;
		*pdat++ = 0x03;
		*pdat++ = addr>>8;
		*pdat++ = addr%256;	
	  UART1SendStr(pdata,sizeof(pdata));
	
}

/*********************************************************************************************************
**	函数：生成写迪文数据变量(2字节)指令
*********************************************************************************************************/
static void Dwin_ChangeVariableValue_Frame1(unsigned short addr,uint16_t dat)
{
	  uint8_t pdata[8]; 
	  uint8_t *pdat;	
     pdat	= pdata;
		*pdat++ = 0x5a;
		*pdat++ = 0xa5;
		*pdat++ = 0x05;
		*pdat++ = 0x82;
		*pdat++ = addr>>8;
		*pdat++ = addr%256;
		*pdat++ = dat>>8;
		*pdat++ = dat%256;
    UART1SendStr(pdata,sizeof(pdata));
	  OSTimeDlyHMSM(0,0,0,5);
}

/*********************************************************************************************************
**	函数：生成写迪文数据变量(4字节)指令
*********************************************************************************************************/
static void Dwin_ChangeVariableValue_Frame2(unsigned short addr,uint32_t dat)
{
	   uint8_t pdata[10];
	   uint8_t *pdat;
	   pdat = pdata;
		*pdat++ = 0x5a;
		*pdat++ = 0xa5;
		*pdat++ = 0x07;
		*pdat++ = 0x82;
		*pdat++ = addr>>8;
		*pdat++ = addr%256;	
		*pdat++ = (dat>>24)&0xff;
		*pdat++ = (dat>>16)&0xff;
		*pdat++ = (dat>>8)&0xff;
		*pdat++ = dat%256;	
    UART1SendStr(pdata,sizeof(pdata));
	  OSTimeDlyHMSM(0,0,0,5);
}

/*********************************************************************************************************
**	函数：生成写迪文数据变量(n字节)指令
*********************************************************************************************************/
static void Dwin_ChangeVariableValue_FrameN(unsigned short addr,uint8_t* pdata,uint8_t dlen)
{
	   uint8_t data[dlen+6];
	   uint8_t *pdat;
	   pdat = data;
		*pdat++ = 0x5a;
		*pdat++ = 0xa5;
		*pdat++ = dlen+3;
		*pdat++ = 0x82;
		*pdat++ = addr>>8;
		*pdat++ = addr%256;		  
     memcpy(pdat++,pdata,dlen);  
    UART1SendStr(data,sizeof(data));
	  OSTimeDlyHMSM(0,0,0,5);
}

/*********************************************************************************************************
**函数:显示文本文档变量指令
**********************************************************************************************************/
static void DisplayText(uint16_t addr,uint8_t *text,uint8_t len)
{
		uint8_t dspbuf[len+6];
		dspbuf[0] = 0x5a;
		dspbuf[1] = 0xa5;
		dspbuf[2] = len+3;
		dspbuf[3] = 0x82;
		dspbuf[4]=addr>>8;
		dspbuf[5]=addr%256;
		memcpy(dspbuf+6,text,len);
		UART1SendStr(dspbuf,len+6);
  	OSTimeDlyHMSM(0,0,0,5);
}


