#ifndef __ETHERNET_H_
#define __ETHERNET_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <ucos_ii.h>
#include "gpio.h"
#include <includes.h>
#include <welink.h>

#ifdef   IN_W5500_GLOBALS_
#define  IN_W5500_EXT
#else
#define  IN_W5500_EXT  extern
#endif

/***************** Common Register *****************/
#define MR		0x0000
#define RST		0x80
#define WOL		0x20
#define PB		0x10
#define PPP		0x08
#define FARP	0x02

#define GAR		0x0001
#define SUBR	0x0005
#define SHAR	0x0009
#define SIPR	0x000f

/********************* Socket Register *******************/
#define Sn_MR		0x0000
#define MULTI_MFEN		0x80
#define BCASTB			0x40
#define	ND_MC_MMB		0x20
#define UCASTB_MIP6B	0x10
#define MR_CLOSE		0x00
#define MR_TCP		0x01
#define MR_UDP		0x02
#define MR_MACRAW		0x04

#define Sn_CR		0x0001
#define OPEN		0x01
#define LISTEN		0x02
#define CONNECT		0x04
#define DISCON		0x08
#define CLOSE		0x10
#define SEND		0x20
#define SEND_MAC	0x21
#define SEND_KEEP	0x22
#define RECV		0x40

#define Sn_IR		0x0002
#define IR_SEND_OK		0x10
#define IR_TIMEOUT		0x08
#define IR_RECV			0x04
#define IR_DISCON		0x02
#define IR_CON			0x01
	
#define Sn_SR		          0x0003	
#define SOCK_CLOSED     	0x00
#define SOCK_INIT		      0x13
#define SOCK_LISTEN 	    0x14
#define SOCK_ESTABLISHED	0x17
#define SOCK_CLOSE_WAIT 	0x1c
#define SOCK_UDP		      0x22
#define SOCK_MACRAW     	0x02

#define SOCK_SYNSEND	  0x15
#define SOCK_SYNRECV	  0x16
#define SOCK_FIN_WAI	  0x18
#define SOCK_CLOSING	  0x1a
#define SOCK_TIME_WAIT	0x1b
#define SOCK_LAST_ACK	  0x1d

#define Sn_PORT		    0x0004
#define Sn_DHAR	     	0x0006
#define Sn_DIPR		    0x000c
#define Sn_DPORTR	    0x0010
	
#define Sn_MSSR		    0x0012
#define Sn_TOS		    0x0015
#define Sn_TTL	    	0x0016
	
#define Sn_RXBUF_SIZE	0x001e
#define Sn_TXBUF_SIZE	0x001f
#define Sn_TX_FSR	    0x0020
#define Sn_TX_RD	    0x0022
#define Sn_TX_WR	    0x0024
#define Sn_RX_RSR   	0x0026
#define Sn_RX_RD	    0x0028
#define Sn_RX_WR	    0x002a


#define PHYCFGR   0x002e
#define RST_PHY		0x80
#define OPMODE		0x40
#define DPX			  0x04
#define SPD			  0x02
#define LINK		  0x01
	
#define  S0_TTL		0x0016

#define Sn_RXBUF_SIZE	0x001e
#define Sn_TXBUF_SIZE	0x001f
#define Sn_TX_FSR	    0x0020
#define Sn_TX_RD	    0x0022
#define Sn_TX_WR	    0x0024
#define Sn_RX_RSR	    0x0026
#define Sn_RX_RD	    0x0028
#define Sn_RX_WR	    0x002a


/* Operation mode bits */
#define VDM		0x00
#define FDM1	0x01
#define	FDM2	0x02
#define FDM4	0x03

/* Read_Write control bit */
#define RWB_READ	0x00
#define RWB_WRITE	0x04

/* Block select bits */
#define COMMON_R	0x00


#define S_RX_SIZE	2048			/*定义Socket接收缓冲区的大小，可以根据W5100_RMSR的设置修改 */
#define S_TX_SIZE	2048  			/*定义Socket发送缓冲区的大小，可以根据W5100_TMSR的设置修改 */

//W5100 Register definition
#define	COMMON_BASE	0x0000			//Base address，根据需要修改

