/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               welink.c
** Last modified Date:      2018.06.20
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
/*********************************************************************************************************
** 帧结构                        ________________________
**                              |                        |
**                              | 	 说明          代码  |
**                              |    唤醒字符        FE  |
**			                      	|	  帧起始字符      68H  |
**			                       	|		地址域           A0  |
**			                      	|		                 A1  |
**			                       	|										 A2  |
**			                      	|										 A3  |
**			                       	|										 A4  |
**			                       	|										 A5  |
**			                      	|	  帧起始字符 	    68H  |
**			                      	|		控制码           C   |
**			                      	|		数据长度         L   |
**			                      	|		数据域          DATA |
**			                      	|		校验码           CS  |
**			                      	|		结束字符        16H  |
**			                       	|________________________|	
**
** 控制码   
**           D7                       D6                D5                 D4 D3 D2 D1 D0
**				传输方向               从站应答标志        后续帧标志             功能码
**				0 主站发出的命令帧     0 从站正确应答      0 无后续数据帧       00000 保留
**				1 从站发出的应答帧     1 从站异常应答      1 有后续数据帧       01000 广播校时
**					                                                              10001 读数据
**						                                                            10010 读后续数据
**                                                                        10011 读通讯地址
**                                                                        10100 写数据
**                                                                        10101 写通讯地址
**                                                                        10110 冻结命令
**                                                                        10111 更改通信速率
**                                                                        11000 更改密码
**                                                                        11001 最大需量清零
**                                                                        11010 电表清零
**                                                                        11011 事件清零
**
** 数据   所有数据项均先传送低位字节，后传送高位字节
**
** 数据及4字节数据标识编码 （ DI3 DI2 DI1 DI0  ）   偏移量均为0x33
**  
** 校验码  从第一个68H到校验码之前所有字节的和
**
*********************************************************************************************************/

#include <meter.h>

 uint16_t SndLen;
 uint8_t  SndBuf[UARTBUFLEN];

 sMeter SMeter;				//电表参数用于发送
 sMeter RMeter;				//电表参数用于接收


const uint32_t MeterDI[] = {0x02010100,0x02020100,0x00010000};
                            //A相电压   A相电流   正向有功总电量  电压块  电流块
                            // DI3 DI2 DI1 DI0  中的DI2 DI1 DI0任意一字节取值为0xff时，DI3不存在0xff现象，则代表该字节定义的数据项与其他字节组成的数据块

static uint8_t  FramePack_645_2007(uint8_t *MeterNo,     //电表号      形参，数据过来打包时需要传送过来的数据
                               uint8_t Ctlw,             //控制码
															 uint32_t IDSymbol,        //数据标识编码
															 uint8_t *DataPtr,         //指针型数据，待发送数据
															 uint8_t DataLen,          //数据长度
															 uint8_t *SendBuff);
static uint16_t  FrameCheck_645_2007( uint8_t *RcvBuff,  //接收数据缓冲区   
                                      uint8_t *MeterNo,  //电表号 
                                      uint8_t *IDSymbol, //数据标识编码
                                      uint8_t *DataPtr,  //指针型数据，接收数据
                                      uint32_t mRcvLen); //接收数据长度
