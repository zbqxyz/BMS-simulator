/****************************************Copyright (c)****************************************************
**                                  UEEE electronics Co.,LTD.
**
**                                 http://www.ueee.com
**          
**                                       wujikun
*********************************************************************************************************/
#ifndef _CAN_H_
#define _CAN_H_

#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <bms.h>

/*********************************************************************************************************
**               C++�������                                                                            
*********************************************************************************************************/
#ifdef __cplusplus
{
#endif


/*********************************************************************************************************
**                    Fpclk=60MHz CAN��׼������ֵ                                                       
*********************************************************************************************************/
#define BPS_1000K   0x160005
#define BPS_800K    0x2E0003
#define BPS_500K    0x2F0005
#define BPS_250K    0x2F000B
#define BPS_125K    0x2F0017
#define BPS_100K    0x1B0027
#define BPS_50K     0x2F003B
#define BPS_20K     0xFF0077
#define BPS_10K     0xFF00EF
#define BPS_5K      0xFF01DF
	
#define CANOFFSET	  0x4000
#define RGE(Address)	(*((volatile unsigned long*)Address))
#define	CANAPP_GLOBAL   extern


#define CAN_MSG_NUMBER 5           /* ���建�����洢��Ԫ��У�����һ�����������洢���� Ŀǰ����������ֵ���5ʱ��Ϊ���� */

/*********************************************************************************************************
**   ���� CAN �����շ��� �ṹ��:
*********************************************************************************************************/
typedef struct _MessageDetail
{
    unsigned char LEN;
    unsigned char FF;                                                   /* �Ƿ��׼֡                   */
    unsigned int CANID;                                                 /* ������FF�й�                 */
    unsigned int DATAA;
    unsigned int DATAB;
}MessageDetail;


extern MessageDetail MsgCAN0BUF[CAN_MSG_NUMBER];                        /*����CAN������  �����Ƚ��ȳ��������ݣ���ֹ����              */
extern MessageDetail *pwMsgCAN0,*prMsgCAN0;
extern MessageDetail MessageDetailT;                                    /* ���÷���ͨ�����ݱ���         */
extern MessageDetail MessageCAN0;                                       /* ����CAN0ͨ��֡����           */
extern MessageDetail MessageCAN1;                                       /* ����CAN1ͨ��֡����           */

extern unsigned int  CANID ;
extern unsigned char CAN_Data[8]; 
/*********************************************************************************************************
**   ���� CANMOD �Ĵ���:
1.This register controls the basic operating mode of the CAN Controller. 
2.Bits not listed read as 0 and should be written as 0.
*********************************************************************************************************/
typedef	union	_canmod_
{
    unsigned long DWord;
    struct {
        unsigned int	RM 		:1;                                     /* ����RMλ(��λλ)             */
        unsigned int	LOM 	:1;                                     /*����LOMλ(ֻ��ģʽλ)         */

        /*
        * ����STMλ "0"���͵���Ϣ���뱻Ӧ��ű��Ͽ�
        * "1"�Բ���ģʽ,CANCMR��SRRλһ��ʹ�ã����Դﵽ�Է����յ�Ч��
        */
        unsigned int	STM		:1; 
        /*
         * ����TPMλ "0"3�����ͻ����������ȼ��ɸ��Ե�CAN ID����
         * "1"3�����ͻ����������ȼ��ɸ��Ե�Tx���ȼ������
         */
        unsigned int	TPM 	:1;		
                
        unsigned int	SM 		:1;                                     /* ����SMλ(˯��ģʽλ)         */

        unsigned int	RPM 	:1;		
        /*
        * ����RPMλ "0"��������������λ��Rx��Tx��Ϊ�͵�ƽ
        * "1"��ת����ģʽ--��������������λ��Rx��Ϊ�ߵ�ƽ
        */
        unsigned int	RSV1 	:1;                                     /* ����λ                       */
        unsigned int	TM 		:1;                                     /* ����TMλ(����ģʽ)           */
        unsigned int	RSV24	:24;                                    /* ����λ                       */
    }Bits;
}REG_CANMOD;

/*********************************************************************************************************
** Function name:  	unsigned char Enter_SWRst(unsigned char Ch);
** Descriptions:  	Ch:CAN�������ţ�0��ʾ��һ·CAN,CAN���������������λģʽ
** Returned value:      0��          ʧ��
**                      1:           �ɹ�	
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char Enter_SWRst(unsigned char Ch);

/**********************************************************************************************************
** Function name:       unsigned char Quit_SWRst(unsigned char Ch);
** Descriptions:        Ch:CAN�������ţ�0��ʾ��һ·CAN,CAN�������˳������λģʽ
** Returned value:      0��          ʧ��
**                      1:           �ɹ�	
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char Quit_SWRst(unsigned char Ch);

/*********************************************************************************************************
** Function name:   	unsigned char CAN_Init(unsigned char Ch, unsigned long Baud);
** Descriptions:        CAN�������˳������λģʽ
** input parameters:    Ch:          CAN�������ţ�0��ʾ��һ·CAN
**                      Baud:        CAN������ֵ
** Returned value:      ��
*********************************************************************************************************/
CANAPP_GLOBAL void CAN_Init(unsigned char Ch, unsigned long Baud);

/*********************************************************************************************************
** Function name:       unsigned char CANRCV(unsigned char Ch, MessageDetail MessageCAN)
** Descriptions:        ����CAN֡����ָ���Ľṹ����	
** input parameters:    Ch:          CAN�������ţ�0��ʾ��һ·CAN
**                      *MessageCAN: ���CAN֡��Ϣ�Ľṹ��
** Output parameters:   *MessageCAN: ���CAN֡��Ϣ�Ľṹ��
** Returned value:      ��
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char CANRCV(unsigned char Ch, MessageDetail *MessageCAN);

/*********************************************************************************************************
** Function name:  	unsigned char WriteCAN0(unsigned char LEN,unsigned char FF, unsigned int 
**                      ID, unsigned char *data);
** Descriptions: 	�û���д���� ֡��Ϣ
** input parameters:    LEN:         ���ݳ���
**                      *data:       �����������ڵ����� 
** Returned value:      �����ɹ����� 1 ʧ�ܷ��� 0
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char WriteCAN0(unsigned char LEN,unsigned char FF,
                                        unsigned int ID,unsigned char *data);

/*********************************************************************************************************
** Function name: 	unsigned char CANSend(unsigned char Ch, unsigned char BUFNum);
** Descriptions:        CAN�������˳������λģʽ
** input parameters:    Ch:CAN�������ţ�0��ʾ��һ·CAN
**                       BUFNum ѡ�񻺳���
                         MessageDetailT ���ĵ�������Ϣ������
** Returned value:��
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char CANSend(unsigned char Ch, unsigned char BUFNum);
/*********************************************************************************************************
** Functoin name:               CAN_IRQHandler
** Descriptions:  	            CAN�������жϴ�����
** input paraments:	            ��
** output paraments:	    	��	
** Returned values:	   			��
*********************************************************************************************************/
CANAPP_GLOBAL void CAN_IRQHandler (void);
CANAPP_GLOBAL void CAN0_RECData_Pro(void);
CANAPP_GLOBAL void CAN1_RECData_Pro(void);


#ifdef __cplusplus

#endif

#endif

/********************************************************************************************************
**                                        End Of File                                                                             
********************************************************************************************************/