//-----------------------------------------------------------------------------
//                               Common register
//-----------------------------------------------------------------------------
//Mode register, R/W, Default=0x00
#define W5500_MODE	COMMON_BASE
#define MODE_RST	0x80				//Software reset
#define MODE_MT		0x20				//Memory test
#define MODE_PB		0x10				//Ping block mode
#define MODE_PPPOE	0x08				//PPOE mode
#define MODE_LB		0x04				//Little_endian/Big_endian ordering in indirect bus I/F
#define MODE_AI		0x02				//Address auto increment in indirect bus I/F
#define MODE_IND	0x01				//Indirect bus I/F mode

//Gateway address register, R/W, default=0x00
#define	W5500_GAR	COMMON_BASE+0x01

//Subnet mask address, R/W, default=0x00
#define	W5500_SUBR	COMMON_BASE+0x05

//Source hardware address, R/W, default=0x00
#define	W5500_SHAR	COMMON_BASE+0x09

//Source IP address, R/W, default=0x00
#define	W5500_SIPR	COMMON_BASE+0x0f

//Interrupt and interrupt mask register
#define	W5500_IR	COMMON_BASE+0x15		//RO, Default=0x00
#define IR_CONFLICT	0x80				//IP conflict
#define IR_UNREACH	0x40				//Destination unreachable
#define IR_PPPOE	0x20				//PPOE close
#define IR_MP		0x10

#define IR_S3_INT	0x08				//Occurrence of socket 3 socket interrupt
#define IR_S2_INT	0x04				//Occurrence of socket 2 socket interrupt
#define IR_S1_INT	0x02				//Occurrence of socket 1 socket interrupt
#define IR_S0_INT	0x01				//Occurrence of socket 0 socket interrupt

#define	W5500_IMR	COMMON_BASE+0x16		//R/W, Default=0x00
#define IMR_CONFLICT	0x80			//IP conflict
#define IMR_UNREACH		0x40			//Destination unreachable
#define IMR_PPPOE		0x20			  //PPOE close
#define IMR_MP			0x10
	
#define IMR_S3_INT		0x08			//Occurrence of socket 3 socket interrupt
#define IMR_S2_INT		0x04			//Occurrence of socket 2 socket interrupt
#define IMR_S1_INT		0x02			//Occurrence of socket 1 socket interrupt
#define IMR_S0_INT		0x01			//Occurrence of socket 0 socket interrupt

#define	W5500_SIR	COMMON_BASE+0x17		//R/W, Default=0x00
#define S7_IR		0x80
#define S6_IR		0x40
#define S5_IR		0x20
#define S4_IR		0x10
#define S3_IR		0x08
#define S2_IR		0x04
#define S1_IR		0x02
#define S0_IR		0x01
	
#define	W5500_SIMR	COMMON_BASE+0x18		//R/W, Default=0x00
#define S7_IMR		0x80
#define S6_IMR		0x40
#define S5_IMR		0x20
#define S4_IMR		0x10
#define S3_IMR		0x08
#define S2_IMR		0x04
#define S1_IMR		0x02
#define S0_IMR		0x01


//Retry time value. Value 1 means 100us, R/W, default=0x07D0
#define W5500_RTR	COMMON_BASE+0x19

//Retry count, R/W, Default=0x08
#define W5500_RCR	COMMON_BASE+0X1b

//RX memory size register, R/W, default=0x55
//--------------------------------------------------------    S1      S0      Memory size
//  Socket3    |  Socket2    |  Socket1    |  Socket0    |    0       0           1KB
//-------------|-------------|-------------|-------------|    0       1           2KB
//S1       S0  |S1       S0  |S1       S0  |S1       S0  |    1       0           4KB
//--------------------------------------------------------    1       1           8KB
#define W5500_RMSR	COMMON_BASE+0x1e

//TX memory size register, R/W, default=0x55
#define W5500_TMSR	COMMON_BASE+0x1f

//Authentication type in PPPOE mode, R, default=0x0000
#define W5500_PATR	COMMON_BASE+0x1c

