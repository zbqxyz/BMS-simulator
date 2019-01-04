/****************************************Copyright (c)****************************************************
**                                    Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               welink.c
** Last modified Date:      2018.06.20
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include "welink.h"

uint8_t WelinkMode;
static uint8_t NoReport;

//uint8_t         DataNo; 
uint8_t         TcpConFlag;			    /*  链路联通标志          		*/
uint8_t         MsaConFlag;			    /*  主站在线标志          		*/
uint8_t        iHeartBeatAckTimOut; //心跳包应答超时计时

unsigned int	iTcpSendCnt;          //桩端发送指令计数
unsigned int	iTcpRecvCnt;          //接收到服务器正确指令计数
unsigned int	iTcpRvChekSumErrCnt;  //接收到服务器错误指令计数

Report        TxReport;
Report        RxReport;
WELINK_PARA   Welink_PARA;

/*********************************************************************************************************
** Function name:       UpstreamPackage
** Descriptions:        上传数据封包
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t UpstreamPackage(Report *pTx,uint8_t *pSnd)
{
	
	*pSnd++ = pTx->head[0]; 
	*pSnd++ = pTx->head[1];
	*pSnd++ = pTx->len%256;
	*pSnd++ = pTx->len/256;	
	*pSnd++ = pTx->encrypt;
	*pSnd++ = pTx->sn;
	*pSnd++ = pTx->cmd%256;
	*pSnd++ = pTx->cmd/256;
	memcpy(pSnd,(uint8_t *)&pTx->data,pTx->len-9);
	*pSnd++ = pTx->cs;
	return (pTx->len);
}
/*********************************************************************************************************
** Function name:       DownstreamCheck
** Descriptions:        下行数据分解   
** input parameters:       
** output parameters:   无
** Returned value:      
*********************************************************************************************************/   
uint16_t DownstreamCheck(Report *pRx,uint8_t *pRcv)
{
	pRx->head[0] = *pRcv++;
	pRx->head[1] = *pRcv++;
	pRx->len     = (*pRcv++)+(*pRcv++)*256;
	pRx->encrypt = *pRcv++;
	pRx->sn      = *pRcv++;
	pRx->cmd     = (*pRcv++)+(*pRcv++)*256;
	memcpy(&pRx->data,pRcv,pRx->len-3);
	pRcv+=(pRx->len-3);
	pRx->cs = *pRcv++;

	return (pRx->len+4);
}
/*********************************************************************************************************
** Function name:       HardwareSendData
** Descriptions:        通过硬件端口发送数据
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void HardwareSendData(uint8_t *sbuf, uint8_t slen, uint8_t port)
{   
 
    if (TcpConFlag == 0) 			
        return ;
    
		if(iTcpSendCnt < 0xffff) 
		{
			 iTcpSendCnt++;//网络端口发送数据计数
		}

    switch(port)
    {
        case WelinkMode_Ethernet:
            OSTimeDly(2);
            WriteSocketDataToBuffer(0,sbuf,slen);
//		       if(PRINT_NETBUFF)
//						 Print("%s", sbuf);
            OSTimeDly(2);
		    
            break;
                   
        default:
            break;  
    }
}
/*********************************************************************************************************
** Function name:       UpstreamCmdHandle
** Descriptions:        主动上报 
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void UpstreamCmdHandle(uint16_t cmd,uint8_t port)
{
	uint16_t i,dlen/*,ulen*/;
	uint8_t  UpDataBuff[300];
	
	TxReport.head[0]= 0xaa;				      //起始域
	TxReport.head[1]= 0xf5;				      //起始域
	TxReport.len  	= 0x00;				      //长度域
	TxReport.encrypt= 0x10;				      //加密信息域  
	TxReport.sn   	= NoReport++;		    //序列号域	
	TxReport.cmd    = cmd;				      //命令代码
	
	switch(cmd)
	{

		case 2:            
          dlen = cmd_02_upload(TxReport.data,port);
	        break;  
	  case 4:            
          dlen = cmd_04_upload(TxReport.data,port);
	        break; 
		case 6:            
          dlen = cmd_06_upload(TxReport.data,port);
	        break;  
	  case 8:            
          dlen = cmd_08_upload(TxReport.data,port);
	        break;
	  case 10:            
          dlen = cmd_10_upload(TxReport.data,port);
	        break;		
	  case 102:               //发送心跳  			
          dlen = cmd_102_upload(TxReport.data,port);
	        break;  
	  case 104:         			//状态上报
          dlen = cmd_104_upload(TxReport.data,port);
	        break;		
	  case 106:               //发送签到 			
          dlen = cmd_106_upload(TxReport.data,port);
	        break;  
	  case 108:            
          dlen = cmd_108_upload(TxReport.data,port);
	        break;
	  case 110:            
          dlen = cmd_110_upload(TxReport.data,port);
	  case 202:            
          dlen = cmd_202_upload(TxReport.data,port);
	        break;
	  case 204:            
          dlen = cmd_204_upload(TxReport.data,port);		
	  case 206:            
          dlen = cmd_206_upload(TxReport.data,port);				

		default:
			dlen = 0;
		break;
	}
	
	if(dlen==0)
		return;

	TxReport.len = dlen + 9;					                  //计算长度域长度值
	TxReport.cs =  TxReport.sn+TxReport.cmd%256 + TxReport.cmd/256;
	for (i=0; i<dlen; i++)
	{
		TxReport.cs = TxReport.cs + TxReport.data[i];	    //计算校验和域校验值	
  }
	  TxReport.cs &=0xff;	
		
