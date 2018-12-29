#ifndef __GPIO_H_
#define __GPIO_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"

#define BEEP        	  (1ul << 12)                             	 			                     
#define BEEP_ON()   		LPC_GPIO2->DIR |= BEEP; LPC_GPIO2->SET |= BEEP     				 
#define BEEP_OFF()   	  LPC_GPIO2->DIR |= BEEP; LPC_GPIO2->CLR |= BEEP 

#define SYSLED        	(1ul << 12)                             	 			                     
#define SYSLED_ON()   	LPC_GPIO0->DIR |= SYSLED; LPC_GPIO0->SET |= SYSLED     				 
#define SYSLED_OFF()   	LPC_GPIO0->DIR |= SYSLED; LPC_GPIO0->CLR |= SYSLED

#define DC_SWITCH        	(1ul << 0)                             	 			                     
#define DC_SWITCH_ON()   	LPC_GPIO0->DIR |= DC_SWITCH; LPC_GPIO0->SET |= DC_SWITCH     				 
#define DC_SWITCH_OFF()   LPC_GPIO0->DIR |= DC_SWITCH; LPC_GPIO0->CLR |= DC_SWITCH

#define Battery_SWITCH        	(1ul << 28)                             	 			                     
#define Battery_SWITCH_ON()   	LPC_GPIO1->DIR |= Battery_SWITCH; LPC_GPIO1->SET |= Battery_SWITCH     				 
#define Battery_SWITCH_OFF()    LPC_GPIO1->DIR |= Battery_SWITCH; LPC_GPIO1->CLR |= Battery_SWITCH

#define FAN_SWITCH        	(1ul << 1)                             	 			                     
#define FAN_SWITCH_ON()   	LPC_GPIO0->DIR |= FAN_SWITCH; LPC_GPIO0->SET |= FAN_SWITCH     				 
#define FAN_SWITCH_OFF()   LPC_GPIO0->DIR |= FAN_SWITCH; LPC_GPIO0->CLR |= FAN_SWITCH


#define GreenLED        	(1ul << 25)                             	 			                     
#define GreenLED_ON()   	LPC_GPIO4->DIR |= GreenLED; LPC_GPIO4->SET |= GreenLED     				 
#define GreenLED_OFF()   	LPC_GPIO4->DIR |= GreenLED; LPC_GPIO4->CLR |= GreenLED

#define YellowLED        	(1ul << 15)                             	 			                     
#define YellowLED_ON()   	LPC_GPIO4->DIR |= YellowLED; LPC_GPIO4->SET |= YellowLED     				 
#define YellowLED_OFF()   LPC_GPIO4->DIR |= YellowLED; LPC_GPIO4->CLR |= YellowLED

#define RedLED        	(1ul << 28)                             	 			                     
#define RedLED_ON()   	LPC_GPIO4->DIR |= RedLED; LPC_GPIO4->SET |= RedLED     				 
#define RedLED_OFF()   	LPC_GPIO4->DIR |= RedLED; LPC_GPIO4->CLR |= RedLED

#define CSLED        	  (1ul << 29)                             	 			                     
#define CSLED_ON()   	  LPC_GPIO4->DIR |= CSLED; LPC_GPIO4->SET |= CSLED     				 
#define CSLED_OFF()   	LPC_GPIO4->DIR |= CSLED; LPC_GPIO4->CLR |= CSLED

#define FRAM_CS        	  (1ul << 16)                             	 			                     
#define FRAM_CS_H()   	 LPC_GPIO0->DIR |= FRAM_CS; LPC_GPIO0->SET |= FRAM_CS     				 
#define FRAM_CS_L()   	 LPC_GPIO0->DIR |= FRAM_CS; LPC_GPIO0->CLR |= FRAM_CS

#define 	CSNOFLASH		(1 << 26)	              			                
#define 	SET_CSNOFLASH()	LPC_GPIO1->DIR |= CSNOFLASH; LPC_GPIO1->SET |= CSNOFLASH	                            
#define 	CLR_CSNOFLASH()	LPC_GPIO1->DIR |= CSNOFLASH; LPC_GPIO1->CLR |= CSNOFLASH	


