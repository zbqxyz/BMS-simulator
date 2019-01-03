#ifndef __DISPLAY_H_
#define __DISPLAY_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <ucos_ii.h>
#include <rtc.h>
#include <uart.h>
#include <meter.h>
#include <calculator.h>
#include <fram.h>
//Dwin屏的变量地址定义

#define DW_ADDR_PAGE0        0x00
#define DW_ADDR_PAGE1        0x01
#define DW_ADDR_PAGE2        0x02
#define DW_ADDR_PAGE3        0x03
#define DW_ADDR_PAGE4        0x04
#define DW_ADDR_PAGE5        0x05
#define DW_ADDR_PAGE6        0x06
#define DW_ADDR_PAGE7        0x07
#define DW_ADDR_PAGE8        0x08
#define DW_ADDR_PAGE9        0x09
#define DW_ADDR_PAGE10       0x0a
#define DW_ADDR_PAGE11       0x0b
#define DW_ADDR_PAGE12       0x0c
#define DW_ADDR_PAGE13       0x0d
#define DW_ADDR_PAGE14       0x0e
#define DW_ADDR_PAGE15       0x0f


#define DW_ADDR_CHARGE_VOLTAGE            0x201   
#define DW_ADDR_CHARGE_CURRENT            0x202
#define DW_ADDR_REQUEST_VOLTAGE           0x203
#define DW_ADDR_REQUEST_CURRENT           0x204
#define DW_ADDR_BATTERY_VOLTAGE           0x310   
#define DW_ADDR_BATTERY_SOC               0x330     
#define DW_ADDR_SINGLE_VOLTAGE            0x340
#define DW_ADDR_SINGLE_TEMPERATURE        0x350


#define DW_ADDR_REQUEST_VOLTAGE_INPUT_A   0x400
#define DW_ADDR_REQUEST_CURRENT_INPUT_A   0x410
#define DW_ADDR_SOC_INPUT                 0x420
#define DW_ADDR_BATTERY_VOLTAGE_INPUT     0x430
#define DW_ADDR_CHARGE_MODE_INPUT         0x440
#define DW_ADDR_BATTERY_TYPE_INPUT        0x450
#define DW_ADDR_SINGLE_VOLTAGE_INPUT      0x460
#define DW_ADDR_SINGLE_TEMPERATURE_INPUT  0x470
#define DW_ADDR_RESET_INPUT               0x480
#define DW_ADDR_STOP_INPUT                0x490

#define DW_ADDR_Auxiliary_POWER           0x500
#define DW_ADDR_CHARGE_MODE               0x510
#define DW_ADDR_BATTERY_TYPE              0x520

#define DW_ADDR_MAX_VOLTAGE_INPUT           0x530
#define DW_ADDR_MAX_CURRENT_INPUT           0x540

#define DW_ADDR_TimeChargeModeStart_INPUT          0x550
#define DW_ADDR_TimeChargeModeStop_INPUT           0x560

#define DW_ADDR_SOCChargeModeStart_INPUT           0x570
#define DW_ADDR_SOCChargeModeStop_INPUT            0x580

#define DW_ADDR_RatedBatteryVolume_INPUT    0x600
#define DW_ADDR_RatedBatteryVoltage_INPUT   0x610

#define DW_ADDR_MAXSingleVoltage_INPUT      0x620
#define DW_ADDR_MINSingleVoltage_INPUT      0x630

#define DW_ADDR_MAXSingleVoltageTemp_INPUT  0x640
#define DW_ADDR_MINSingleVoltageTemp_INPUT  0x650

#define DW_ADDR_BMSSoftVer_INPUT            0x660
#define DW_ADDR_MeterNO_INPUT               0x670

#define DW_ADDR_ChargeLimitedTime_INPUT     0x680
#define DW_ADDR_SOCIncreaseTime_INPUT       0x690

#define DW_ADDR_VIN_INPUT                   0x700

#define DW_ADDR_MAX_VOLTAGE_DISPLAY           0x800
#define DW_ADDR_MAX_CURRENT_DISPLAY           0x810

#define DW_ADDR_RatedBatteryVolume_DISPLAYT   0x820
#define DW_ADDR_RatedBatteryVoltage_DISPLAY   0x830

#define DW_ADDR_MAXSingleVoltage_DISPLAY      0x840
#define DW_ADDR_MINSingleVoltage_DISPLAY      0x850

#define DW_ADDR_MAXSingleVoltageTemp_DISPLAY  0x860
#define DW_ADDR_MINSingleVoltageTemp_DISPLAY  0x870

#define DW_ADDR_BMSSoftVer_DISPLAY            0x880
#define DW_ADDR_MeterNO_DISPLAY               0x890

#define DW_ADDR_ChargeLimitedTime_DISPLAY     0x900
#define DW_ADDR_SOCIncreaseTime_DISPLAY       0x910

#define DW_ADDR_VIN_DISPLAY                   0x920


typedef struct _DwinMsg
{                                                                         
    uint8_t Head1;
	  uint8_t Head2;
    uint8_t Lenth;
  	uint8_t Order;
    uint16_t Addr;
	  uint8_t Datalen;
	  uint8_t Data[30];
  	uint8_t Value;
	
}DwinMsg;

extern DwinMsg RDwinMessage;
extern DwinMsg WDwinMessage;

typedef struct _DisplayMsg
{
                                                                         
    unsigned int PageFlag;                                               
    unsigned int VariableValueFlag;
	
}DisplayMsg;

extern DisplayMsg DisplayMessage;
extern	void DisplayMain (void);


#endif
