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
                                               
    unsigned int ChargeStage;       // ���׶�
    uint16_t  RatedBatteryVolume;   // ��������
    uint16_t  RatedBatteryVoltage;  // ���ص�ѹ		
	  uint8_t   SoftVersion;          // �汾��
	  uint8_t   BatteryType;          // �������
  	uint8_t   ChargeMode;           // ���ģʽ
	  uint8_t   SOC;                  // SOC
    uint16_t  MAXVoltage;           // ��������ѹ
	  uint16_t  MAXCurrent;	          // ����������
	  uint16_t  BatteryVoltage;       // ��ص�ѹ
    uint16_t  RequestVoltage;       // �����ѹ
	  uint16_t  RequestCurrent;       // �������
	  uint16_t  MAXSingleVoltage;	    // ����������ѹ
	  uint16_t  MINSingleVoltage;		  // ����������ѹ
    uint16_t  SingleVoltage;        // �����ѹ
	  uint8_t   MAXSingleVoltageTemp; // ����������¶�
	  uint8_t   MINSingleVoltageTemp;	// ����������¶�
	  uint8_t   SingleVoltageTemp;    // �����¶�
    uint8_t   BMSStopFlag;          // ͣ����־
		uint8_t   VIN[9];		          // ����ʶ����
	  uint8_t   CS;                   // У��λ
	
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