//PPP LCP request timer register, R/W, default=0x28
//Value 1 is about 25ms
#define W5500_PTIMER	COMMON_BASE+0x28

//PPP LCP magic number register, R/W, default=0x00
#define W5500_PMAGIC	COMMON_BASE+0x29

//Unreachable IP address, RO, default=0x00
#define W5500_UIPR	COMMON_BASE+0x2a

//Unreachable port register, RO, default=0x0000
#define W5500_UPORT	COMMON_BASE+0x2e


//-----------------------------------------------------------------------------
//                                  Socket register
//-----------------------------------------------------------------------------
//Socket mode register, R/W, default=0x00
#define W5500_S0_MR	COMMON_BASE+0x0000		//Socket 0
#define W5500_S1_MR	COMMON_BASE+0x0000		//Socket 1
#define W5500_S2_MR	COMMON_BASE+0x0000		//Socket 2
#define W5500_S3_MR	COMMON_BASE+0x0000		//Socket 3
#define S_MR_MULTI	0x80				//Multcasting
#define S_MR_MC		0x20				//Multcast
											//  P3   P2   P1   P0     Meaning
#define S_MR_CLOSED	0x00				//  0    0    0    0      Closed
#define S_MR_TCP	0x01				//  0    0    0    1      TCP
#define S_MR_UDP	0x02				//  0    0    1    0      UDP
#define S_MR_IPRAW	0x03				//  0    0    1    1      IPRAW
#define S_MR_MACRAW	0x04				//  0    1    0    0      MACRAW
#define S_MR_PPPOE	0x05				//  0    1    0    1      PPPOE

//Socket command register, R/W, default=0x00
#define W5500_S0_CR	COMMON_BASE+0x0001		//Socket 0
#define W5500_S1_CR	COMMON_BASE+0x0001		//Socket 1
#define W5500_S2_CR	COMMON_BASE+0x0001		//Socket 2
#define W5500_S3_CR	COMMON_BASE+0x0001		//Socket 3
#define S_CR_OPEN		0x01			//It is used to initialize the socket
#define S_CR_LISTEN		0x02			//In TCP mode, it waits for a connection request from any remote
											//peer(Client)
#define S_CR_CONNECT	0x04			//In TCP mode, it sends a connection request to remote peer(SERVER)
#define S_CR_DISCON		0x08			//In TCP mode, it sends a connection termination request
#define S_CR_CLOSE		0x10			//Used to close the socket
#define S_CR_SEND		0x20			//It transmit the data as much as the increase size of write pointer
#define S_CR_SEND_MAC	0x21			//In UDP mode, same as SEND
#define S_CR_SEND_KEEP	0x22			//In TCP mode
#define S_CR_RECV		0x40			//Receiving is processed including the value of socket RX read
											//pointer register

	/* Definition for PPPOE */
#define S_CR_PCON		0x23			//Start of ADSL connection
#define S_CR_PDISCON	0x24			//End of ADSL connection
#define S_CR_PCR		0x25			//Send REQ message ineach phase
#define S_CR_PCN		0x26			//Send NAK message in each phase
#define S_CR_PCJ		0x27			//Senf REJECT message in each phase

//Socket interrup register, RO, default=0x00
#define W5500_S0_IR	COMMON_BASE+0x0002		//Socket 0
#define W5500_S1_IR	COMMON_BASE+0x0502		//Socket 1
#define W5500_S2_IR	COMMON_BASE+0x0602		//Socket 2
#define W5500_S3_IR	COMMON_BASE+0x0702		//Socket 3
#define S_IR_SENDOK		0x10			//Send data complete
#define S_IR_TIMEOUT	0x08			//Set if timeout occurs during connection or termination
											//or data transmission
#define S_IR_RECV		0x04			//Set if data is received
#define S_IR_DISCON		0x02			//Set if receiv connection termination request
#define S_IR_CON		0x01			//Set if connetion is established

	/* Definition for PPPOE */
#define S_IR_PRECV		0x80			//Indicate receiving no support option data
#define S_IR_PFAIL		0x40			//Indicate PAP Authentication fail
#define S_IR_PNEXT		0x20			//Go next phase

