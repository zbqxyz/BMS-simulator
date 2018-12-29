/****************************************Copyright (c)***************************************************************
**                                     ueee  Co.,LTD.
**                                     
**                                   http://www.ueee.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               ethernet.c
** Last modified Date:      2017.09.20
** Last Version:            1.0
** Description:             
********************************************************************************************************************/
#include <ethernet.h>

/***************----- 网络参数变量定义 -----***************/

 unsigned char  Gateway_IP[4];	     	//网关IP地址 
 unsigned char  Sub_Mask[4];			  	//子网掩码 
 unsigned char  Phy_Addr[6];  		  	//物理地址(MAC) 
 unsigned char  IP_Addr[4];				    //本机IP地址 

 unsigned int   S0_Port;   			  //端口0的端口号
 unsigned char  S0_DIP[4];				//端口0目的IP地址 
 unsigned int   S0_DPort;			  	//端口0目的端口号

 unsigned char  S1_Port[2];   			/* Socket1 Port number */
 unsigned char  S1_DIP[4];   			  /* Socket1 Destination IP Address */
 unsigned char  S1_DPort[2];				/* Socket1 Destination Port number */

 unsigned char  S2_Port[2];				  /* Socket2 Port number */
 unsigned char  S2_DIP[4];				  /* Socket2 Destination IP Address */
 unsigned char  S2_DPort[2];				/* Socket2 Destination Port number */

 unsigned char  S3_Port[2];				  /* Socket3 Port number */
 unsigned char  S3_DIP[4];			  	/* Socket3 Destination IP Address */
 unsigned char  S3_DPort[2];				/* Socket3 Destination Port number */

 unsigned char  S0_State;				/* Socket0 state recorder */
 unsigned char  S1_State;				/* Socket1 state recorder */
 unsigned char  S2_State;				/* Socket2 state recorder */
 unsigned char  S3_State;				/* Socket3 state recorder */
 
 unsigned char  ENETRcvBuf[512];			/* 端口接收数据缓冲区 */
 uint16_t ENETRcvLen;                 /* 定义端口接收数据长度*/
 unsigned char  S0_Mode;	            //端口0的运行模式,0:TCP服务器模式,1:TCP客户端模式,2:UDP(广播)模式
/*******************************************************************************************************************
** Function name:		SysLinkParaInit
** Descriptions:		W5500读写延时程序
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void SysLinkParaInit(void)
{	
	      int i;
	
	      Welink_PARA.TermNo[0] = 0x31;      //桩编号
	      Welink_PARA.TermNo[1] = 0x31;
	      Welink_PARA.TermNo[2] = 0x38;
	      Welink_PARA.TermNo[3] = 0x30;
	      Welink_PARA.TermNo[4] = 0x35;
	      Welink_PARA.TermNo[5] = 0x31;
	      Welink_PARA.TermNo[6] = 0x3f;
	      Welink_PARA.TermNo[7] = 0x30;
	      Welink_PARA.TermNo[8] = 0x30;
	      Welink_PARA.TermNo[9] = 0x35;
	
	      for(i=10;i<32;i++)
	    	{
	         Welink_PARA.TermNo[i] = 0x00;
				}
		
				Welink_PARA.SoftVer = 10000;
							
        Welink_PARA.GateWay[0] = 1;				 /* 充电桩网关 */	
        Welink_PARA.GateWay[1] = 20;			
        Welink_PARA.GateWay[2] = 168;			
        Welink_PARA.GateWay[3] = 192;			

        Welink_PARA.IP[0] = 88;						  /* 充电桩IP*/	
        Welink_PARA.IP[1] = 20;			
        Welink_PARA.IP[2] = 168;			
        Welink_PARA.IP[3] = 192;		
        Welink_PARA.Port  = 9000 ;					/* 充电桩端口*/		

        Welink_PARA.MASK[0] = 0 ;				  	/* 子网掩码 */
        Welink_PARA.MASK[1] = 255 ;			
        Welink_PARA.MASK[2] = 255 ;			
        Welink_PARA.MASK[3] = 255 ;			

        Welink_PARA.MAC[0] = 0x00 ;					/* 充电桩MAC地址 */	
        Welink_PARA.MAC[1] = 0x10 ;			
        Welink_PARA.MAC[2] = 0x10 ;			
        Welink_PARA.MAC[3] = 0x10 ;			
        Welink_PARA.MAC[4] = 0x10 ;			
        Welink_PARA.MAC[5] = 0x10 ;
				
				Welink_PARA.ServerIP[0]=254;   	    	   /*服务器IP*/ 
				Welink_PARA.ServerIP[1]=20;   	    	   /*服务器IP*/ 
				Welink_PARA.ServerIP[2]=168;   	    	   /*服务器IP*/ 
			  Welink_PARA.ServerIP[3]=192;   	    	   /*服务器IP*/ 
        Welink_PARA.ServerPort=1800 ;					   /*服务器端口*/		
				
	//		Welink_PARA.CS = CheckSum((uint8_t *)&Welink_PARA,(uint8_t *)&Welink_PARA+sizeof(Welink_PARA)-1);	
			FramWrite((uint8_t *)&Welink_PARA,WELINK_PARA_ADDR,sizeof(Welink_PARA));       //存储参数到铁电
}

