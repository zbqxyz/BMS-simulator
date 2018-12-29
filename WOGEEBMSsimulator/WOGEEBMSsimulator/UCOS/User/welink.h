#ifndef __WELINK_H_
#define __WELINK_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <ucos_ii.h>
#include "gpio.h"
#include <rtc.h>
#include <uart.h>
#include <ethernet.h>
#include <link.h>
#include <rtc.h>

#define WelinkMode_Ethernet   1
#define WelinkMode_4G         2

#define HeartBeatTime         30     //�������
#define HeartBeatTimeOut      3      //������Ӧ��ʱ���Դ���

extern    uint8_t         WelinkMode;         /*  ͨѶģʽ                  */
extern    uint8_t         TcpConFlag;			    /*  ��·��ͨ��־          		*/
extern    uint8_t         MsaConFlag;			    /*  ��վ���߱�־          		*/

//extern    uint8_t         DataNo;      
	
typedef struct{
	uint8_t  head[2];     //�̶���ʶ 0xfa 0xf5  
	uint16_t len;         //����
	uint8_t  encrypt;     //������Ϣλ
	uint8_t  sn;          //���к�
	uint16_t cmd;         //����
	uint8_t  data[256];   //����
	uint8_t  cs;          //У���
}Report;

typedef struct
{
  	uint16_t  	ParaFlag ;
	  uint16_t		SoftVer ;				 	  	/*�ն�����汾��*/
	  uint16_t		HardwareVer ;					/*�ն�Ӳ���汾��*/
	  uint8_t		  TermNo[32];						/*�ն˻������*/
		uint8_t		  TermType; 						/*���׮����*/	
		uint16_t		ItemType; 				  	/*��Ŀ����*/
	  uint32_t		StartTimes; 				  /*��������*/
	  uint8_t		  UploadMode ;					/*�ϴ�ģʽ*/	
	  uint16_t		CheckInTime;			  	/*ǩ�����ʱ��*/
	  uint8_t     InternalVariable;     /*�ڲ�����*/
	  uint8_t     ChargePort;           /*���ǹ����*/
	  uint8_t     HeartRate;            /*��������*/
	  uint8_t     HeartRateTimeOut;     /*��������*/	
	  uint8_t      SicFlag;              /*���ܱ�־*/
	  uint32_t     ReportNo;             /*����¼����*/
	
	  uint8_t		  SocketType ;					/*���׮ͨ��ģʽ*/	
    uint8_t     ServerIP[4];   	    	/*������IP*/ 
    uint16_t   	ServerPort ;					/*�������˿�*/
	  uint8_t     GateWay[4] ;          /*����*/
    uint8_t		   IP[4] ;						  /*����IP*/
    uint16_t   	 Port ;						    /*����˿�*/
    uint8_t 	   MASK[4];           	/*��������*/
    uint8_t 		 MAC[6];            	/*�����ַ*/
    uint16_t   	RatedVoltage ;				/*���ѹ*/
    uint16_t   	RatedCurrent ;				/*�����*/
    uint8_t   	JudgeTime ;						/*���ء���ѹ��Ƿѹ�ж�ʱ��*/
  	uint8_t		  HeartBeattimes;				/*�������� ��λ min*/
	  uint8_t		  ComTimeOut ;					/*ͨѶ��ʱʱ��*/
	  uint8_t		  Repeat ;				    	/*�ط����� */
	  uint8_t		  DutyCycle;						/*ռ�ձ�*/
	  uint8_t		  FeiLv[2];							/*����*/
		uint8_t     RegisterState;        /*ǩ��״̬*/
		uint8_t			AutoUpload;           /*�Զ��ϴ�*/       	
	  uint8_t	  	CS ;
}WELINK_PARA;



extern  WELINK_PARA   Welink_PARA;



void UpstreamCmdHandle(uint16_t cmd,uint8_t port);
extern void WelinkMain(void );
extern void RecvDataHandle( void );



#endif

