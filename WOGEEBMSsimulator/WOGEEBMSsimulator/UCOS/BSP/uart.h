#ifndef __UART_H_
#define __UART_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <string.h>
#include <gpio.h>
#include <ucos_ii.h>

#define 	UART0_BPS  			115200 		                /* ����ͨ�Ų�����               */
#define 	UART1_BPS      	115200 								  		/* ����ͨ�Ų�����               */
#define 	UART2_BPS  			2400 		                /* ����ͨ�Ų�����               */
#define 	UART3_BPS      	115200 									 	/* ����ͨ�Ų�����               */
#define 	UART4_BPS  			9600		                  /* ����ͨ�Ų�����               */

#define		UARTBUFLEN			1024                     	/* �����շ����ݻ���������       */


#define IER_RBR     0x01
#define IER_THRE    0x02
#define IER_RLS     0x04

#define IIR_PEND    0x01
#define IIR_RLS     0x03
#define IIR_RDA     0x02
#define IIR_CTI     0x06
#define IIR_THRE    0x01

#define LSR_RDR     0x01
#define LSR_OE      0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE    0x20
#define LSR_TEMT    0x40
#define LSR_RXFE    0x80


extern uint8_t    RcvBufU0[UARTBUFLEN];                                       /*  ���ڽ������ݻ�����          */
extern uint8_t    SndBufU0[UARTBUFLEN];                                       /*  ���ڷ������ݻ�����          */
extern uint32_t   GulNumU0;                                             	  	/*  ���ڽ������ݵĸ���          */
extern uint32_t   RcvLenU0;                                                   /*  ���ڽ������ݵĳ���          */
extern uint32_t   SndLenU0;                                                   /*  ���ڽ������ݵĳ���          */
extern uint8_t    RcvNewU0;                                                   /*  ���ڽ��������ݵı�־        */
extern void UART0Init (void);
extern void UART0SendByte (uint8_t ucDat);
extern void UART0SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART0RxbufClear(void );


extern uint8_t    RcvBufU1[UARTBUFLEN];                                       /*  ���ڽ������ݻ�����          */
extern uint8_t    SndBufU1[UARTBUFLEN];                                       /*  ���ڷ������ݻ�����          */
extern uint32_t   GulNumU1;                                             	  	/*  ���ڽ������ݵĸ���          */
extern uint32_t   RcvLenU1;                                                   /*  ���ڽ������ݵĳ���          */
extern uint32_t   SndLenU1;                                                   /*  ���ڽ������ݵĳ���          */
extern uint8_t    RcvNewU1;                                                   /*  ���ڽ��������ݵı�־        */
extern void UART1Init (void);
extern void UART1SendByte (uint8_t ucDat);
extern void UART1SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART1RxbufClear(void );


extern uint8_t    RcvBufU2[UARTBUFLEN];                                       /*  ���ڽ������ݻ�����          */
extern uint8_t    SndBufU2[UARTBUFLEN];                                       /*  ���ڷ������ݻ�����          */
extern uint32_t   GulNumU2;                                             	  	/*  ���ڽ������ݵĸ���          */
extern uint32_t   RcvLenU2;                                                   /*  ���ڽ������ݵĳ���          */
extern uint32_t   SndLenU2;                                                   /*  ���ڽ������ݵĳ���          */
extern uint8_t    RcvNewU2;                                                   /*  ���ڽ��������ݵı�־        */
extern void UART2Init (void);
extern void UART2SendByte (uint8_t ucDat);
extern void UART2SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART2RxbufClear(void );


extern uint8_t    RcvBufU3[UARTBUFLEN];                                       /*  ���ڽ������ݻ�����          */
extern uint8_t    SndBufU3[UARTBUFLEN];                                       /*  ���ڷ������ݻ�����          */
extern uint32_t   GulNumU3;                                             	  	/*  ���ڽ������ݵĸ���          */
extern uint32_t   RcvLenU3;                                                   /*  ���ڽ������ݵĳ���          */
extern uint32_t   SndLenU3;                                                   /*  ���ڽ������ݵĳ���          */
extern uint8_t    RcvNewU3;                                                   /*  ���ڽ��������ݵı�־        */
extern void UART3Init (void);
extern void UART3SendByte (uint8_t ucDat);
extern void UART3SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART3RxbufClear(void );


extern uint8_t    RcvBufU4[UARTBUFLEN];                                       /*  ���ڽ������ݻ�����          */
extern uint8_t    SndBufU4[UARTBUFLEN];                                       /*  ���ڷ������ݻ�����          */
extern uint32_t   GulNumU4;                                             	  	/*  ���ڽ������ݵĸ���          */
extern uint32_t   RcvLenU4;                                                   /*  ���ڽ������ݵĳ���          */
extern uint32_t   SndLenU4;                                                   /*  ���ڽ������ݵĳ���          */
extern uint8_t    RcvNewU4;                                                   /*  ���ڽ��������ݵı�־        */
extern void UART4Init (void);
extern void UART4SendByte (uint8_t ucDat);
extern void UART4SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART4RxbufClear(void );

                                      		  


#endif
