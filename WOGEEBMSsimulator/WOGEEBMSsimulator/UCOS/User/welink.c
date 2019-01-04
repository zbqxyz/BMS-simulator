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
uint8_t         TcpConFlag;			    /*  ��·��ͨ��־          		*/
uint8_t         MsaConFlag;			    /*  ��վ���߱�־          		*/
uint8_t        iHeartBeatAckTimOut; //������Ӧ��ʱ��ʱ

unsigned int	iTcpSendCnt;          //׮�˷���ָ�����
unsigned int	iTcpRecvCnt;          //���յ���������ȷָ�����
unsigned int	iTcpRvChekSumErrCnt;  //���յ�����������ָ�����

Report        TxReport;
Report        RxReport;
WELINK_PARA   Welink_PARA;

/*********************************************************************************************************
** Function name:       UpstreamPackage
** Descriptions:        �ϴ����ݷ��
** input parameters:    
** output parameters:   ��
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
** Descriptions:        �������ݷֽ�   
** input parameters:       
** output parameters:   ��
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
** Descriptions:        ͨ��Ӳ���˿ڷ�������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void HardwareSendData(uint8_t *sbuf, uint8_t slen, uint8_t port)
{   
 
    if (TcpConFlag == 0) 			
        return ;
    
		if(iTcpSendCnt < 0xffff) 
		{
			 iTcpSendCnt++;//����˿ڷ������ݼ���
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
** Descriptions:        �����ϱ� 
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void UpstreamCmdHandle(uint16_t cmd,uint8_t port)
{
	uint16_t i,dlen/*,ulen*/;
	uint8_t  UpDataBuff[300];
	
	TxReport.head[0]= 0xaa;				      //��ʼ��
	TxReport.head[1]= 0xf5;				      //��ʼ��
	TxReport.len  	= 0x00;				      //������
	TxReport.encrypt= 0x10;				      //������Ϣ��  
	TxReport.sn   	= NoReport++;		    //���к���	
	TxReport.cmd    = cmd;				      //�������
	
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
	  case 102:               //��������  			
          dlen = cmd_102_upload(TxReport.data,port);
	        break;  
	  case 104:         			//״̬�ϱ�
          dlen = cmd_104_upload(TxReport.data,port);
	        break;		
	  case 106:               //����ǩ�� 			
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

	TxReport.len = dlen + 9;					                  //���㳤���򳤶�ֵ
	TxReport.cs =  TxReport.sn+TxReport.cmd%256 + TxReport.cmd/256;
	for (i=0; i<dlen; i++)
	{
		TxReport.cs = TxReport.cs + TxReport.data[i];	    //����У�����У��ֵ	
  }
	  TxReport.cs &=0xff;	
		
//	memset(UpDataBuff,0,sizeof(UpDataBuff));            //���·�װ�����⽫�ṹ���ڲ���Ҫ�Ķ������ݷ���  ��TxReport.data�е�256���������ݹ������޷�����У��λ
//	ulen = UpstreamPackage(&TxReport, UpDataBuff);	    //����������ݷ���֡����	
// 	HardwareSendData(UpDataBuff, ulen, port);	          //Ӳ����������	
//	�����ʵ�ַ�ʽ������ע�͵�ʵ�ַ�ʽ�кβ�ͬ
   memcpy(UpDataBuff,(uint8_t*)&TxReport,TxReport.len-1);
	 UpDataBuff[TxReport.len-1]=TxReport.cs;
   HardwareSendData(UpDataBuff, TxReport.len, port);	  //Ӳ����������		
}	

