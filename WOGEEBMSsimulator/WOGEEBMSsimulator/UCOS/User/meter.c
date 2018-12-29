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
** ֡�ṹ                        ________________________
**                              |                        |
**                              | 	 ˵��          ����  |
**                              |    �����ַ�        FE  |
**			                      	|	  ֡��ʼ�ַ�      68H  |
**			                       	|		��ַ��           A0  |
**			                      	|		                 A1  |
**			                       	|										 A2  |
**			                      	|										 A3  |
**			                       	|										 A4  |
**			                       	|										 A5  |
**			                      	|	  ֡��ʼ�ַ� 	    68H  |
**			                      	|		������           C   |
**			                      	|		���ݳ���         L   |
**			                      	|		������          DATA |
**			                      	|		У����           CS  |
**			                      	|		�����ַ�        16H  |
**			                       	|________________________|	
**
** ������   
**           D7                       D6                D5                 D4 D3 D2 D1 D0
**				���䷽��               ��վӦ���־        ����֡��־             ������
**				0 ��վ����������֡     0 ��վ��ȷӦ��      0 �޺�������֡       00000 ����
**				1 ��վ������Ӧ��֡     1 ��վ�쳣Ӧ��      1 �к�������֡       01000 �㲥Уʱ
**					                                                              10001 ������
**						                                                            10010 ����������
**                                                                        10011 ��ͨѶ��ַ
**                                                                        10100 д����
**                                                                        10101 дͨѶ��ַ
**                                                                        10110 ��������
**                                                                        10111 ����ͨ������
**                                                                        11000 ��������
**                                                                        11001 �����������
**                                                                        11010 �������
**                                                                        11011 �¼�����
**
** ����   ������������ȴ��͵�λ�ֽڣ����͸�λ�ֽ�
**
** ���ݼ�4�ֽ����ݱ�ʶ���� �� DI3 DI2 DI1 DI0  ��   ƫ������Ϊ0x33
**  
** У����  �ӵ�һ��68H��У����֮ǰ�����ֽڵĺ�
**
*********************************************************************************************************/

#include <meter.h>

 uint16_t SndLen;
 uint8_t  SndBuf[UARTBUFLEN];

 sMeter SMeter;				//���������ڷ���
 sMeter RMeter;				//���������ڽ���


const uint32_t MeterDI[] = {0x02010100,0x02020100,0x00010000};
                            //A���ѹ   A�����   �����й��ܵ���  ��ѹ��  ������
                            // DI3 DI2 DI1 DI0  �е�DI2 DI1 DI0����һ�ֽ�ȡֵΪ0xffʱ��DI3������0xff�����������ֽڶ�����������������ֽ���ɵ����ݿ�

static uint8_t  FramePack_645_2007(uint8_t *MeterNo,     //����      �βΣ����ݹ������ʱ��Ҫ���͹���������
                               uint8_t Ctlw,             //������
															 uint32_t IDSymbol,        //���ݱ�ʶ����
															 uint8_t *DataPtr,         //ָ�������ݣ�����������
															 uint8_t DataLen,          //���ݳ���
															 uint8_t *SendBuff);
static uint16_t  FrameCheck_645_2007( uint8_t *RcvBuff,  //�������ݻ�����   
                                      uint8_t *MeterNo,  //���� 
                                      uint8_t *IDSymbol, //���ݱ�ʶ����
                                      uint8_t *DataPtr,  //ָ�������ݣ���������
                                      uint32_t mRcvLen); //�������ݳ���
/*********************************************************************************************************
** Function name:       MeterMain()
** Descriptions:        �����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
		  MeterDISymbol = (RMeter.DI[3]<<24)|(RMeter.DI[2]<<16)|(RMeter.DI[1]<<8)| RMeter.DI[0];  //���ݱ�ʶ���� ��8bit����ת����32bit����
		  switch (MeterDISymbol)
			{
				  case 0x02010100:													      		//��ѹ			
					         ChargerMsg.ChargeVoltage =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));		
					
				        break ;
#if 0
				  case 0x0201ff00:													      		//��ѹ��			
					         ChargerMsg.ChargeVoltage =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));										
				        break ;
#endif
					
					case 0x02020100:				                          	//����							
                   ChargerMsg.ChargeCurrent =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));
						    break;
#if 0
					case 0x0202ff00:				                          	//������							
                   ChargerMsg.ChargeCurrent =  NBCD_To_HEX_L(RMeter.Data,sizeof(RMeter.Data));
						    break;
#endif				
					case 0x00010000:					                  				//�����й��ܵ���	
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
** Descriptions:        DL645-2007��Լ֡��� ,���ش�����֡���ȣ����ݼ�0x33�����յ��ͨѶ��֡��ʽ������
** input parameters:    645��Լ֡��ʽ�ڵ������β�
** output parameters:   ��
** Returned value:      ���ش�����֡���ȣ����ݼ�0x33
*********************************************************************************************************/
static uint8_t  FramePack_645_2007(uint8_t *MeterNo,         //����      �βΣ����ݹ������ʱ��Ҫ���͹���������
                               uint8_t Ctlw,             //������
															 uint32_t IDSymbol,        //���ݱ�ʶ����
															 uint8_t *DataPtr,         //ָ�������ݣ�����������
															 uint8_t  DataLen,         //���ݳ���
															 uint8_t *SendBuff)        //ָ�������ݣ����ͻ�����
{  
		  uint8_t 	i;                                       //�������
		  uint8_t 	CCs;                                     //�������
		  uint8_t 	*pSendBuff;                              //����ָ֡��
		  pSendBuff = SendBuff;                              //ָ��ָ���ͻ����� ����ָ��ķ�ʽ��ʼ���ͻ�������������ݣ�����֡���  
		  *pSendBuff++ = 0xFE;  				 					        	 //�����ַ�
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;	
		  *pSendBuff++ = 0x68;                               //��ָ��ָ��λ�÷��ͻ��������������ʼ��   
		  memcpy((uint8_t *)&pSendBuff[0],MeterNo,6);        //�ѵ��Ŵӵ�0�����һλ���ݿ��������ͻ�������������6λ����
		   pSendBuff+=6;                                     //ʹpSendBuffָ��ָ���ַ������һ��֡��ʼ��
		  *pSendBuff++ = 0x68;                               //��ָ��ָ��λ�÷��ͻ��������������ʼ��   
		  *pSendBuff++ = Ctlw;                               //ָ��λ�ÿ�����
		  *pSendBuff++ = DataLen; 						               //ָ��λ�÷������ݳ���		  
		  if(DataLen>=4) 
		  {
				*pSendBuff++ = (IDSymbol%256)+0x33;  			       //���ݱ�ʶ
				*pSendBuff++ = (IDSymbol>>8)%256+0x33;
				*pSendBuff++ = (IDSymbol>>16)%256+0x33;
				*pSendBuff++ = (IDSymbol>>24)%256+0x33;
        for(i=0;i<(DataLen-4);i++) 
		   		{
                   *pSendBuff++ = 0x33 + *DataPtr++;   	//ÿ�����ݼ�0x33��ŵ�������
		  		}
		  }		  
		  CCs=0;
		  for(i=4;i<(DataLen+14);i++)                       //����У�飬ÿ�������ۼӺ͵���У��λ
		  {
              CCs=CCs+*(SendBuff+i);
		  }		  
		  *pSendBuff++ = CCs;                                //ָ��λ�÷���У��λ
		  
		  *pSendBuff++ = 0x16;                               //ָ��λ�÷��ý�����

		  return (DataLen+16);                               //���ش��������ݳ�+FE*4
}

