/****************************************Copyright (c)****************************************************
**                                    Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               link.c
** Last modified Date:      2018.06.30
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include "link.h"

static uint16_t HeartBeatCnt;

/*********************************************************************************************************
** Function name:       cmd_02_upload
** Descriptions:        ���׮�����β���������Ӧ���������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_02_upload(uint8_t *pbgn,uint8_t port)
 {
	 return 0;
 }
 
 /*********************************************************************************************************
** Function name:       cmd_04_upload
** Descriptions:        ���׮���ַ�����������Ӧ���������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_04_upload(uint8_t *pbgn,uint8_t port)
 {
	 
	 
	 return 0;
 }
 
 /*********************************************************************************************************
** Function name:       cmd_06_upload
** Descriptions:        ���׮�ĺ�̨��������Ӧ��
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_06_upload(uint8_t *pbgn,uint8_t port)
 {
	  uint8_t *pdat;
  	uint8_t i;
	
	 pdat = pbgn;	 
	
	*pdat++ = 0x00;                                                //ҵ������              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //Ԥ��
	*pdat++ = 0;
	 
	 for(i=0;i<32;i++)                                             //׮���  32�ֽ�
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }	 
	 
	*pdat++ = 0;                                                   //���ǹ�� 1�ֽ�
	 
	*pdat++ = 0;	                                                 //������ʼ��־ 4�ֽ�
	*pdat++ = 0;	                                                 //������ʼ��־ 4�ֽ�	 
	*pdat++ = 0;	                                                 //������ʼ��־ 4�ֽ� 
	*pdat++ = 0;	                                                 //������ʼ��־ 4�ֽ�	 
	 	 
	*pdat++ = 0;	                                                 //������� 
	*pdat++ = 0;	                                                 //����ִ�н��
	 
	return pdat-pbgn;
 }
 
 /*********************************************************************************************************
** Function name:       cmd_08_upload
** Descriptions:        Ӧ���̨���ƿ����������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_08_upload(uint8_t *pbgn,uint8_t port)
 {
  uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	 
	
	*pdat++ = 0x00;                                                //ҵ������              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //Ԥ��
	*pdat++ = 0;
	 
	 for(i=0;i<32;i++)                                             //׮���  32�ֽ�
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }	 
	 
	*pdat++ = 0;                                                   //���ǹ�� 1�ֽ�
	 
	*pdat++ = 0;	                                                 //����ִ�н�� 4�ֽ�
	*pdat++ = 0;	                                                 //����ִ�н�� 4�ֽ�	 
	*pdat++ = 0;	                                                 //����ִ�н�� 4�ֽ�	 
	*pdat++ = 0;	                                                 //����ִ�н�� 4�ֽ�	 
	 	 
	return pdat-pbgn;
 }
 /*********************************************************************************************************
** Function name:       cmd_10_upload
** Descriptions:        �ϴ���������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_10_upload(uint8_t *pbgn,uint8_t port)
 {
	 return 0;
 }

/*********************************************************************************************************
** Function name:       cmd_102_upload
** Descriptions:        �����ϱ�����
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_102_upload(uint8_t *pbgn,uint8_t port)
{
	uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	
	
	*pdat++ = 0xff;                                                //ҵ������              
	*pdat++ = 0xff;
	
	*pdat++ = 0;                                                   //Ԥ��
	*pdat++ = 0;
	
	 for(i=0;i<32;i++)                                             //׮���  32�ֽ�
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	 
	  HeartBeatCnt++;
	 *pdat++ = HeartBeatCnt%256;                                  //������������         
	 *pdat++ = HeartBeatCnt/256;
	 
	return pdat-pbgn;
	 
}
/*********************************************************************************************************
** Function name:       cmd_104_upload
** Descriptions:        ���׮״̬��Ϣ�ϱ�
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_104_upload(uint8_t *pbgn,uint8_t port)
 {
	uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	
	
	*pdat++ = 0x0d;                                                //ҵ������              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //Ԥ��
	*pdat++ = 0;
	
	 for(i=0;i<32;i++)                                             //׮���  32�ֽ�
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	 *pdat++ = 1;                                                  //���ǹ���� 1���ֽ�
	 *pdat++ = 1;                                                  //����     1���ֽ� 
	 *pdat++ = 1;                                                  //���׮���� 1���ֽ�
	 *pdat++ = 0;                                                  //���׮����״̬  ���� or ������ 1���ֽ�
	 *pdat++ = 0;                                                  //��ǰSOC    1���ֽ�

	 *pdat++ = 0;                                                  //�澯����   4���ֽ�
	 *pdat++ = 0;                                                  //�澯����   4���ֽ�
	 *pdat++ = 0;                                                  //�澯����   4���ֽ�
	 *pdat++ = 0;                                                  //�澯����   4���ֽ�
 
	 *pdat++ = 0;                                                  //������״̬ 1���ֽ�

	 *pdat++ = 0;                                                  //���γ���ۼƳ�����  4���ֽ�	 
	 *pdat++ = 0;                                                  //���γ���ۼƳ�����  4���ֽ�
	 *pdat++ = 0;                                                  //���γ���ۼƳ�����  4���ֽ�
	 *pdat++ = 0;                                                  //���γ���ۼƳ�����  4���ֽ�

	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�	 
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�	 
	 *pdat++ = 0;                                                  //�ڲ����� 8���ֽ�
	 
	 *pdat++ = 0;                                                  //ֱ������ѹ 2���ֽ�
	 *pdat++ = 0;                                                  //ֱ������ѹ 2���ֽ�	 
	 
	 *pdat++ = 0;                                                  //ֱ�������� 2���ֽ�
	 *pdat++ = 0;                                                  //ֱ�������� 2���ֽ�	 	 
	 
	 *pdat++ = 0;                                                  //BMS�����ѹ  2���ֽ�
	 *pdat++ = 0;                                                  //BMS�����ѹ  2���ֽ�
	 
	 *pdat++ = 0;                                                  //BMS�������  2���ֽ�
	 *pdat++ = 0;                                                  //BMS�������  2���ֽ� 
	 	 
	 *pdat++ = 0;                                                  //BMS���ģʽ	 
	 
	 *pdat++ = 0;                                                  //����A�����ѹ 2���ֽ�
	 *pdat++ = 0;                                                  //����A�����ѹ 2���ֽ�
	 
	 *pdat++ = 0;                                                  //����B�����ѹ 2���ֽ�
	 *pdat++ = 0;                                                  //����B�����ѹ 2���ֽ�	 	 
	 
	 *pdat++ = 0;                                                  //����C�����ѹ 2���ֽ�
	 *pdat++ = 0;                                                  //����C�����ѹ 2���ֽ�
	 
	 *pdat++ = 0;                                                  //����A������� 2���ֽ�
	 *pdat++ = 0;                                                  //����A������� 2���ֽ�	 
	 
	 *pdat++ = 0;                                                  //����B������� 2���ֽ�
	 *pdat++ = 0;                                                  //����B������� 2���ֽ�	 
	 
	 *pdat++ = 0;                                                  //����C������� 2���ֽ�
	 *pdat++ = 0;                                                  //����C������� 2���ֽ�	 
	 
	 *pdat++ = 0;                                                  //ʣ����ʱ��    2���ֽ�
	 *pdat++ = 0;                                                  //ʣ����ʱ��    2���ֽ� 

	 *pdat++ = 0;                                                  //���ʱ��    4���ֽ�
	 *pdat++ = 0;                                                  //���ʱ��    4���ֽ� 
	 *pdat++ = 0;                                                  //���ʱ��    4���ֽ�
	 *pdat++ = 0;                                                  //���ʱ��    4���ֽ�

	 *pdat++ = 0;                                                  //������   4���ֽ�
	 *pdat++ = 0;                                                  //������   4���ֽ�
	 *pdat++ = 0;                                                  //������   4���ֽ�	 
	 *pdat++ = 0;                                                  //������   4���ֽ�	 
	 
	 *pdat++ = 0;                                                  //���ǰ������   4���ֽ�	 
	 *pdat++ = 0;                                                  //���ǰ������   4���ֽ�	
	 *pdat++ = 0;                                                  //���ǰ������   4���ֽ�	
	 *pdat++ = 0;                                                  //���ǰ������   4���ֽ�	
	 
	 *pdat++ = 0;                                                  //��ǰ������  4���ֽ�	
	 *pdat++ = 0;                                                  //��ǰ������  4���ֽ�	
	 *pdat++ = 0;                                                  //��ǰ������  4���ֽ�	
	 *pdat++ = 0;                                                  //��ǰ������  4���ֽ�		

	 *pdat++ = 0;                                                  //���������ʽ

	 *pdat++ = 0;                                                  //������ 1���ֽ�
	 *pdat++ = 0;                                                  //������	4���ֽ�
	 *pdat++ = 0;                                                  //������	4���ֽ�	 
	 *pdat++ = 0;                                                  //������	4���ֽ�	 
	 *pdat++ = 0;                                                  //������	4���ֽ�	 
	 
	 *pdat++ = 0;                                                  //ԤԼ��־	 
	 
	 for(i=0;i<32;i++)                                             //��翨��  32�ֽ�
	 {
	   *pdat++ = 0;
	 }
	 
	 *pdat++ = 0;                                                  //ԤԼ��ʱʱ�� 1�ֽ�
	 
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�	 
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�	 
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�	 
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //ԤԼ��ʼ���ʱ�� 8�ֽ�

	 *pdat++ = 0;                                                  //���ǰ�����  4�ֽ�	 
	 *pdat++ = 0;                                                  //���ǰ�����  4�ֽ�	 	 	 
	 *pdat++ = 0;                                                  //���ǰ�����  4�ֽ�	
	 *pdat++ = 0;                                                  //���ǰ�����  4�ֽ�		 	 
	 
	 *pdat++ = 0;                                                  //Ԥ��  4�ֽ�	 
	 *pdat++ = 0;                                                  //Ԥ��  4�ֽ�	 
	 *pdat++ = 0;                                                  //Ԥ��  4�ֽ�	 	 
	 *pdat++ = 0;                                                  //Ԥ��  4�ֽ�	
	 
	 *pdat++ = 0;                                                  //��繦�� 4�ֽ�	 
	 *pdat++ = 0;                                                  //��繦�� 4�ֽ�	 	 	 	 
	 *pdat++ = 0;                                                  //��繦�� 4�ֽ�	 
	 *pdat++ = 0;                                                  //��繦�� 4�ֽ�	 	

	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�
	 *pdat++ = 0;                                                  //ϵͳ���� 12���ֽ�	 
	 
	 *pdat++ = 0;                                                  //������¶ȣ�ƫ����-50  1�ֽ�
	 *pdat++ = 0;                                                  //�����¶ȣ�  ƫ����-50  1�ֽ�
	 *pdat++ = 0;	                                                 //������¶ȣ�ƫ����-50  1�ֽ�                                          
 
	return pdat-pbgn;
	  
 }

/*********************************************************************************************************
** Function name:       cmd_106_upload
** Descriptions:        ���׮ǩ����Ϣ�ϱ�
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_106_upload(uint8_t *pbgn,uint8_t port)
{
	uint8_t *pdat;
	uint16_t i;
	
	 pdat = pbgn;
	*pdat++ = 0x00;                                               //AES ��������Ч  2�ֽ�
	*pdat++ = 0x00;
	*pdat++ = 0xff;                                               //Ԥ��  2�ֽ�
	*pdat++ = 0xff;
	
	 for(i=0;i<32;i++)                                            //׮���  32�ֽ�
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	  *pdat++ = Welink_PARA.SicFlag;
    *pdat++ = Welink_PARA.SoftVer%256;                         //����汾�� 4�ֽ�
    *pdat++ = (Welink_PARA.SoftVer>>4)&0xff;
	  *pdat++ = (Welink_PARA.SoftVer>>8)&0xff;
	  *pdat++ = (Welink_PARA.SoftVer>>16)&0xff;
	 
	  *pdat++ = Welink_PARA.ItemType%256;                        //���׮��Ŀ���� 2�ֽ�    
	  *pdat++ = Welink_PARA.ItemType/256;
	 
    *pdat++ = Welink_PARA.StartTimes%256;                      //�������� 4�ֽ�
    *pdat++ = (Welink_PARA.StartTimes>>4)&0xff;
	  *pdat++ = (Welink_PARA.StartTimes>>8)&0xff;
	  *pdat++ = (Welink_PARA.StartTimes>>16)&0xff;	    
	  
	  *pdat++ = Welink_PARA.UploadMode;                           //�����ϴ�ģʽ
	 
	  *pdat++ = Welink_PARA.CheckInTime%256;                     //ǩ�����ʱ��
	  *pdat++ = Welink_PARA.CheckInTime/256;
	 
	  *pdat++ = Welink_PARA.InternalVariable;                     //�����ڲ�����	 
	  *pdat++ = Welink_PARA.ChargePort;                           //���ǹ����
	  *pdat++ = Welink_PARA.HeartRate;                            //��������
	  *pdat++ = Welink_PARA.HeartRateTimeOut;                     //�������ڳ�ʱ����
		
	  *pdat++ = Welink_PARA.ReportNo%256;                      //�������� 4�ֽ�
    *pdat++ = (Welink_PARA.ReportNo>>4)&0xff;
	  *pdat++ = (Welink_PARA.ReportNo>>8)&0xff;
	  *pdat++ = (Welink_PARA.ReportNo>>16)&0xff;
				
	  *pdat++ = HEX_To_BCD((RTCtimedate.year>>4)&0xff);           //��ǰϵͳʱ��
		*pdat++ = HEX_To_BCD(RTCtimedate.year%256);
	  *pdat++ = HEX_To_BCD(RTCtimedate.month);
		*pdat++ = HEX_To_BCD(RTCtimedate.day);
		*pdat++ = HEX_To_BCD(RTCtimedate.hour);
	  *pdat++ = HEX_To_BCD(RTCtimedate.minute);
		*pdat++ = HEX_To_BCD(RTCtimedate.second);
		*pdat++ = 0xff;                                             //���������
	
	  for(i=0;i<24;i++)                                            //Ԥ��  32�ֽ�
	 {
	   *pdat++ = 0x00;
	 }	
	 
    for(i=0;i<4;i++)                                            //�����  4�ֽ�
	 {
	   *pdat++ =  rand()%256;
	 }	
				
	return pdat-pbgn;
	
}

/*********************************************************************************************************
** Function name:       cmd_108_upload
** Descriptions:        ���׮�ĸ澯��Ϣ�ϱ�
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_108_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}

/*********************************************************************************************************
** Function name:       cmd_110_upload
** Descriptions:        ����������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_110_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}

/*********************************************************************************************************
** Function name:       cmd_202_upload
** Descriptions:        ����¼�ϱ�
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_202_upload(uint8_t *pbgn,uint8_t port)
{
	uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	
	
	*pdat++ = 0x0d;                                                //ҵ������              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //Ԥ��
	*pdat++ = 0;
	
	 for(i=0;i<32;i++)                                             //׮���  32�ֽ�
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	 *pdat++ = 1;                                                  //���ǹλ������ 1���ֽ�
	 *pdat++ = 1;                                                  //����     1���ֽ� 

	 for(i=0;i<32;i++)                                             //��翨��  32�ֽ�
	 {
	   *pdat++ = 0;
	 }

	 *pdat++ = 0;                                                  //��翪ʼʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ��	8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ��	8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ��	8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //��翪ʼʱ�� 8�ֽ�

	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�
	 *pdat++ = 0;                                                  //������ʱ�� 8�ֽ�

	 *pdat++ = 0;                                                  //���ʱ�䳤�� 4�ֽ� 
	 *pdat++ = 0;                                                  //���ʱ�䳤�� 4�ֽ�
	 *pdat++ = 0;                                                  //���ʱ�䳤�� 4�ֽ�
	 *pdat++ = 0;                                                  //���ʱ�䳤�� 4�ֽ�

	 *pdat++ = 0;                                                  //��ʼSOC
	 *pdat++ = 0;                                                  //����SOC
	 
	 *pdat++ = 0;                                                  //������ԭ�� 4�ֽ�
	 *pdat++ = 0;                                                  //������ԭ�� 4�ֽ�
	 *pdat++ = 0;                                                  //������ԭ�� 4�ֽ�
	 *pdat++ = 0;                                                  //������ԭ�� 4�ֽ�

	 *pdat++ = 0;                                                  //���γ����� 4�ֽ�
	 *pdat++ = 0;                                                  //���γ����� 4�ֽ�	 
	 *pdat++ = 0;                                                  //���γ����� 4�ֽ�	 
	 *pdat++ = 0;                                                  //���γ����� 4�ֽ�	 
	 
	 *pdat++ = 0;                                                  //���ǰ������ 4�ֽ� 
	 *pdat++ = 0;                                                  //���ǰ������ 4�ֽ� 	 
	 *pdat++ = 0;                                                  //���ǰ������ 4�ֽ� 
	 *pdat++ = 0;                                                  //���ǰ������ 4�ֽ� 	 
	 
	 *pdat++ = 0;                                                  //��������� 4�ֽ� 	 
	 *pdat++ = 0;                                                  //��������� 4�ֽ� 		 
	 *pdat++ = 0;                                                  //��������� 4�ֽ� 		 
	 *pdat++ = 0;                                                  //��������� 4�ֽ� 		 

	 *pdat++ = 0;                                                  //���γ���� 4�ֽ� 	
	 *pdat++ = 0;                                                  //���γ���� 4�ֽ� 
	 *pdat++ = 0;                                                  //���γ���� 4�ֽ� 
	 *pdat++ = 0;                                                  //���γ���� 4�ֽ� 

	 *pdat++ = 0;                                                  //Ԥ��
	 *pdat++ = 0;                                                  //Ԥ��
	 *pdat++ = 0;                                                  //Ԥ��
	 *pdat++ = 0;                                                  //Ԥ��

	 *pdat++ = 0;                                                  //���ǰ����� 4�ֽ�
	 *pdat++ = 0;                                                  //���ǰ����� 4�ֽ�
	 *pdat++ = 0;                                                  //���ǰ����� 4�ֽ�
	 *pdat++ = 0;                                                  //���ǰ����� 4�ֽ�

	 *pdat++ = 0;                                                  //����¼���� 4�ֽ�
	 *pdat++ = 0;                                                  //����¼���� 4�ֽ�
	 *pdat++ = 0;                                                  //����¼���� 4�ֽ�
	 *pdat++ = 0;                                                  //����¼���� 4�ֽ�

	 *pdat++ = 0;                                                  //�ܳ���¼��Ŀ 4�ֽ�
	 *pdat++ = 0;                                                  //�ܳ���¼��Ŀ 4�ֽ�
	 *pdat++ = 0;                                                  //�ܳ���¼��Ŀ 4�ֽ�
	 *pdat++ = 0;                                                  //�ܳ���¼��Ŀ 4�ֽ�

	 *pdat++ = 0;                                                  //Ԥ�� 1�ֽ�
	 
	 *pdat++ = 0;                                                  //������ 1�ֽ�
	 *pdat++ = 0;                                                  //�����Բ��� 4�ֽ�
	 *pdat++ = 0;                                                  //�����Բ��� 4�ֽ�
	 *pdat++ = 0;                                                  //�����Բ��� 4�ֽ�	 
	 *pdat++ = 0;                                                  //�����Բ��� 4�ֽ�
	 
	 for(i=0;i<17;i++)                                             //����VIN  17�ֽ�
	 {
	   *pdat++ = 0;
	 }

	 for(i=0;i<8;i++)                                              //���ƺ�  8�ֽ�
	 {
	   *pdat++ = 0;
	 }

	 for(i=0;i<96;i++)                                              //ʱ�ε���  96�ֽ�
	 {
	   *pdat++ = 0;
	 }

	 *pdat++ = 0;                                                  //������ʽ 1�ֽ�
	 
	 for(i=0;i<32;i++)                                              //�����ˮ��  96�ֽ�
	 {
	   *pdat++ = 0;
	 }
	 
	 	return pdat-pbgn;	
	
}

/*********************************************************************************************************
** Function name:       cmd_204_upload
** Descriptions:        �û��˻���ѯ�ϱ�
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_204_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}

/*********************************************************************************************************
** Function name:       cmd_206_upload
** Descriptions:        �û�������֤�ϱ�
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_206_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}



/*********************************************************************************************************
** Function name:       cmd_1_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_1_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_3_query_2_proess
** Descriptions:        Э��ͨѶʱ���ѯ������
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_3_query_2_proess(uint8_t *p,uint16_t len,uint8_t port)
{
	
	
	
}

/*********************************************************************************************************
** Function name:       cmd_3_setup_2_proess
** Descriptions:        Э��ͨѶʱ�����ô�����
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_3_setup_2_proess(uint8_t *p,uint16_t len,uint8_t port)
{
	
	
	
}


/*********************************************************************************************************
** Function name:       cmd_3_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_3_proess(uint8_t *p,uint16_t len,uint8_t port)
{ 
   uint32_t  para_data_addr;                            //������ַ
	
	 para_data_addr =(*(p+5))+(*(p+6)*256)+(*(p+7)*256*256)+(*(p+8)*256*256*256);
	
    if(*(p+4)==0)                                        //��ѯѰ����
		{	
			switch (para_data_addr)
			{
				case 1:                                          //��ѯ���׮����
					
					   break;
								
				case 2:                                          //��ѯ��׼ʱ��ʱ��
					   break;	
					 
		   	case 3:                                          //��ѯ����Ա����
			  		cmd_3_query_2_proess(p+10,*(p+9),0);
					   break;
				
		   	case 4:                                          //��ѯ����Ա����
					   break;				
				
		   	case 5:                                          //��ѯMAC��ַ
					   break;			
		   	case 6:                                          //Ԥ��
					   break;	
		   	case 7:                                          //��ѯ��ά��
					   break;		
				
				default:
					   break;				   
				 
			}
		 UpstreamCmdHandle(4,port);                          //��������	
		}		
    else if(*(p+4)==1)                                   //���ò���
		{	
			switch (para_data_addr)
			{
				case 1:                                          //���ó��׮����
					
					   break;
								
				case 2:                                          //���ñ�׼ʱ��ʱ��
					  cmd_3_setup_2_proess(p+10,*(p+9),0);
				    
					   break;	
					 
		   	case 3:                                          //���ù���Ա����
					   break;
				
		   	case 4:                                          //���ò���Ա����
					   break;				
				
		   	case 5:                                          //����MAC��ַ
					   break;			
		   	case 6:                                          //Ԥ��
					   break;	
		   	case 7:                                          //���ö�ά��
					   break;		
				
				default:
					   break;				   
				 
			}
				
		}
		
		
}

/*********************************************************************************************************
** Function name:       cmd_5_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_5_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_7_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_7_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_9_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_9_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_101_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_101_proess(uint8_t *p,uint16_t len,uint8_t port)
{	
    
		Ethernet_Reply_Time=0;

}

/*********************************************************************************************************
** Function name:       cmd_103_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_103_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

	 UpstreamCmdHandle(104,port);                          //��������	
}

/*********************************************************************************************************
** Function name:       cmd_0x105_proess
** Descriptions:        ����������Ӧ����׮ǩ������
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_105_proess(uint8_t *p,uint16_t len,uint8_t port)
{	
   	uint8_t *pdat;
	
	   pdat = p;	
	   MsaConFlag  = 1;                                      //���߱�־λ ��λ
		 UpstreamCmdHandle(102,port);                          //��������
		
}

/*********************************************************************************************************
** Function name:       cmd_107_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_107_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_109_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_109_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_201_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_201_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_203_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_203_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_205_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_205_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}