/*******************************************************************************************************************
** Function name:		Ndelay
** Descriptions:		W5500读写延时程序
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void Ndelay(int dely)
{
    int i;
   for(i=dely;i>0;i--);
	
}
/*******************************************************************************************************************
** Function name:	  	Fdelay
** Descriptions:	  	发送1字节的数据
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void EthernetSendByte(uint8_t  data)
{
		SSP2_SendData(data);			                               						
}
/*******************************************************************************************************************
** Function name:	  	EthernetReadByte
** Descriptions:	  	收取1字节的数据
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
unsigned char  EthernetReadByte(void)
{
	  uint8_t tmp ;	
    tmp = SSP2_ReadData();
	  return tmp;
}
/*******************************************************************************************************************
** Function name:	  	Read_W5500
** Descriptions:	  	从W5500指定的地址读取一个字节
** input parameters:	地址
** output parameters:	无
********************************************************************************************************************/
unsigned char Read_W5500(unsigned short addr)
{
	unsigned char  tmp ;

	/* Set W5500 SCS Low */
	NET_CS_L() ;
	Ndelay(5);
	
	/* Write Address */													
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte( addr%256 );

	/* Write Control Byte */
	EthernetSendByte(FDM1|RWB_READ|COMMON_R);

	/* Read 1 byte */
	tmp = EthernetReadByte();	

	/* Set W5500 SCS High*/
	NET_CS_H();										    	// 发送一个哑数据并读取数据 
	Ndelay(5);
	return(tmp);		
}
/*******************************************************************************************************************
** Function name:	  	Write_W5500
** Descriptions:	  	将一个字节写入W5500指定的地址
** input parameters:	地址，字节数据
** output parameters:	无
********************************************************************************************************************/
void Write_W5500(unsigned short addr, unsigned char  dat)
{
	/* Set W5500 SCS Low */
	NET_CS_L() ;
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte( addr%256 );

	/* Write Control Byte */
	EthernetSendByte(FDM1|RWB_WRITE|COMMON_R);

	/* Write 1 byte */
	EthernetSendByte(dat);								// 写入数据 

	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
}
/*******************************************************************************************************************
** Function name:	  	WriteSocket_1Byte
** Descriptions:	  	Write W5500 Socket Register 1 byte
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void WriteSocket_1Byte(SOCKET s, unsigned int addr, unsigned char dat)
{
	/* Set W5500 SCS Low */
	NET_CS_L(); 
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte( addr%256 );

	/* Write Control Byte */
	EthernetSendByte(FDM1|RWB_WRITE|(s*0x20+0x08));

	/* Write 1 byte */
	EthernetSendByte(dat);

	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
}
/*******************************************************************************************************************
** Function name:	  	WriteSocket_2Byte
** Descriptions:	  	Write W5500 Socket Register 2 byte
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void WriteSocket_2Byte(SOCKET s, unsigned int addr, unsigned int dat)
{
	/* Set W5500 SCS Low */
	NET_CS_L() ;
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte( addr%256 );

	/* Write Control Byte */
	EthernetSendByte(FDM2|RWB_WRITE|(s*0x20+0x08));

	/* Write 2 byte */
	EthernetSendByte(dat/256);
	EthernetSendByte(dat%256);

	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
}
/*******************************************************************************************************************
** Function name:	  	WriteSocket_4Byte
** Descriptions:	  	Write W5500 Socket Register 4 byte
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void WriteSocket_4Byte(SOCKET s, unsigned int addr, unsigned char *dat_ptr)
{
	/* Set W5500 SCS Low */
	NET_CS_L() ;
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte( addr%256 );

	/* Write Control Byte */
	EthernetSendByte(FDM4|RWB_WRITE|(s*0x20+0x08));

	/* Write 4 byte */
	EthernetSendByte(*dat_ptr++);
	EthernetSendByte(*dat_ptr++);
	EthernetSendByte(*dat_ptr++);
	EthernetSendByte(*dat_ptr++);

	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
}
/*******************************************************************************************************************
** Function name:	  	ReadSocket_1Byte
** Descriptions:	  	Read W5500 Socket register 1 Byte
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
unsigned char ReadSocket_1Byte(SOCKET s, unsigned int addr)
{
	unsigned char i=0;

	/* Set W5500 SCS Low */
	NET_CS_L() ;
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte(addr%256);

	/* Write Control Byte */
	EthernetSendByte(FDM1|RWB_READ|(s*0x20+0x08));
	i = EthernetReadByte();	
	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
	return i;
}
/*******************************************************************************************************************
** Function name:	  	ReadSocket_2Byte
** Descriptions:	  	Read W5500 Socket register 2 Bytes
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
unsigned int ReadSocket_2Byte(SOCKET s, unsigned int addr)
{
	unsigned int i,tmp;

	/* Set W5500 SCS Low */
	NET_CS_L() ;
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(addr/256);					// 发送地址 
	EthernetSendByte( addr%256 );

	/* Write Control Byte */
	EthernetSendByte(FDM2|RWB_READ|(s*0x20+0x08));

	/* Read 1 byte */
	tmp = EthernetReadByte();	
	i =tmp*256;
	
	/* Read 1 byte */
	tmp = EthernetReadByte();	
	i +=tmp;

	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
	return i;
}
/*******************************************************************************************************************
** Function name:	  	ReadSocketDataToBuffer
** Descriptions:	  	Read data from W5500 Socket data RX Buffer
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
unsigned int ReadSocketDataToBuffer(SOCKET s, unsigned char *dat_ptr)
{
	unsigned int rx_size;
	unsigned int offset, offset1;
	unsigned int i;
	
	rx_size=ReadSocket_2Byte(s,Sn_RX_RSR);
	if(rx_size==0)		/* if no data received, return */
		return 0;
	if(rx_size>UARTBUFLEN)
		rx_size=UARTBUFLEN;

	offset=ReadSocket_2Byte(s,Sn_RX_RD);
	offset1=offset;
	offset&=(S_RX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	NET_CS_L();
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(offset/256); 			
	EthernetSendByte(offset%256);

	/* Write Control Byte */
	EthernetSendByte(VDM|RWB_READ|(s*0x20+0x18));		

	if((offset+rx_size)<S_RX_SIZE){
		/* Read Data */
		for(i=0;i<rx_size;i++)
			*dat_ptr++=EthernetReadByte();
	}
	else{
		offset=S_RX_SIZE-offset;
		for(i=0;i<offset;i++)
			*dat_ptr++=EthernetReadByte();
		
		/* Set W5500 SCS High */
		NET_CS_H();
		Ndelay(5);
		
		/* Set W5500 SCS Low */
		NET_CS_L() ;
		Ndelay(5);
		/* Write Address */
		EthernetSendByte(0x00);
		EthernetSendByte(0x00);

		/* Write Control Byte */
		EthernetSendByte(VDM|RWB_READ|(s*0x20+0x18));		

		/* Read Data */
		for(;i<rx_size;i++)
			*dat_ptr++=EthernetReadByte();
	}
	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
	/* Update offset*/
	offset1+=rx_size;
	WriteSocket_2Byte(s, Sn_RX_RD, offset1);
	WriteSocket_1Byte(s, Sn_CR, RECV);					/* Write RECV Command */
	return rx_size;
}
/*******************************************************************************************************************
** Function name:	  	WriteSocketDataToBuffer
** Descriptions:	  	Write data to W5500 Socket data TX Buffer
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
void WriteSocketDataToBuffer(SOCKET s, unsigned char *dat_ptr, unsigned int size)
{
	unsigned int offset,offset1;
	unsigned int i;

	offset=ReadSocket_2Byte(s,Sn_TX_WR);
	offset1=offset;
	offset&=(S_TX_SIZE-1);		/* Calculate the real physical address */
	
	/* Set W5500 SCS Low */
	NET_CS_L();
	Ndelay(5);
	/* Write Address */
	EthernetSendByte(offset/256); 				// 发送地址 
	EthernetSendByte(offset%256);

	/* Write Control Byte */
	EthernetSendByte(VDM|RWB_WRITE|(s*0x20+0x10));		

	if((offset+size)<S_TX_SIZE){
		/* Write Data */
		for(i=0;i<size;i++)
			EthernetSendByte(*dat_ptr++);		/* Send a byte*/
	}
	else
	{
		offset=S_TX_SIZE-offset;
		for(i=0;i<offset;i++)
			EthernetSendByte(*dat_ptr++);		/* Send a byte*/
		/* Set W5500 SCS High */
		NET_CS_H();
		Ndelay(5);
		/* Set W5500 SCS Low */
		NET_CS_L();
		Ndelay(5);
		/* Write Address */
		EthernetSendByte(0x00);
		EthernetSendByte(0x00);
		
		/* Write Control Byte */
		EthernetSendByte(VDM|RWB_WRITE|(s*0x20+0x10));		
	
		/* Send Data */
		for(;i<size;i++)
			EthernetSendByte(*dat_ptr++);		/* Send a byte */
	}
	
	/* Set W5500 SCS High */
	NET_CS_H();
	Ndelay(5);
	/* Updata offset */
	offset1+=size;
	WriteSocket_2Byte(s, Sn_TX_WR, offset1);
	WriteSocket_1Byte(s, Sn_CR, SEND);					/* Write SEND Command */
	
}
/*******************************************************************************************************************
** Function name:	  	SocketConnect
** Descriptions:	  	Set Socket n in TCP Client mode
** input parameters:	无
** output parameters:	无
********************************************************************************************************************/
unsigned char SocketConnect(SOCKET s)
{
	/* Set Socket n in TCP mode */
	WriteSocket_1Byte(s,Sn_MR,MR_TCP);
	/* Open Socket n */
	WriteSocket_1Byte(s,Sn_CR,OPEN);

	OSTimeDly(10);	/* Wait for a moment */
	if(ReadSocket_1Byte(s,Sn_SR)!=SOCK_INIT)
	{
		WriteSocket_1Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	/* make connect */
	WriteSocket_1Byte(s,Sn_CR,CONNECT);
	return TRUE;
}
/*******************************************************************************************************************
** Function name:	  	W5500Init
** Descriptions:	  	对W5500初始化
** input parameters:	地址
** output parameters:	无
********************************************************************************************************************/
void W5500Init(void)
{
	unsigned char  i  ;
	unsigned char  xx[10]  ;

	memset(xx,0,sizeof(xx));

	//select chip
 	NET_CS_L();											// 片选使能

	//hardware reset
	W5500_ResetOFF() ;										// 硬件复位
	OSTimeDly(OS_TICKS_PER_SEC/5);
	
	W5500_ResetON();	
	OSTimeDly(OS_TICKS_PER_SEC/5);

	//software reset	
	Write_W5500(W5500_MODE,MODE_RST);						// 软复位W5100

	//wait for hardware link
	do{                                          
		OSTimeDly(OS_TICKS_PER_SEC/5);			    		/* wait 100ms */
		i=Read_W5500(PHYCFGR);		
	}while((i&LINK)==0);								
	
	//write gateway ip							
	for(i = 0 ; i < 4 ; i++)						        	// 设置网关(Gateway)的IP地址，4字节 
	{
		Write_W5500(W5500_GAR+i,Gateway_IP[i]);				
	}
	
	//writ subnet mask 														//  设置子网掩码(MASK)值，4字节。子网掩码用于子网运算
	for(i=0;i<4;i++)
	{
		Write_W5500(W5500_SUBR+i,Sub_Mask[i]);				
	}

	for(i=0;i<6;i++)
	{
		Write_W5500(W5500_SHAR+i,Phy_Addr[i]);				
	}

	for(i=0;i<4;i++)
	{
		Write_W5500(W5500_SIPR+i,IP_Addr[i]);				
	}

	OSTimeDly(OS_TICKS_PER_SEC);

	//read for IP_Addr
	for(i = 0 ; i < 4 ; i++)
	{
		xx[i] = Read_W5500(W5500_SIPR+i);			
	}
	//read for Sub_Mask
	for(i = 0 ; i < 4 ; i++)
	{
		xx[i] = Read_W5500(W5500_SUBR+i);			
	}
	//read for Gateway_IP
	for(i = 0 ; i < 4 ; i++)
	{
		xx[i] = Read_W5500(W5500_GAR+i);			
	}
	//read for Phy_Addr
	for(i = 0 ; i < 6 ; i++)
	{
		xx[i] = Read_W5500(W5500_SHAR+i);			
	}														
																				
}
/*******************************************************************************************************************
** Function name:	  	Socketn_Config
** Descriptions:	  	Config Scoket
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
void Socketn_Config(SOCKET s)
{
	/* Set Socket 0 Port Number as S0_Port */
	WriteSocket_2Byte(s,Sn_PORT,S0_Port+s);		

	/* Set Socket0 Destination IP as S0_DIP*/
	WriteSocket_4Byte(s,Sn_DIPR,S0_DIP);

	/* Set Socket0 Destination Port Number as S0_DPort */
	WriteSocket_2Byte(s,Sn_DPORTR,S0_DPort);
	
	/* Set Maximum Segment Size as 1460 */
	WriteSocket_2Byte(s, Sn_MSSR, 1460);

	/* Set RX Buffer Size as 2K */
	WriteSocket_1Byte(s,Sn_RXBUF_SIZE, 0x02);
	/* Set TX Buffer Size as 2K */
	WriteSocket_1Byte(s,Sn_TXBUF_SIZE, 0x02);
}
/*******************************************************************************************************************
** Function name:	  	DetectGateway
** Descriptions:	  	
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
unsigned char DetectGateway(SOCKET s)
{
	static unsigned int i=0;
	unsigned char dat[4];

	/* Set a different subnet destination IP */
	dat[0]=192+1;
	dat[1]=168+1;
	dat[2]=1+1;
	dat[3]=100+1;
	WriteSocket_4Byte(s,Sn_DIPR,dat);
	
	/* Set Socket n in TCP mode */
	WriteSocket_1Byte(s,Sn_MR,MR_TCP);
	/* Open Socket n */
	WriteSocket_1Byte(s,Sn_CR,OPEN);
		
	OSTimeDly(OS_TICKS_PER_SEC/5);							/* 延时200ms */
	i=ReadSocket_1Byte(s,Sn_SR);
	if(i!=SOCK_INIT)
	{
		WriteSocket_1Byte(s,Sn_CR,CLOSE);		      /* Close Socket 0 */
		return FALSE;
	}

	/* Set Socket n in Client mode */
	WriteSocket_1Byte(s,Sn_CR,CONNECT);							/*打开socket0的TCP连接*/

	do
	{
		OSTimeDly(OS_TICKS_PER_SEC/5);							/* 延时200ms */
		
		if(ReadSocket_1Byte(s,Sn_IR)&IR_TIMEOUT) 		/* No Gateway Detected */
		{
			WriteSocket_1Byte(s,Sn_CR,CLOSE);					/* Close Socket 0 */
			return FALSE;

		}
		else if(ReadSocket_1Byte(s,Sn_DHAR)!=0xff) 	/* Detect Gateway */	
		{
			WriteSocket_1Byte(s,Sn_CR,CLOSE);					/* Close Socket 0 */
			break;
		}
	}while(1);
	return TRUE;
}