//Socket status register, RO, default=0x00
#define W5500_S0_SSR	COMMON_BASE+0x0403		//Socket 0
#define W5500_S1_SSR	COMMON_BASE+0x0503		//Socket 1
#define W5500_S2_SSR	COMMON_BASE+0x0603		//Socket 2
#define W5500_S3_SSR	COMMON_BASE+0x0703		//Socket 3
#define S_SSR_CLOSED		0x00			//In case that OPEN command are given to Sn_CR, Timeout interrupt
												//is asserted or connection is terminated
#define S_SSR_INIT			0x13			//In case that Sn_MR is set as TCP and OPEN commands are given to Sn_CR
#define S_SSR_LISTEN		0x14			//In case that under the SOCK_INIT status, LISTEN commands
												//are given to Sn_CR
#define S_SSR_ESTABLISHED	0x17			//In case that connection is established
#define S_SSR_CLOSE_WAIT	0x1c			//In case that connection temination request is received
#define S_SSR_UDP			0x22			//In case that OPEN command is given to Sn_CR when Sn_MR is set as UDP
#define S_SSR_IPRAW			0x32			//In case that OPEN command is given to Sn_CR when Sn_MR is set as IPRAW
#define S_SSR_MACRAW		0x42			//In case that OPEN command is given to S0_CR when S0_MR is set as MACRAW
#define S_SSR_PPPOE			0x5f			//In case that OPEN command is given to S0_CR when S0_MR is set as PPPOE
	//Below is shown in the status change, and does not need much attention
#define S_SSR_SYNSEND		0x15
#define S_SSR_SYNRECV		0x16
#define S_SSR_FIN_WAIT		0x18
#define S_SSR_CLOSING		0x1a
#define S_SSR_TIME_WAIT		0x1b
#define S_SSR_LAST_ACK		0x1d
#define S_SSR_ARP0			0x11
#define S_SSR_ARP1			0x21
#define S_SSR_ARP2			0x31

//Socket Source port register, R/W, default=0x00
#define W5500_S0_PORT	COMMON_BASE+0x0004		//Socket 0
#define W5500_S1_PORT	COMMON_BASE+0x0504		//Socket 1
#define W5500_S2_PORT	COMMON_BASE+0x0604		//Socket 2
#define W5500_S3_PORT	COMMON_BASE+0x0704		//Socket 3

//Socket destination hardware address register, R/W, default=0x00
#define W5500_S0_DHAR	COMMON_BASE+0x0006		//Socket 0
#define W5500_S1_DHAR	COMMON_BASE+0x0506		//Socket 1
#define W5500_S2_DHAR	COMMON_BASE+0x0606		//Socket 2
#define W5500_S3_DHAR	COMMON_BASE+0x0706		//Socket 3

//Socket destination IP address register, R/W, default=0x00
#define W5500_S0_DIPR	COMMON_BASE+0x000c		//Socket 0
#define W5500_S1_DIPR	COMMON_BASE+0x000c		//Socket 1
#define W5500_S2_DIPR	COMMON_BASE+0x000c		//Socket 2
#define W5500_S3_DIPR	COMMON_BASE+0x000c		//Socket 3

//Socket destionation port register, R/W, default=0x00
#define W5500_S0_DPORT	COMMON_BASE+0x0010		//Socket 0
#define W5500_S1_DPORT	COMMON_BASE+0x0510		//Socket 1
#define W5500_S2_DPORT	COMMON_BASE+0x0610		//Socket 2
#define W5500_S3_DPORT	COMMON_BASE+0x0710		//Socket 3

//Socket maximum segment size register, R/W, default=0x00
#define W5500_S0_MSS	COMMON_BASE+0x0012		//Socket 0
#define W5500_S1_MSS	COMMON_BASE+0x0512		//Socket 1
#define W5500_S2_MSS	COMMON_BASE+0x0612		//Socket 2
#define W5500_S3_MSS	COMMON_BASE+0x0712		//Socket 3