/*********************************************************************************************************
** Function name:       UpstreamCmdHandle
** Descriptions:        ָ�����
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint8_t DownstreamCmdHandle(uint8_t *pRcv,uint16_t len,uint8_t port)
{
	uint16_t dlen;
	uint8_t  i,sn,cmd,cs;
	
	if ((*pRcv!=0xaa)||(*(pRcv+1)!=0xf5))         //�ж�֡ͷ
		return 0; 												  	     
	
	dlen = *(pRcv+2) + (*(pRcv+3))*256 ;					//������		
		
	if(*(pRcv+4)!=0x02)										  	    //��Ϣ��  
		return 0;
	
	  sn = *(pRcv+5);                             //���к���	
   	cmd=*(pRcv+6)+ (*(pRcv+7))*256 ;						//������	   
#if 0	
	for(i=0;i<dlen;i++)                            //����У���
		cs+=*(pRcv+i);

	if(cs!=*(pRcv+dlen-1))								    		    //�ж�У���
	{
		 if(iTcpRvChekSumErrCnt < 0xffff)
		 iTcpRvChekSumErrCnt++;                     //���յ�����������ָ��
	   return 0; 
	}														
	DownstreamCheck(&RxReport,p);                 //�������
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
		 iTcpRecvCnt++;//���յ���������ȷָ�����
	}
	return cmd;
}
/*********************************************************************************************************
** Function name:       ENETconnect
** Descriptions:        ���½�����̫��TCP����
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void ReConnectMSA(void)		//ANDY 2016.1.6
{
   TcpConFlag=0;
	 MsaConFlag=0;
}
/*********************************************************************************************************
** Function name:       ENETconnect
** Descriptions:        ������̫��TCP����
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void ENETconnect( void )	
{   
	if(WelinkMode == WelinkMode_Ethernet)
	{
		if(TcpConFlag ==0){
				ReConnectMSA();
				W5500_Initialization();										//��ʼ��W5500
 				W5500SocketRXD(0);											  //����SOCKET,TCP����		
		}
	}
		if(WelinkMode == WelinkMode_4G)
	{
 
	}	
}
/*********************************************************************************************************
** Function name:       MsaLogin
** Descriptions:        ��½��վ
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void MsaLogin( void )		
{
	uint8_t i=0,j=0;
	
 if(Ethernet_Reply_Time/120 >=1)                                 //������Ӧ��δ��ʱ
	 {		
		 Ethernet_Reply_Time=0; 		 
		 ReConnectMSA();	                                        	 //��ʱ��ʱ����λ 
	    return;
	 }
	if(MsaConFlag==1||TcpConFlag==0)										  					//TCPδ���ӻ����ѵ�¼��վ
		return;

	 if(MsaConFlag!=1)
	 {    			
	     for(i=0;i<10;i++)
	     {															
		     UpstreamCmdHandle(106, WelinkMode_Ethernet);									          //����ǩ��			   
				 for (j=0; j<3; j++)
	    	{																					                              //ɨ����
		    	OSTimeDly(OS_TICKS_PER_SEC*2);													              //ɨ����				
			    RecvDataHandle();
#if 0
			    if(MsaConFlag!=1)
			    {
			    	ReConnectMSA();																                    	//��ʱ��ʱ����λ                 
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
** output parameters:   ��
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
** Descriptions:        �����ϴ�����
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/

void SendDataHandle( void )
{	
	if(MsaConFlag==0||TcpConFlag==0)										//�ǵ�½����״̬����������
		return;
	if(WelinkMode == WelinkMode_Ethernet){
		
		 if(ScnTime%15==0) {	
       	UpstreamCmdHandle(104,WelinkMode_Ethernet);   //����״̬
			  OSTimeDlyHMSM(0,0,0,200);                     // ��ֹͬһ���η�������			 
        UpstreamCmdHandle(102,WelinkMode_Ethernet);   //��������
			  OSTimeDlyHMSM(0,0,0,800);                     // ��ֹͬһ���η�������
		 }
		 
	}				
}

/*********************************************************************************************************
** Function name:       WelinkMain
** Descriptions:        
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void WelinkMain(void )
{
	WelinkMode = WelinkMode_Ethernet;
	TcpConFlag= 0;	
	while(1)
  {
        OSTimeDlyHMSM(0,0,0,500);           // ɨ����

        ENETconnect();                      // ����TCP

        MsaLogin();                         // ǩ����½��վ

        RecvDataHandle();                   // �������ݴ���

        SendDataHandle();			              // �������ݴ���
   }

}





	
	
	
	
	
	