/*********************************************************************************************************
** Function name:       FrameCheck_645_2007()
** Descriptions:        DL645-2007��Լ֡��� ,�ж������ͨ������֡�׵�ַƫ����������ԭ����
** input parameters:    645��Լ֡��ʽ�ڵ������β�
** output parameters:   ��
** Returned value:      �������ݳ���
*********************************************************************************************************/
static uint16_t  FrameCheck_645_2007( uint8_t *RcvBuff,  //�������ݻ�����   
                                      uint8_t *MeterNo,
                                      uint8_t *IDSymbol,
                                      uint8_t *DataPtr,  //ָ�������ݣ���������
                                      uint32_t mRcvLen)  //�������ݳ���
{
                                                
		uint16_t  k,mk;                                             
		uint8_t	Temp,CheckSum;
		
		for(mk=0;mk<mRcvLen;mk++)								
		{									   
			   if(RcvBuff[mk]==0x68&&RcvBuff[mk+7]==0x68)									//����ͨѶ֡�е�����0x68
			   {
					 Temp = RcvBuff[9+mk]+10;			                            //Ѱ�ҽ����ַ�16H��֡�е�λ��		
					 if(RcvBuff[mk+Temp+1]!=0x16)                             //�жϽ����ַ��Ƿ���ȷ
						 continue; 			                                        //�����˳�
					 CheckSum = 0;                                            //У��λ���������                     
					 for(k=0;k<Temp;k++)
						 CheckSum += RcvBuff[mk+k];			                        //��ȡУ��λ����		   
					 if(CheckSum!=RcvBuff[mk+Temp])                           //�ж�У��λ�Ƿ���ȷ
						 continue;                                              //�����˳�
					 memcpy(MeterNo,RcvBuff+mk+1,6);		                    	//������
				   Temp = RcvBuff[8+mk] & 0x80;				                     	//�ж�Ӧ�������D7�Ƿ�Ϊ1	
				   if(Temp == 0)
						 continue; 							                                //�����˳�
				   Temp = RcvBuff[8+mk] & 0x40;					                    //�ж�Ӧ�������D6�Ƿ�Ϊ1	
				   if(Temp != 0)
						 continue; 							                                //�����˳�				   
				   Temp = RcvBuff[8+mk] & 0x0f;                             //ȡ��������ĵ�4λ
				   if(Temp == 0x01 || Temp == 0x02)			                    //�жϿ�������ĺ���λ��
				   {
					   if(RcvBuff[mk+9] <= 4)                                 //���ݳ���С�ڵ���4���޳�
							 continue;		                                       				   
					   Temp = RcvBuff[9+mk];						                    	//��ȡ���ݳ��ȵ�ֵ 		
						 if(Temp>(12+4))                                        //Ӧ�����ݳ���12���ֽڵ��ӵ�
							 continue;									
					   for(k=0;k<4;k++)
						 {
					   	  IDSymbol[k] = RcvBuff[mk+10+k] - 0x33;              //IDSymbol��ȡ���ݱ�ʶ��
					   }						 
						 for(k=4;k<Temp;k++){	
							 if(RcvBuff[10+mk+k]==0x32 || RcvBuff[10+mk+k]==0x00 || RcvBuff[10+mk+k]==0xff)
								RcvBuff[10+mk+k]=0;
							 else
								DataPtr[k-4] = RcvBuff[10+mk+k] - 0x33;		          //��ԭ����
						 }
				   }
				   else if(Temp == 0x04)							                  		//�жϿ�������ĺ���λ��
				   {        
				   		 if(RcvBuff[mk+9] != 0)                               //���ݳ��Ȳ�Ϊ��
								 continue;	                   
				   		 Temp = 10;
				   		 CheckSum = 0;
					   	 for(k=0;k<Temp;k++)
							   CheckSum += RcvBuff[mk+k];                         //��ȡУ��λ����	 
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