/*********************************************************************************************************
** Function name:       MeterMain()
** Descriptions:        读电表主函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void MeterMain(void)
 {
	 
	 uint8_t  i;

	 for(i=0;i<3;i++)
	 {
		  uint8_t mark;	
      uint32_t MeterDISymbol;
      MeterDISymbol = 0;	 
		  memset(SMeter.MeterNO,0xaa,sizeof(SMeter.MeterNO));
		  memset(SMeter.Data,0,sizeof(SMeter.Data));		 
      SndLen = FramePack_645_2007(SMeter.MeterNO,  READCTLW,  MeterDI[i],   SMeter.Data,  4,  SndBuf);
    	UART2SendStr(SndBuf,SndLen);

		  if(PRINT_BUFF)
		  Print("%s", SndBuf);
			
		  OSTimeDlyHMSM(0,0,1,500);	
    	memset(RMeter.DI,0x00,sizeof(RMeter.DI));	
      memset(RMeter.Data,0x00,sizeof(RMeter.Data));	
    	mark = FrameCheck_645_2007(RcvBufU2,RMeter.MeterNO,RMeter.DI,RMeter.Data,UARTBUFLEN);      		 
		  MeterDISymbol = (RMeter.DI[3]<<24)|(RMeter.DI[2]<<16)|(RMeter.DI[1]<<8)| RMeter.DI[0];  //数据标识编码 由8bit数组转换成32bit数据
		  switch (MeterDISymbol)
			{
				  case 0x02010100:													      		//电压			
					         ChargerMsg.ChargeVoltage =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));		
					
				        break ;
#if 0
				  case 0x0201ff00:													      		//电压块			
					         ChargerMsg.ChargeVoltage =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));										
				        break ;
#endif
					
					case 0x02020100:				                          	//电流							
                   ChargerMsg.ChargeCurrent =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));
						    break;
#if 0
					case 0x0202ff00:				                          	//电流块							
                   ChargerMsg.ChargeCurrent =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));
						    break;
#endif				
					case 0x00010000:					                  				//正向有功总电能	
                   ChargerMsg.ChargerEnergy =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));					
                break;
					default:
								break ;
								
				}
		 	UART2RxbufClear();	  
	 }
 }

/*********************************************************************************************************
** Function name:       FramePack_645_2007()
** Descriptions:        DL645-2007规约帧打包 ,返回打包后的帧长度，数据加0x33，参照电表通讯的帧格式看代码
** input parameters:    645规约帧格式内的数据形参
** output parameters:   无
** Returned value:      返回打包后的帧长度，数据加0x33
*********************************************************************************************************/
static uint8_t  FramePack_645_2007(uint8_t *MeterNo,         //电表号      形参，数据过来打包时需要传送过来的数据
                               uint8_t Ctlw,             //控制码
															 uint32_t IDSymbol,        //数据标识编码
															 uint8_t *DataPtr,         //指针型数据，待发送数据
															 uint8_t  DataLen,         //数据长度
															 uint8_t *SendBuff)        //指针型数据，发送缓冲区
{  
		  uint8_t 	i;                                       //定义变量
		  uint8_t 	CCs;                                     //定义变量
		  uint8_t 	*pSendBuff;                              //定义帧指针
		  pSendBuff = SendBuff;                              //指针指向发送缓冲器 ，用指针的方式开始向发送缓冲器里放置数据，进行帧打包  
		  *pSendBuff++ = 0xFE;  				 					        	 //引导字符
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;	
		  *pSendBuff++ = 0x68;                               //往指针指向位置发送缓冲器里面放置起始符   
		  memcpy((uint8_t *)&pSendBuff[0],MeterNo,6);        //把电表号从第0到最后一位数据拷贝到发送缓冲区，共拷贝6位数据
		   pSendBuff+=6;                                     //使pSendBuff指针指向地址域后的另一个帧起始符
		  *pSendBuff++ = 0x68;                               //往指针指向位置发送缓冲器里面放置起始符   
		  *pSendBuff++ = Ctlw;                               //指针位置控制码
		  *pSendBuff++ = DataLen; 						               //指针位置放置数据长度		  
		  if(DataLen>=4) 
		  {
				*pSendBuff++ = (IDSymbol%256)+0x33;  			       //数据标识
				*pSendBuff++ = (IDSymbol>>8)%256+0x33;
				*pSendBuff++ = (IDSymbol>>16)%256+0x33;
				*pSendBuff++ = (IDSymbol>>24)%256+0x33;
        for(i=0;i<(DataLen-4);i++) 
		   		{
                   *pSendBuff++ = 0x33 + *DataPtr++;   	//每个数据加0x33后放到缓冲区
		  		}
		  }		  
		  CCs=0;
		  for(i=4;i<(DataLen+14);i++)                       //数据校验，每个数据累加和等于校验位
		  {
              CCs=CCs+*(SendBuff+i);
		  }		  
		  *pSendBuff++ = CCs;                                //指针位置放置校验位
		  
		  *pSendBuff++ = 0x16;                               //指针位置放置结束符

		  return (DataLen+16);                               //返回打包后的数据长+FE*4
}