/*******************************************************************************************************************
** Function name:	  	Read_IR
** Descriptions:	  	
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
unsigned char Read_IR(SOCKET s)
{
	unsigned char i;

	i=ReadSocket_1Byte(s,Sn_IR);

	if(i!=0)
		WriteSocket_1Byte(s,Sn_IR,i);

	return i;
}

/*******************************************************************************************************************
** Function name:	  	Read_IR
** Descriptions:	  	Process Received data
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
void LoopBack(void)
{
	if(WelinkMode == WelinkMode_Ethernet)
	{		
		memset((uint8_t *)&ENETRcvBuf[0],0,sizeof(ENETRcvBuf));
		ENETRcvLen = ReadSocketDataToBuffer(0, (unsigned char *)&ENETRcvBuf[0]);/* Read W5500 Data to Rx_Buffer */
		if(PRINT_NETBUFF)
			Print("%s", ENETRcvBuf);
	}

//	GulNumU3 = ReadSocketDataToBuffer(0, (unsigned char *)&ENETRcvBuf[0]);/* Read W5500 Data to Rx_Buffer */
//	if(GulNumU3>0){
//		if((RcvLenU3+GulNumU3)>=UARTBUFLEN || RcvBufU3[0]!=0xaa){
//				RcvLenU3=0;
//				memset((uint8_t *)&RcvBufU3[0],0,sizeof(RcvBufU3));
//				RcvNewU3=0;
//		}
//		memcpy(RcvBufU3+RcvLenU3,ENETRcvBuf,GulNumU3);
//		RcvNewU3=1;
//		RcvLenU3+=GulNumU3;
//		memset((uint8_t *)&ENETRcvBuf[0],0,sizeof(ENETRcvBuf));
//		GulNumU3=0;
//	}
}

