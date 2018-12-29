#ifndef __FRAM_H_
#define __FRAM_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <ucos_ii.h>
#include <ssp.h>
#include <gpio.h>
#include <print.h>
/******************************************
*    FRAM���������
*******************************************/
#define  CMD_FRAM_WR_ENABLE				0x06     //����дʹ��������
#define  CMD_FRAM_WR_DISABLE			0x04     //д��ֹ
#define  CMD_FRAM_RD_STATUS				0x05     //��״̬�Ĵ���
#define  CMD_FRAM_WR_STATUS				0x01     //д״̬�Ĵ���
#define  CMD_FRAM_RD_MEMORY				0x03     //���洢������
#define  CMD_FRAM_WR_MEMORY				0x02     //д�洢������

#define 	BMS_MSG_PARA_ADDR 		0x0000 		 //BMS��Ϣ�����洢��ַ
#define 	WELINK_PARA_ADDR 	  	0x1024 		 //��̫����Ϣ�����洢��ַ
#define 	SYS_PARA_ADDR 	  	  0x2048 		 //ϵͳ�����洢��ַ

extern void	FramRead(uint8_t *dataptr,uint16_t addr,uint16_t length);
extern void FramWrite(uint8_t *dataptr,uint16_t addr,uint16_t length);

#endif
