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

#define METDATLEN		12		//������ݳ���

typedef struct {

	uint8_t 	MeterNO[6];       //�����
	uint8_t	  DI[4];					  //���ݱ�ʶ
	uint8_t	  Slen;						  //�������ݳ���	
	uint8_t	  Rlen;						  //�������ݳ���
	uint8_t	  Data[METDATLEN];	//��������
}sMeter;

extern  sMeter SMeter;				//���������ڷ���
extern  sMeter RMeter;				//���������ڽ���

extern  void MeterMain(void);

#endif