/*******************************************************************************************************************
** Function name:	  	W5500LoadParameters
** Descriptions：网络通信参数,这些参数包括: 网关IP，子网掩码， 物理地址，
	               本机IP地址。评估板开放了W5500的4个端口，因此还要分别读取4个端口的端口号、
                 目的IP地址和目的端口号(只有在TCP客户端和UDP时有效)。
	               端口工作模式控制：0：TCP服务器模式；1：TCP客户端模式；2：UDP模式	
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
void W5500LoadParameters(void)
{
	unsigned char  i , j ;
	SysLinkParaInit();
	for(i = 0 , j = 3 ; i < 4; i ++ , j -- )		// 加载网关参数 
	{
		Gateway_IP[i] = Welink_PARA.GateWay[j] ;
	}
	
	for(i = 0 , j = 3 ; i < 4; i ++ , j -- )		// 加载子网掩码 
	{
		Sub_Mask[i] = Welink_PARA.MASK[j];
	}

	for(i = 0 , j = 5 ; i < 6; i ++ , j -- )		// 加载物理地址 
	{
		Phy_Addr[i]=Welink_PARA.MAC[j];
	}

	for(i = 0 , j = 3 ; i < 4; i ++ , j -- )		// 加载IP地址 
	{
		IP_Addr[i]=Welink_PARA.IP[j];
	}

	S0_Port =  Welink_PARA.Port;
	
	for(i = 0 , j = 3 ; i < 4; i ++ , j -- )		// 加载端口0的目的IP地址
	{
		S0_DIP[i]=Welink_PARA.ServerIP[j];
	}

	S0_DPort = Welink_PARA.ServerPort ;					// 目标端口

	S0_Mode = TCP_CLIENT ; 											// 加载端口0的工作模式 
}

/*******************************************************************************************************************
** Function name:	    W5500_Initialization
** Descriptions：     Init
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
void W5500_Initialization(void)
{
	S0_State = 0 ; 									// Socket0 状态寄存器

	W5500LoadParameters() ; 			  // 载入以太网参数

	W5500Init();										// 初始化以太网参数
 
	if(DetectGateway(0)==FALSE)			// 检查网关服务器 
		return;	

	if(DetectGateway(1)==FALSE)			// 检查网关服务器 
		return;	
}

/*******************************************************************************************************************
** Function name:	  W5500SocketRXD
** Descriptions：    Init
** input parameters:	none
** output parameters:	none
********************************************************************************************************************/
void W5500SocketRXD(SOCKET s)
{
	unsigned char i=SOCK_ESTABLISHED;
	
	do{
		if((Read_W5500(PHYCFGR)&LINK)==0)		  	  /* Detect Ethernet Link */
		{				   	
			WriteSocket_1Byte(s,Sn_CR,CLOSE);
			while((Read_W5500(PHYCFGR)&LINK)==0); 	/* Wait Ethernet Link */
		}

		i=Read_IR(s);							              	/* Process IR */
		if(i&IR_TIMEOUT){					   	
			WriteSocket_1Byte(s,Sn_CR,CLOSE);
		}		
		if(i&IR_CON)
			;
		
		if(i&IR_DISCON){			   	
			WriteSocket_1Byte(s,Sn_CR,CLOSE);
		}

		i=ReadSocket_1Byte(s,Sn_SR);
		if(i==0){
			TcpConFlag = 0 ;
			Socketn_Config(s);
			while(SocketConnect(s)==FALSE);	
		}
		else if(i==SOCK_ESTABLISHED){
			TcpConFlag = 1 ;				   								//SOCKET接通
			S0_State=1;
			LoopBack();
			break;
		}
		OSTimeDly(OS_TICKS_PER_SEC/10);
	}while(i!=SOCK_ESTABLISHED);

}