#define USBLED        	(1ul << 13)                             	 			                     
#define USBLED_ON()   	LPC_GPIO0->DIR |= USBLED; LPC_GPIO0->SET |= USBLED     				 
#define USBLED_OFF()   	LPC_GPIO0->DIR |= USBLED; LPC_GPIO0->CLR |= USBLED

#define WDT        	    (1ul << 27)                             	 			                     
#define WDT_ON()   	  	LPC_GPIO0->DIR |= WDT; LPC_GPIO0->SET |= WDT    				 
#define WDT_OFF()   	  LPC_GPIO0->DIR |= WDT; LPC_GPIO0->CLR |= WDT

#define EN485_U1        	    (1ul << 3) 
#define ENRD485_U1()   	LPC_GPIO4->DIR |= EN485_U1; LPC_GPIO4->SET |= EN485_U1            
#define ENWR485_U1() 		LPC_GPIO4->DIR |= EN485_U1; LPC_GPIO4->CLR |= EN485_U1   
 
#define NET_CS        	  (1ul << 8)                             	 			                     
#define NET_CS_H()   	   LPC_GPIO1->DIR |= NET_CS; LPC_GPIO1->SET |= NET_CS     				 
#define NET_CS_L()   	   LPC_GPIO1->DIR |= NET_CS; LPC_GPIO1->CLR |= NET_CS


#define Welink_Power        	    (1ul << 26) 
#define Welink_PowerON()    	LPC_GPIO3->DIR |= Welink_Power; LPC_GPIO3->SET |= Welink_Power            
#define Welink_PowerOFF() 		LPC_GPIO3->DIR |= Welink_Power; LPC_GPIO3->CLR |= Welink_Power 

#define Welink_Reset        	    (1ul << 0) 
#define Welink_ResetON()    	LPC_GPIO5->DIR |= Welink_Reset; LPC_GPIO5->SET |= Welink_Reset            
#define Welink_ResetOFF() 		LPC_GPIO5->DIR |= Welink_Reset; LPC_GPIO5->CLR |= Welink_Reset 

#define W5500_Reset        	    (1ul << 10)                          //低电平复位有效
#define W5500_ResetON()   	  LPC_GPIO4->DIR |= W5500_Reset; LPC_GPIO4->SET |= W5500_Reset            
#define W5500_ResetOFF() 		  LPC_GPIO4->DIR |= W5500_Reset; LPC_GPIO4->CLR |= W5500_Reset

#define W5500_INT       	      (1ul << 6) 
#define W5500_INT_Init()   	  (LPC_GPIO2->DIR &= ~W5500_INT)         //初始化W5500中断端口    
#define W5500_INT_Status() 	  (!(LPC_GPIO2->PIN & W5500_INT))	       //检测W5500输入端口状态    

 
#define INswitch_INT       	      (1ul << 6) 
#define INswitch_INT_Init()   	  (LPC_GPIO4->DIR &= ~INswitch_INT)         //初始化拨码开关    
#define INswitch_INT_Status() 	  (!(LPC_GPIO4->PIN & INswitch_INT))	      //检测拨码开关输入状态    
 
#define DI1_INT       	        (1ul << 25) 
#define DI1_INT_Init()   	      (LPC_GPIO3->DIR &= ~DI1_INT)            //初始化拨码开关    
#define DI1_INT_Status() 	      (!(LPC_GPIO3->PIN & DI1_INT))	      //检测拨码开关输入状态    

#define DI2_INT       	      (1ul << 20) 
#define DI2_INT_Init()   	  (LPC_GPIO1->DIR &= ~DI2_INT)         //初始化拨码开关    
#define DI2_INT_Status() 	  (!(LPC_GPIO1->PIN & DI2_INT))	       //检测拨码开关输入状态    

#define DI3_INT       	      (1ul << 23) 
#define DI3_INT_Init()   	  (LPC_GPIO1->DIR &= ~DI3_INT)         //初始化拨码开关    
#define DI3_INT_Status() 	  (!(LPC_GPIO1->PIN & DI3_INT))	       //检测拨码开关输入状态    

#define DI4_INT       	      (1ul << 24) 
#define DI4_INT_Init()   	  (LPC_GPIO1->DIR &= ~DI4_INT)         //初始化拨码开关    
#define DI4_INT_Status() 	  (!(LPC_GPIO1->PIN & DI4_INT))	       //检测拨码开关输入状态    
 
extern void GPIOInit(void );










#endif

