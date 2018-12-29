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
**               C++代码兼容                                                                            
*********************************************************************************************************/
#ifdef __cplusplus
{
#endif


/*********************************************************************************************************
**                    Fpclk=60MHz CAN标准波特率值                                                       
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


#define CAN_MSG_NUMBER 5           /* 定义缓冲区存储单元大校最后一个缓冲区不存储数据 目前发现这个数字等于5时较为合适 */

/*********************************************************************************************************
**   定义 CAN 报文收发项 结构体:
*********************************************************************************************************/
typedef struct _MessageDetail
{
    unsigned char LEN;
    unsigned char FF;                                                   /* 是否标准帧                   */
    unsigned int CANID;                                                 /* 长度与FF有关                 */
    unsigned int DATAA;
    unsigned int DATAB;
}MessageDetail;


extern MessageDetail MsgCAN0BUF[CAN_MSG_NUMBER];                        /*定义CAN缓冲曲  用于先进先出处理数据，防止丢包              */
extern MessageDetail *pwMsgCAN0,*prMsgCAN0;
extern MessageDetail MessageDetailT;                                    /* 引用发送通道数据变量         */
extern MessageDetail MessageCAN0;                                       /* 引用CAN0通道帧变量           */
extern MessageDetail MessageCAN1;                                       /* 引用CAN1通道帧变量           */

extern unsigned int  CANID ;
extern unsigned char CAN_Data[8]; 
/*********************************************************************************************************
**   定义 CANMOD 寄存器:
1.This register controls the basic operating mode of the CAN Controller. 
2.Bits not listed read as 0 and should be written as 0.
*********************************************************************************************************/
typedef	union	_canmod_
{
    unsigned long DWord;
    struct {
        unsigned int	RM 		:1;                                     /* 定义RM位(复位位)             */
        unsigned int	LOM 	:1;                                     /*定义LOM位(只听模式位)         */

        /*
        * 定义STM位 "0"发送的信息必须被应答才被认可
        * "1"自测试模式,CANCMR的SRR位一起使用，可以达到自发自收的效果
        */
        unsigned int	STM		:1; 
        /*
         * 定义TPM位 "0"3个发送缓冲区的优先级由各自的CAN ID决定
         * "1"3个发送缓冲区的优先级由各自的Tx优先级域决定
         */
        unsigned int	TPM 	:1;		
                
        unsigned int	SM 		:1;                                     /* 定义SM位(睡眠模式位)         */

        unsigned int	RPM 	:1;		
        /*
        * 定义RPM位 "0"如果传输的是显性位，Rx和Tx脚为低电平
        * "1"翻转极性模式--如果传输的是显性位，Rx脚为高电平
        */
        unsigned int	RSV1 	:1;                                     /* 保留位                       */
        unsigned int	TM 		:1;                                     /* 定义TM位(测试模式)           */
        unsigned int	RSV24	:24;                                    /* 保留位                       */
    }Bits;
}REG_CANMOD;

/*********************************************************************************************************
** Function name:  	unsigned char Enter_SWRst(unsigned char Ch);
** Descriptions:  	Ch:CAN控制器号，0表示第一路CAN,CAN控制器进入软件复位模式
** Returned value:      0：          失败
**                      1:           成功	
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char Enter_SWRst(unsigned char Ch);

/**********************************************************************************************************
** Function name:       unsigned char Quit_SWRst(unsigned char Ch);
** Descriptions:        Ch:CAN控制器号，0表示第一路CAN,CAN控制器退出软件复位模式
** Returned value:      0：          失败
**                      1:           成功	
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char Quit_SWRst(unsigned char Ch);

/*********************************************************************************************************
** Function name:   	unsigned char CAN_Init(unsigned char Ch, unsigned long Baud);
** Descriptions:        CAN控制器退出软件复位模式
** input parameters:    Ch:          CAN控制器号，0表示第一路CAN
**                      Baud:        CAN波特率值
** Returned value:      无
*********************************************************************************************************/
CANAPP_GLOBAL void CAN_Init(unsigned char Ch, unsigned long Baud);

/*********************************************************************************************************
** Function name:       unsigned char CANRCV(unsigned char Ch, MessageDetail MessageCAN)
** Descriptions:        接受CAN帧放在指定的结构体中	
** input parameters:    Ch:          CAN控制器号，0表示第一路CAN
**                      *MessageCAN: 存放CAN帧信息的结构体
** Output parameters:   *MessageCAN: 存放CAN帧信息的结构体
** Returned value:      无
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char CANRCV(unsigned char Ch, MessageDetail *MessageCAN);

/*********************************************************************************************************
** Function name:  	unsigned char WriteCAN0(unsigned char LEN,unsigned char FF, unsigned int 
**                      ID, unsigned char *data);
** Descriptions: 	用户填写发送 帧信息
** input parameters:    LEN:         数据长度
**                      *data:       发送数据所在的数组 
** Returned value:      操作成功返回 1 失败返回 0
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char WriteCAN0(unsigned char LEN,unsigned char FF,
                                        unsigned int ID,unsigned char *data);

/*********************************************************************************************************
** Function name: 	unsigned char CANSend(unsigned char Ch, unsigned char BUFNum);
** Descriptions:        CAN控制器退出软件复位模式
** input parameters:    Ch:CAN控制器号，0表示第一路CAN
**                       BUFNum 选择缓冲区
                         MessageDetailT 报文的特征信息及数据
** Returned value:无
*********************************************************************************************************/
CANAPP_GLOBAL unsigned char CANSend(unsigned char Ch, unsigned char BUFNum);
/*********************************************************************************************************
** Functoin name:               CAN_IRQHandler
** Descriptions:  	            CAN控制器中断处理函数
** input paraments:	            无
** output paraments:	    	无	
** Returned values:	   			无
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
