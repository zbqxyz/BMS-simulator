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
** Descriptions:        ����ʱ��
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ExRTCSet(DATETime *pTimeAndDate )
{	 
		  
    pTimeAndDate->year     = 2018;                                        /* ��ʼ����  �ȼ���   (*pTimeAndDate).year     = 2018;                         */	
    pTimeAndDate->month    = 12;                                           /* ��ʼ����                     */
    pTimeAndDate->day      = 5;                                           /* ��ʼ����                     */
    pTimeAndDate->week     = 4;                                           /* ��ʼ����                     */
    pTimeAndDate->hour     = 12;                                          /* ��ʼ��ʱ                     */
    pTimeAndDate->minute   = 12;                                          /* ��ʼ����                     */
    pTimeAndDate->second   = 12;                                          /* ��ʼ����                     */      	
} 

/*********************************************************************************************************
** Function name:       RTCInit
** Descriptions:        ��ʼ��RTC
** input parameters:    *pTimeAndDate�����ʼ��ʱ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint32_t RTCInit (DATETime *pTimeAndDate )
{	
  
		LPC_SC->PCONP |= (1ul << 9);                      /* ���� RTC Ҫ�õ��ⲿʱ��     */

		if ( LPC_RTC->RTC_AUX & (0x1<<4) )              /* RTC ����ʧЧ̽���־      */
		{
			LPC_RTC->RTC_AUX |= (0x1<<4);                 /* ���                        */
		}
	
		LPC_RTC->CCR   = 0x00;                          /* ��ֹʱ�������               */
    LPC_RTC->ILR   = 0x03;                          /* ����ж�                  		*/
    LPC_RTC->CIIR  = 0x1;                           /* ʹ���������ж�               */
		//LPC_RTC->CIIR = 0;                            /* ������������                 */
		LPC_RTC->AMR   = 0xff;                          /* ��ֹ�����ж�                	*/
    
		//LPC_RTC->CALIBRATION = 0x0;                   /* У׼�Ĵ�����0                */
		
    LPC_RTC->YEAR  = pTimeAndDate->year;            /* �˴���Ϊǧ��                 */
    LPC_RTC->MONTH = pTimeAndDate->month;
    LPC_RTC->DOM   = pTimeAndDate->day;
    LPC_RTC->DOW   = pTimeAndDate->week;
    LPC_RTC->HOUR  = pTimeAndDate->hour;
    LPC_RTC->MIN   = pTimeAndDate->minute;
    LPC_RTC->SEC   = pTimeAndDate->second;
    
    LPC_RTC->CIIR  = 0x01;

    NVIC_EnableIRQ(RTC_IRQn);                        /* �����жϲ�ʹ��                  */
    NVIC_SetPriority(RTC_IRQn, 18);
   
	  LPC_RTC->CIIR = 0x00000001;                      /* Set counter increment interrupt */
		LPC_RTC->ILR = (0x01 | 0x02);                    /* ��������������ж� ��־         */

	  LPC_RTC->CCR   = 0x01;                           /* ����RTC                         */   

    return 1; 
}

/*********************************************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTC�жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void  RTC_IRQHandler (void)
{  	                                                 					
	static uint8_t tmp;

	ulTimes = LPC_RTC->CTIME0;                                                	/* ��ȡ������ʱ�ӼĴ���         */
	ulDatas = LPC_RTC->CTIME1;
 
	RTCtimedate.year   	= (ulDatas >> 16) & 0xfff; 
	RTCtimedate.month  	= (ulDatas >> 8) & 0x0f; 
	RTCtimedate.day   	=  ulDatas & 0x1f; 
	RTCtimedate.week  	= (ulTimes >> 24) & 0x07; 
	RTCtimedate.hour  	= (ulTimes >> 16) & 0x1f;
	RTCtimedate.minute	= (ulTimes >> 8) & 0x3f;
	RTCtimedate.second	=  ulTimes & 0x3f;
 
	LPC_RTC->ILR = 0x01;															                          /* ����жϱ�־                 */			
	secflag = 1;															   	                              //���־
	ScnTime++; 	
	DspDlyNs++;															                                    //��ʱ��ʱ
	WelinkTime++;
	Ethernet_Reply_Time++;	

	if(tmp++%2)
	{
		SYSLED_ON();
							
	}	else 
	{
		
	SYSLED_OFF();
				
	}
	
 if(Sys_PARA.TimeChargeMode==1)                                             //��ʱ���ģʽ
	  ChargedTime++;
 else
	 ChargedTime=0;
 
 if(Sys_PARA.SOCChargeMode==1)                                               //��SOC���ģʽ
 {
	  SOChargedTime++;
 }
 if(((SOChargedTime/60)>=Sys_PARA.SOCIncreaseTime)&&(Sys_PARA.SOCChargeMode==1)) 
 {
    BMSMessage.SOC++;
 
 }
}
