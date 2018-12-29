#ifndef __METER_H_
#define __METER_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <uart.h>
#include <bms.h>
#include <calculator.h>
#include <ucos_ii.h>

#define READ_CTLW			       0x11
#define WRITE_TIME_CTLW		   0x08
#define READCTLW			0x11
#define READCTLW			0x11

#define METDATLEN		12		//电表数据长度

typedef struct {

	uint8_t 	MeterNO[6];       //电表编号
	uint8_t	  DI[4];					  //数据标识
	uint8_t	  Slen;						  //发送数据长度	
	uint8_t	  Rlen;						  //接收数据长度
	uint8_t	  Data[METDATLEN];	//数据内容
}sMeter;

extern  sMeter SMeter;				//电表参数用于发送
extern  sMeter RMeter;				//电表参数用于接收

extern  void MeterMain(void);

#endif
