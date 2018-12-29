#ifndef __BMS_H_
#define __BMS_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <ucos_ii.h>
#include "main.h"
#include  <can.h>
#include <gpio.h>
#include <print.h>
#include <main.h>
#include "calculator.h"
#include "fram.h"
#include "rtc.h"
typedef struct _MessageBMS
{                                                                      
                                               
    unsigned int ChargeStage;       // 充电阶段
    uint16_t  RatedBatteryVolume;   // 额定电池容量
    uint16_t  RatedBatteryVoltage;  // 额定电池电压		
	  uint8_t   SoftVersion;          // 版本号
	  uint8_t   BatteryType;          // 电池类型
  	uint8_t   ChargeMode;           // 充电模式
	  uint8_t   SOC;                  // SOC
    uint16_t  MAXVoltage;           // 最高允许电压
	  uint16_t  MAXCurrent;	          // 最高允许电流
	  uint16_t  BatteryVoltage;       // 电池电压
    uint16_t  RequestVoltage;       // 需求电压
	  uint16_t  RequestCurrent;       // 需求电流
	  uint16_t  MAXSingleVoltage;	    // 最高允许单体电压
	  uint16_t  MINSingleVoltage;		  // 最低允许单体电压
    uint16_t  SingleVoltage;        // 单体电压
	  uint8_t   MAXSingleVoltageTemp; // 最高允许单体温度
	  uint8_t   MINSingleVoltageTemp;	// 最低允许单体温度
	  uint8_t   SingleVoltageTemp;    // 单体温度
    uint8_t   BMSStopFlag;          // 停机标志
		uint8_t   VIN[9];		          // 车辆识别码
	  uint8_t   CS;                   // 校验位
	
}BMSMsg;


extern BMSMsg BMSMessage;

typedef struct _MessageCharger
{
     uint16_t  ChargeVoltage;                                         
     uint16_t  ChargeCurrent; 
	   uint32_t  ChargerEnergy;
	   
}ChargeMsg;
extern ChargeMsg ChargerMsg;

extern void BMSParameterInit(void);
extern void BMSMain(void);
extern void CHM_Analyse(void);
extern void CRM_Analyse(void);
extern void CML_Analyse(void);
extern void CRO_Analyse(void);
extern void CCS_Analyse(void);
extern void CST_Analyse(void);
extern void CSD_Analyse(void);
extern void CEM_Analyse(void);
extern void BMS_DATA_Packet_Analyse(void);

#endif