//Socket IP protocol register, R/W, default=0x00
//See http://www.iana.org/assignments/protocol-number
#define W5500_S0_PROTO	COMMON_BASE+0x0014		//Socket 0
#define W5500_S1_PROTO	COMMON_BASE+0x0514		//Socket 1
#define W5500_S2_PROTO	COMMON_BASE+0x0614		//Socket 2
#define W5500_S3_PROTO	COMMON_BASE+0x0714		//Socket 3

//Socket IP type of servce register, R/W, default=0x00
#define W5500_S0_TOS	COMMON_BASE+0x0015		//Socket 0
#define W5500_S1_TOS	COMMON_BASE+0x0515		//Socket 1
#define W5500_S2_TOS	COMMON_BASE+0x0615		//Socket 2
#define W5500_S3_TOS	COMMON_BASE+0x0715		//Socket 3

//Socket IP time to live register, R/W, default=0x80
#define W5500_S0_TTL	COMMON_BASE+0x0016		//Socket 0
#define W5500_S1_TTL	COMMON_BASE+0x0516		//Socket 1
#define W5500_S2_TTL	COMMON_BASE+0x0616		//Socket 2
#define W5500_S3_TTL	COMMON_BASE+0x0716		//Socket 3

//Socket TX free size register, RO, default=0x0800
//should read upper byte first and lower byte later
#define W5500_S0_TX_FSR		COMMON_BASE+0x0020		//Socket 0
#define W5500_S1_TX_FSR		COMMON_BASE+0x0520		//Socket 1
#define W5500_S2_TX_FSR		COMMON_BASE+0x0620		//Socket 2
#define W5500_S3_TX_FSR		COMMON_BASE+0x0720		//Socket 3

//Socket TX read pointer register, RO, default=0x0000
//should read upper byte first and lower byte later
#define W5500_S0_TX_RR		COMMON_BASE+0x0022		//Socket 0
#define W5500_S1_TX_RR		COMMON_BASE+0x0522		//Socket 1
#define W5500_S2_TX_RR		COMMON_BASE+0x0622		//Socket 2
#define W5500_S3_TX_RR		COMMON_BASE+0x0722		//Socket 3

//Socket TX write pointer register, R/W, default=0x0000
//should read upper byte first and lower byte later
#define W5500_S0_TX_WR		COMMON_BASE+0x0024		//Socket 0
#define W5500_S1_TX_WR		COMMON_BASE+0x0524		//Socket 1
#define W5500_S2_TX_WR		COMMON_BASE+0x0624		//Socket 2
#define W5500_S3_TX_WR		COMMON_BASE+0x0724		//Socket 3

//Socket RX size register, RO, default=0x0000
//should read upper byte first and lower byte later
#define W5500_S0_RX_RSR		COMMON_BASE+0x0026		//Socket 0
#define W5500_S1_RX_RSR		COMMON_BASE+0x0526		//Socket 1
#define W5500_S2_RX_RSR		COMMON_BASE+0x0626		//Socket 2
#define W5500_S3_RX_RSR		COMMON_BASE+0x0726		//Socket 3

//Socket RX read pointer register, R/W, default=0x0000
//should read upper byte first and lower byte later
#define W5500_S0_RX_RR		COMMON_BASE+0x0028		//Socket 0
#define W5500_S1_RX_RR		COMMON_BASE+0x0528		//Socket 1
#define W5500_S2_RX_RR		COMMON_BASE+0x0628		//Socket 2
#define W5500_S3_RX_RR		COMMON_BASE+0x0728		//Socket 3

//Socket RX read pointer register, R/W, default=0x0000
//should read upper byte first and lower byte later
#define W5500_S0_RX_WR		COMMON_BASE+0x002A		//Socket 0
#define W5500_S1_RX_WR		COMMON_BASE+0x052A		//Socket 1
#define W5500_S2_RX_WR		COMMON_BASE+0x062A		//Socket 2
#define W5500_S3_RX_WR		COMMON_BASE+0x072A		//Socket 3

//TX memory
#define W5500_TX	COMMON_BASE+0x4000

//RX memory
#define W5500_RX	COMMON_BASE+0x6000


//#define TRUE		0xff
//#define FALSE		0x00


typedef unsigned char  SOCKET;

