/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                 http://www.tomorn.cn
**                                      wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               rtc.c
** Last modified Date:      2017.05.18
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <rtc.h>

uint8_t	 secflag;
uint16_t ScnTime;
uint16_t DspDlyNs;
uint16_t WelinkTime;
uint16_t WelinkTime;
uint16_t Ethernet_Reply_Time;
uint32_t ChargedTime;
uint32_t SOChargedTime;
uint32_t ulTimes;
uint32_t ulDatas;
DATETime RTCtimedate;

/*********************************************************************************************************
** Function name:       ExRTCSet
** Descriptions:        设置时钟
** input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ExRTCSet(DATETime *pTimeAndDate )
{	 
		  
    pTimeAndDate->year     = 2018;                                        /* 初始化年  等价于   (*pTimeAndDate).year     = 2018;                         */	
    pTimeAndDate->month    = 12;                                           /* 初始化月                     */
    pTimeAndDate->day      = 5;                                           /* 初始化日                     */
    pTimeAndDate->week     = 4;                                           /* 初始化周                     */
    pTimeAndDate->hour     = 12;                                          /* 初始化时                     */
    pTimeAndDate->minute   = 12;                                          /* 初始化分                     */
    pTimeAndDate->second   = 12;                                          /* 初始化秒                     */      	
} 

/*********************************************************************************************************
** Function name:       RTCInit
** Descriptions:        初始化RTC
** input parameters:    *pTimeAndDate输入初始化时间
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint32_t RTCInit (DATETime *pTimeAndDate )
{	
  
		LPC_SC->PCONP |= (1ul << 9);                      /* 访问 RTC 要用到外部时钟     */

		if ( LPC_RTC->RTC_AUX & (0x1<<4) )              /* RTC 振荡器失效探测标志      */
		{
			LPC_RTC->RTC_AUX |= (0x1<<4);                 /* 清除                        */
		}
	
		LPC_RTC->CCR   = 0x00;                          /* 禁止时间计数器               */
    LPC_RTC->ILR   = 0x03;                          /* 清除中断                  		*/
    LPC_RTC->CIIR  = 0x1;                           /* 使能秒增量中断               */
		//LPC_RTC->CIIR = 0;                            /* 屏蔽增量报警                 */
		LPC_RTC->AMR   = 0xff;                          /* 禁止报警中断                	*/
    
		//LPC_RTC->CALIBRATION = 0x0;                   /* 校准寄存器清0                */
		
    LPC_RTC->YEAR  = pTimeAndDate->year;            /* 此处年为千年                 */
    LPC_RTC->MONTH = pTimeAndDate->month;
    LPC_RTC->DOM   = pTimeAndDate->day;
    LPC_RTC->DOW   = pTimeAndDate->week;
    LPC_RTC->HOUR  = pTimeAndDate->hour;
    LPC_RTC->MIN   = pTimeAndDate->minute;
    LPC_RTC->SEC   = pTimeAndDate->second;
    
    LPC_RTC->CIIR  = 0x01;

    NVIC_EnableIRQ(RTC_IRQn);                        /* 设置中断并使能                  */
    NVIC_SetPriority(RTC_IRQn, 18);
   
	  LPC_RTC->CIIR = 0x00000001;                      /* Set counter increment interrupt */
		LPC_RTC->ILR = (0x01 | 0x02);                    /* 清除增量、报警中断 标志         */

	  LPC_RTC->CCR   = 0x01;                           /* 启动RTC                         */   

    return 1; 
}

/*********************************************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTC中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void  RTC_IRQHandler (void)
{  	                                                 					
	static uint8_t tmp;

	ulTimes = LPC_RTC->CTIME0;                                                	/* 读取完整的时钟寄存器         */
	ulDatas = LPC_RTC->CTIME1;
 
	RTCtimedate.year   	= (ulDatas >> 16) & 0xfff; 
	RTCtimedate.month  	= (ulDatas >> 8) & 0x0f; 
	RTCtimedate.day   	=  ulDatas & 0x1f; 
	RTCtimedate.week  	= (ulTimes >> 24) & 0x07; 
	RTCtimedate.hour  	= (ulTimes >> 16) & 0x1f;
	RTCtimedate.minute	= (ulTimes >> 8) & 0x3f;
	RTCtimedate.second	=  ulTimes & 0x3f;
 
	LPC_RTC->ILR = 0x01;															                          /* 清除中断标志                 */			
	secflag = 1;															   	                              //秒标志
	ScnTime++; 	
	DspDlyNs++;															                                    //延时计时
	WelinkTime++;
	Ethernet_Reply_Time++;	

	if(tmp++%2)
	{
		SYSLED_ON();
							
	}	else 
	{
		
	SYSLED_OFF();
				
	}
	
 if(Sys_PARA.TimeChargeMode==1)                                             //限时充电模式
	  ChargedTime++;
 else
	 ChargedTime=0;
 
 if(Sys_PARA.SOCChargeMode==1)                                               //限SOC充电模式
 {
	  SOChargedTime++;
 }
 if(((SOChargedTime/60)>=Sys_PARA.SOCIncreaseTime)&&(Sys_PARA.SOCChargeMode==1)) 
 {
    BMSMessage.SOC++;
 
 }
}