/*********************************************************************************************************
** Function name:       FrameCheck_645_2007()
** Descriptions:        DL645-2007规约帧检测 ,判定解包，通过返回帧首地址偏移量，并还原数据
** input parameters:    645规约帧格式内的数据形参
** output parameters:   无
** Returned value:      返回数据长度
*********************************************************************************************************/
static uint16_t  FrameCheck_645_2007( uint8_t *RcvBuff,  //接收数据缓冲区   
                                      uint8_t *MeterNo,
                                      uint8_t *IDSymbol,
                                      uint8_t *DataPtr,  //指针型数据，接收数据
                                      uint32_t mRcvLen)  //接收数据长度
{
                                                
		uint16_t  k,mk;                                             
		uint8_t	Temp,CheckSum;
		
		for(mk=0;mk<mRcvLen;mk++)								
		{									   
			   if(RcvBuff[mk]==0x68&&RcvBuff[mk+7]==0x68)									//锁定通讯帧中的两个0x68
			   {
					 Temp = RcvBuff[9+mk]+10;			                            //寻找结束字符16H在帧中的位置		
					 if(RcvBuff[mk+Temp+1]!=0x16)                             //判断结束字符是否正确
						 continue; 			                                        //否则退出
					 CheckSum = 0;                                            //校验位数据先清空                     
					 for(k=0;k<Temp;k++)
						 CheckSum += RcvBuff[mk+k];			                        //获取校验位数据		   
					 if(CheckSum!=RcvBuff[mk+Temp])                           //判断校验位是否正确
						 continue;                                              //否则退出
					 memcpy(MeterNo,RcvBuff+mk+1,6);		                    	//保存表号
				   Temp = RcvBuff[8+mk] & 0x80;				                     	//判断应答控制命D7是否为1	
				   if(Temp == 0)
						 continue; 							                                //否则退出
				   Temp = RcvBuff[8+mk] & 0x40;					                    //判断应答控制命D6是否为1	
				   if(Temp != 0)
						 continue; 							                                //否则退出				   
				   Temp = RcvBuff[8+mk] & 0x0f;                             //取控制命令的低4位
				   if(Temp == 0x01 || Temp == 0x02)			                    //判断控制命令的后两位，
				   {
					   if(RcvBuff[mk+9] <= 4)                                 //数据长度小于等于4的剔除
							 continue;		                                       				   
					   Temp = RcvBuff[9+mk];						                    	//获取数据长度的值 		
						 if(Temp>(12+4))                                        //应答数据超过12个字节的扔掉
							 continue;									
					   for(k=0;k<4;k++)
						 {
					   	  IDSymbol[k] = RcvBuff[mk+10+k] - 0x33;              //IDSymbol获取数据标识符
					   }						 
						 for(k=4;k<Temp;k++){	
							 if(RcvBuff[10+mk+k]==0x32 || RcvBuff[10+mk+k]==0x00 || RcvBuff[10+mk+k]==0xff)
								RcvBuff[10+mk+k]=0;
							 else
								DataPtr[k-4] = RcvBuff[10+mk+k] - 0x33;		          //还原数据
						 }
				   }
				   else if(Temp == 0x04)							                  		//判断控制命令的后两位，
				   {        
				   		 if(RcvBuff[mk+9] != 0)                               //数据长度不为零
								 continue;	                   
				   		 Temp = 10;
				   		 CheckSum = 0;
					   	 for(k=0;k<Temp;k++)
							   CheckSum += RcvBuff[mk+k];                         //获取校验位数据	 
						 if(CheckSum!=RcvBuff[mk+Temp])
							  continue;	
						 if(RcvBuff[mk+Temp+1]!=0x16)
							  continue; 
				   }
				   break;
			   }											   
		}
		
		return mk;
}
