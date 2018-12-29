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
*    FRAM命令操作码
*******************************************/
#define  CMD_FRAM_WR_ENABLE				0x06     //设置写使能锁存器
#define  CMD_FRAM_WR_DISABLE			0x04     //写禁止
#define  CMD_FRAM_RD_STATUS				0x05     //读状态寄存器
#define  CMD_FRAM_WR_STATUS				0x01     //写状态寄存器
#define  CMD_FRAM_RD_MEMORY				0x03     //读存储器数据
#define  CMD_FRAM_WR_MEMORY				0x02     //写存储器数据

#define 	BMS_MSG_PARA_ADDR 		0x0000 		 //BMS信息参数存储地址
#define 	WELINK_PARA_ADDR 	  	0x1024 		 //以太网信息参数存储地址
#define 	SYS_PARA_ADDR 	  	  0x2048 		 //系统参数存储地址

extern void	FramRead(uint8_t *dataptr,uint16_t addr,uint16_t length);
extern void FramWrite(uint8_t *dataptr,uint16_t addr,uint16_t length);

#endif