//	memset(UpDataBuff,0,sizeof(UpDataBuff));            //重新封装，避免将结构体内不需要的多余数据发出  如TxReport.data中的256个数，数据过长而无法发送校验位
//	ulen = UpstreamPackage(&TxReport, UpDataBuff);	    //打包发送数据返回帧长度	
// 	HardwareSendData(UpDataBuff, ulen, port);	          //硬件发送数据	
//	下面的实现方式和上面注释的实现方式有何不同
   memcpy(UpDataBuff,(uint8_t*)&TxReport,TxReport.len-1);
	 UpDataBuff[TxReport.len-1]=TxReport.cs;
   HardwareSendData(UpDataBuff, TxReport.len, port);	  //硬件发送数据		
}	

/*********************************************************************************************************
** Function name:       UpstreamCmdHandle
** Descriptions:        指令解析
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint8_t DownstreamCmdHandle(uint8_t *pRcv,uint16_t len,uint8_t port)
{
	uint16_t dlen;
	uint8_t  i,sn,cmd,cs;
	
	if ((*pRcv!=0xaa)||(*(pRcv+1)!=0xf5))         //判断帧头
		return 0; 												  	     
	
	dlen = *(pRcv+2) + (*(pRcv+3))*256 ;					//长度域		
		
	if(*(pRcv+4)!=0x02)										  	    //信息域  
		return 0;
	
	  sn = *(pRcv+5);                             //序列号域	
   	cmd=*(pRcv+6)+ (*(pRcv+7))*256 ;						//命令字	   
#if 0	
	for(i=0;i<dlen;i++)                            //计算校验和
		cs+=*(pRcv+i);

	if(cs!=*(pRcv+dlen-1))								    		    //判断校验和
	{
		 if(iTcpRvChekSumErrCnt < 0xffff)
		 iTcpRvChekSumErrCnt++;                     //接收到服务器错误指令
	   return 0; 
	}														
	DownstreamCheck(&RxReport,p);                 //解包拷贝
#endif

	switch(cmd)
	{		
		case 1:
			cmd_1_proess(pRcv+8,dlen-9,port);
			break;
		
		case 3:
			cmd_3_proess(pRcv+8,dlen-9,port);
			break;
		
		case 5:
			cmd_5_proess(pRcv+8,dlen-9,port);
			break;
		
		case 7:
			cmd_7_proess(pRcv+8,dlen-9,port);
			break;
		
		case 9:
			cmd_9_proess(pRcv+8,dlen-9,port);
			break;

		case 101:
			cmd_101_proess(pRcv+8,dlen-9,port);
			break;

		case 103:
			cmd_103_proess(pRcv+8,dlen-9,port);
			break;		
		
		case 105:
			cmd_105_proess(pRcv+8,dlen-9,port);
			break;
		
		case 107:
			cmd_107_proess(pRcv+8,dlen-9,port);
			break;
		
	  case 109:
			cmd_109_proess(pRcv+8,dlen-9,port);
			break;
		
		case 201:
			cmd_201_proess(pRcv+8,dlen-9,port);
			break;
		
		case 203:
			cmd_203_proess(pRcv+8,dlen-9,port);
			break;	
		
		case 205:
			cmd_205_proess(pRcv+8,dlen-9,port);
			break;		
		
		default:
	 			break;
}
	if( cmd != 0)
	{
		 if(iTcpRecvCnt < 0xffff)
		 iTcpRecvCnt++;//接收到服务器正确指令计数
	}
	return cmd;
}
/*********************************************************************************************************
** Function name:       ENETconnect
** Descriptions:        重新建立以太网TCP连接
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void ReConnectMSA(void)		//ANDY 2016.1.6
{
   TcpConFlag=0;
	 MsaConFlag=0;
}
/*********************************************************************************************************
** Function name:       ENETconnect
** Descriptions:        建立以太网TCP连接
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void ENETconnect( void )	
{   
	if(WelinkMode == WelinkMode_Ethernet)
	{
		if(TcpConFlag ==0){
				ReConnectMSA();
				W5500_Initialization();										//初始化W5500
 				W5500SocketRXD(0);											  //建立SOCKET,TCP连接		
		}
	}
		if(WelinkMode == WelinkMode_4G)
	{
 
	}	
}
/*********************************************************************************************************
** Function name:       MsaLogin
** Descriptions:        登陆主站
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void MsaLogin( void )		
{
	uint8_t i=0,j=0;
	
 if(Ethernet_Reply_Time/120 >=1)                                 //心跳包应答未超时
	 {		
		 Ethernet_Reply_Time=0; 		 
		 ReConnectMSA();	                                        	 //超时计时器复位 
	    return;
	 }
	if(MsaConFlag==1||TcpConFlag==0)										  					//TCP未连接或者已登录主站
		return;

	 if(MsaConFlag!=1)
	 {    			
	     for(i=0;i<10;i++)
	     {															
		     UpstreamCmdHandle(106, WelinkMode_Ethernet);									          //发送签到			   
				 for (j=0; j<3; j++)
	    	{																					                              //扫描间隔
		    	OSTimeDly(OS_TICKS_PER_SEC*2);													              //扫描间隔				
			    RecvDataHandle();
#if 0
			    if(MsaConFlag!=1)
			    {
			    	ReConnectMSA();																                    	//超时计时器复位                 
			    	return;
			    }
#endif
		     }
	      }
			}
	if(MsaConFlag==0)
	{			        
		TcpConFlag=0;
	}
}
/*********************************************************************************************************
** Function name:       RecvDataHandle
** Descriptions:        
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void RecvDataHandle( void )	
{
    static uint16_t i;   

    
	if(TcpConFlag==0)								
		return;
	
	if(WelinkMode == WelinkMode_Ethernet){
		  W5500SocketRXD(0);
			for(i=0; i<ENETRcvLen; i++)
    	{
		   DownstreamCmdHandle((uint8_t *)ENETRcvBuf+i, ENETRcvLen-i, Welink_PARA.SocketType);		//
		  	break;
	     }	
	}

	if(WelinkMode== WelinkMode_4G)
		return;			
	
}

/*********************************************************************************************************
** Function name:       SendDataHandle
** Descriptions:        发送上传数据
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/

void SendDataHandle( void )
{	
	if(MsaConFlag==0||TcpConFlag==0)										//非登陆在线状态不发送数据
		return;
	if(WelinkMode == WelinkMode_Ethernet){
		
		 if(ScnTime%15==0) {	
       	UpstreamCmdHandle(104,WelinkMode_Ethernet);   //发送状态
			  OSTimeDlyHMSM(0,0,0,200);                     // 防止同一秒多次发送心跳			 
        UpstreamCmdHandle(102,WelinkMode_Ethernet);   //发送心跳
			  OSTimeDlyHMSM(0,0,0,800);                     // 防止同一秒多次发送心跳
		 }
		 
	}				
}

/*********************************************************************************************************
** Function name:       WelinkMain
** Descriptions:        
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void WelinkMain(void )
{
	WelinkMode = WelinkMode_Ethernet;
	TcpConFlag= 0;	
	while(1)
  {
        OSTimeDlyHMSM(0,0,0,500);           // 扫描间隔

        ENETconnect();                      // 建立TCP

        MsaLogin();                         // 签到登陆主站

        RecvDataHandle();                   // 下行数据处理

        SendDataHandle();			              // 上行数据处理
   }

}





	
	
	
	
	
	