IN_W5500_EXT unsigned char  UDP_DIPR[4];
IN_W5500_EXT unsigned char  UDP_DPORT[2];

																/* 端口数据缓冲区 */
//IN_RS4853_EXT	unsigned char 	__align(4)	ENETRcvBuf[(256 + sizeof(int8) - 1) / sizeof(int8)];
																
 IN_W5500_EXT unsigned char  ENETRcvBuf[512];			/* 端口接收数据缓冲区 */
//IN_W5500_EXT unsigned char  Tx_Buffer[1024];			/* 端口发送数据缓冲区 */
 IN_W5500_EXT uint16_t ENETRcvLen;        /* 定义端口接收数据长度*/
											/* 网络参数寄存器 */
IN_W5500_EXT unsigned char  Gateway_IP[4];	     	/* Gateway IP Address */
IN_W5500_EXT unsigned char  Sub_Mask[4];				/* Subnet Mask */
IN_W5500_EXT unsigned char  Phy_Addr[6];  			/* Physical Address */
IN_W5500_EXT unsigned char  IP_Addr[4];				/* Loacal IP Address */

IN_W5500_EXT unsigned int  S0_Port;   			/* Socket0 Port number */
IN_W5500_EXT unsigned char  S0_DIP[4];				/* Socket0 Destination IP Address */
IN_W5500_EXT unsigned int  S0_DPort;				/* Socket0 Destination Port number */

IN_W5500_EXT unsigned char  S1_Port[2];   			/* Socket1 Port number */
IN_W5500_EXT unsigned char  S1_DIP[4];   			/* Socket1 Destination IP Address */
IN_W5500_EXT unsigned char  S1_DPort[2];				/* Socket1 Destination Port number */

IN_W5500_EXT unsigned char  S2_Port[2];				/* Socket2 Port number */
IN_W5500_EXT unsigned char  S2_DIP[4];				/* Socket2 Destination IP Address */
IN_W5500_EXT unsigned char  S2_DPort[2];				/* Socket2 Destination Port number */

IN_W5500_EXT unsigned char  S3_Port[2];				/* Socket3 Port number */
IN_W5500_EXT unsigned char  S3_DIP[4];				/* Socket3 Destination IP Address */
IN_W5500_EXT unsigned char  S3_DPort[2];				/* Socket3 Destination Port number */

IN_W5500_EXT unsigned char  S0_State;				/* Socket0 state recorder */
IN_W5500_EXT unsigned char  S1_State;				/* Socket1 state recorder */
IN_W5500_EXT unsigned char  S2_State;				/* Socket2 state recorder */
IN_W5500_EXT unsigned char  S3_State;				/* Socket3 state recorder */

#define S_INIT	0x01
#define S_CONN	0x02

IN_W5500_EXT unsigned char  S0_Data;					/* Socket0 receive data and transmit OK */
IN_W5500_EXT unsigned char  S1_Data;					/* Socket1 receive data and transmit OK */
IN_W5500_EXT unsigned char  S2_Data;					/* Socket2 receive data and transmit OK */
IN_W5500_EXT unsigned char  S3_Data;					/* Socket3 receive data and transmit OK*/

#define S_RECEIVE		0x01
#define S_TRANSMITOK	0x02

IN_W5500_EXT unsigned char  S0_Mode;	
IN_W5500_EXT unsigned char  S1_Mode;
IN_W5500_EXT unsigned char  S2_Mode;
IN_W5500_EXT unsigned char  S3_Mode;

#define TCP_SERVER		0x00		/* TCP服务器模式 */
#define TCP_CLIENT		0x01		/* TCP客户端模式 */
#define UDP_MODE	   	0x02		/* UDP模式 */

IN_W5500_EXT unsigned char  W5100_Interrupt;



extern void SysLinkParaInit(void);
extern  void W5500SocketRXD(SOCKET s);
extern void W5500_Initialization(void);
extern void W5500SocketRXD(SOCKET s);
extern unsigned int ReadSocketDataToBuffer(SOCKET s, unsigned char *dat_ptr);
extern void WriteSocketDataToBuffer(SOCKET s, unsigned char *dat_ptr, unsigned int size);


#endif
