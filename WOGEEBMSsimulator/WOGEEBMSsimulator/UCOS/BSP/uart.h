#ifndef __UART_H_
#define __UART_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <string.h>
#include <gpio.h>
#include <ucos_ii.h>

#define 	UART0_BPS  			115200 		                /* 串口通信波特率               */
#define 	UART1_BPS      	115200 								  		/* 串口通信波特率               */
#define 	UART2_BPS  			2400 		                /* 串口通信波特率               */
#define 	UART3_BPS      	115200 									 	/* 串口通信波特率               */
#define 	UART4_BPS  			9600		                  /* 串口通信波特率               */

#define		UARTBUFLEN			1024                     	/* 串口收发数据缓冲区长度       */


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


extern uint8_t    RcvBufU0[UARTBUFLEN];                                       /*  串口接收数据缓冲区          */
extern uint8_t    SndBufU0[UARTBUFLEN];                                       /*  串口发送数据缓冲区          */
extern uint32_t   GulNumU0;                                             	  	/*  串口接收数据的个数          */
extern uint32_t   RcvLenU0;                                                   /*  串口接收数据的长度          */
extern uint32_t   SndLenU0;                                                   /*  串口接收数据的长度          */
extern uint8_t    RcvNewU0;                                                   /*  串口接收新数据的标志        */
extern void UART0Init (void);
extern void UART0SendByte (uint8_t ucDat);
extern void UART0SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART0RxbufClear(void );


extern uint8_t    RcvBufU1[UARTBUFLEN];                                       /*  串口接收数据缓冲区          */
extern uint8_t    SndBufU1[UARTBUFLEN];                                       /*  串口发送数据缓冲区          */
extern uint32_t   GulNumU1;                                             	  	/*  串口接收数据的个数          */
extern uint32_t   RcvLenU1;                                                   /*  串口接收数据的长度          */
extern uint32_t   SndLenU1;                                                   /*  串口接收数据的长度          */
extern uint8_t    RcvNewU1;                                                   /*  串口接收新数据的标志        */
extern void UART1Init (void);
extern void UART1SendByte (uint8_t ucDat);
extern void UART1SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART1RxbufClear(void );


extern uint8_t    RcvBufU2[UARTBUFLEN];                                       /*  串口接收数据缓冲区          */
extern uint8_t    SndBufU2[UARTBUFLEN];                                       /*  串口发送数据缓冲区          */
extern uint32_t   GulNumU2;                                             	  	/*  串口接收数据的个数          */
extern uint32_t   RcvLenU2;                                                   /*  串口接收数据的长度          */
extern uint32_t   SndLenU2;                                                   /*  串口接收数据的长度          */
extern uint8_t    RcvNewU2;                                                   /*  串口接收新数据的标志        */
extern void UART2Init (void);
extern void UART2SendByte (uint8_t ucDat);
extern void UART2SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART2RxbufClear(void );


extern uint8_t    RcvBufU3[UARTBUFLEN];                                       /*  串口接收数据缓冲区          */
extern uint8_t    SndBufU3[UARTBUFLEN];                                       /*  串口发送数据缓冲区          */
extern uint32_t   GulNumU3;                                             	  	/*  串口接收数据的个数          */
extern uint32_t   RcvLenU3;                                                   /*  串口接收数据的长度          */
extern uint32_t   SndLenU3;                                                   /*  串口接收数据的长度          */
extern uint8_t    RcvNewU3;                                                   /*  串口接收新数据的标志        */
extern void UART3Init (void);
extern void UART3SendByte (uint8_t ucDat);
extern void UART3SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART3RxbufClear(void );


extern uint8_t    RcvBufU4[UARTBUFLEN];                                       /*  串口接收数据缓冲区          */
extern uint8_t    SndBufU4[UARTBUFLEN];                                       /*  串口发送数据缓冲区          */
extern uint32_t   GulNumU4;                                             	  	/*  串口接收数据的个数          */
extern uint32_t   RcvLenU4;                                                   /*  串口接收数据的长度          */
extern uint32_t   SndLenU4;                                                   /*  串口接收数据的长度          */
extern uint8_t    RcvNewU4;                                                   /*  串口接收新数据的标志        */
extern void UART4Init (void);
extern void UART4SendByte (uint8_t ucDat);
extern void UART4SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void UART4RxbufClear(void );

                                      		  


#endif
