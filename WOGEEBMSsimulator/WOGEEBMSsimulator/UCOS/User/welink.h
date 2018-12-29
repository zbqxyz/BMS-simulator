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

#define HeartBeatTime         30     //心跳间隔
#define HeartBeatTimeOut      3      //心跳响应超时重试次数

extern    uint8_t         WelinkMode;         /*  通讯模式                  */
extern    uint8_t         TcpConFlag;			    /*  链路联通标志          		*/
extern    uint8_t         MsaConFlag;			    /*  主站在线标志          		*/

//extern    uint8_t         DataNo;      
	
typedef struct{
	uint8_t  head[2];     //固定标识 0xfa 0xf5  
	uint16_t len;         //长度
	uint8_t  encrypt;     //加密信息位
	uint8_t  sn;          //序列号
	uint16_t cmd;         //命令
	uint8_t  data[256];   //数据
	uint8_t  cs;          //校验和
}Report;

typedef struct
{
  	uint16_t  	ParaFlag ;
	  uint16_t		SoftVer ;				 	  	/*终端软件版本号*/
	  uint16_t		HardwareVer ;					/*终端硬件版本号*/
	  uint8_t		  TermNo[32];						/*终端机器编号*/
		uint8_t		  TermType; 						/*充电桩类型*/	
		uint16_t		ItemType; 				  	/*项目类型*/
	  uint32_t		StartTimes; 				  /*启动次数*/
	  uint8_t		  UploadMode ;					/*上传模式*/	
	  uint16_t		CheckInTime;			  	/*签到间隔时间*/
	  uint8_t     InternalVariable;     /*内部变量*/
	  uint8_t     ChargePort;           /*充电枪数量*/
	  uint8_t     HeartRate;            /*心跳周期*/
	  uint8_t     HeartRateTimeOut;     /*心跳周期*/	
	  uint8_t      SicFlag;              /*加密标志*/
	  uint32_t     ReportNo;             /*充电记录数量*/
	
	  uint8_t		  SocketType ;					/*充电桩通信模式*/	
    uint8_t     ServerIP[4];   	    	/*服务器IP*/ 
    uint16_t   	ServerPort ;					/*服务器端口*/
	  uint8_t     GateWay[4] ;          /*网关*/
    uint8_t		   IP[4] ;						  /*自身IP*/
    uint16_t   	 Port ;						    /*自身端口*/
    uint8_t 	   MASK[4];           	/*子网掩码*/
    uint8_t 		 MAC[6];            	/*物理地址*/
    uint16_t   	RatedVoltage ;				/*额定电压*/
    uint16_t   	RatedCurrent ;				/*额定电流*/
    uint8_t   	JudgeTime ;						/*过载、过压、欠压判断时间*/
  	uint8_t		  HeartBeattimes;				/*心跳周期 单位 min*/
	  uint8_t		  ComTimeOut ;					/*通讯超时时间*/
	  uint8_t		  Repeat ;				    	/*重发次数 */
	  uint8_t		  DutyCycle;						/*占空比*/
	  uint8_t		  FeiLv[2];							/*费率*/
		uint8_t     RegisterState;        /*签到状态*/
		uint8_t			AutoUpload;           /*自动上传*/       	
	  uint8_t	  	CS ;
}WELINK_PARA;



extern  WELINK_PARA   Welink_PARA;



void UpstreamCmdHandle(uint16_t cmd,uint8_t port);
extern void WelinkMain(void );
extern void RecvDataHandle( void );



#endif

