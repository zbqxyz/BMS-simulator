/****************************************Copyright (c)****************************************************
**                                 Tomorn electronics Co.,LTD.
**                                     
**                                 http://www.tomorn.cn
**                                      wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Main.c
** Last modified Date:      2018.05.18
** Last Version:            1.0
** Description:             The main function  ������
*********************************************************************************************************/

#include <includes.h>
#include <cortexm3.h>

/*********************************************************************************************************
 �궨��
*********************************************************************************************************/

/*********************************************************************************************************
  VARIABLES ����
*********************************************************************************************************/
static OS_STK  GstkStart[TASK_START_STK_SIZE];                            /*  ��������Ķ�ջ            */
static OS_STK  GstkDISPLAY[TASK_DISPLAY_STK_SIZE];                        /*  ��������Ķ�ջ            */
static OS_STK  GstkMETER[TASK_METER_STK_SIZE];                            /*  ��������Ķ�ջ            */
static OS_STK  GstkBMS[TASK_BMS_STK_SIZE];                                /*  BMSͨѶ����Ķ�ջ         */
static OS_STK  GstkWelink[TASK_Welink_STK_SIZE];                          /*  4G����Ķ�ջ              */
/*********************************************************************************************************
   FUNCTION PROTOTYPES ��������
*********************************************************************************************************/
static void taskStart (void  *parg);                                      /*  The start task  ��������  */
static void taskDisplay(void  *parg);
static void taskMeter(void  *parg);
static void taskBMS(void  *parg);
static void taskWelink(void  *parg);
static void SystemParameterInit(void);


SYS_PARA   Sys_PARA;


/*********************************************************************************************************
** Function name:	      main
** Descriptions:	      �������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main (void)
{    
		GPIOInit();
	  SSP0_Init();
	  SSP2_Init();
	  Timer0Init();
	  UART0Init();
	  UART1Init(); 
	  UART2Init();
	  UART3Init();	
    UART4Init();
	
	  CAN_Init(0, BPS_250K);
	  CAN_Init(1, BPS_250K);
	
	  ExRTCSet(&RTCtimedate);                                             /*�ṹ�崫��ʱ�����ݽṹ��ĵ�ַ*/
    RTCInit (&RTCtimedate);	                                            /*�ṹ�崫��ʱ�����ݽṹ��ĵ�ַ*/
	
	  SystemParameterInit();		 
		
    OSInit();                                                           /*  ��ʼ���ں�                  */                                                                   
	  OSTaskCreate (taskStart,                                          
						   (void *)0, 
				       &GstkStart[TASK_START_STK_SIZE-1],  
			      	 TASK_START_PRIO );                                     /*  Initialize the start task   */							 
																																			/*  ��ʼ����������              */ 
 	 if(PRINT_STRING)
	  Print("%s\n", "OS Start  !");							 
	  OSStart();                                                        /*  Start uC/OS-II ����uC/OS-II */
	  return(0) ;
}

/*********************************************************************************************************
** Function name:           taskStart	   
** Descriptions:            Start task	
** input parameters:        *parg
** output parameters:       ��      
** Returned value:          �� 
*********************************************************************************************************/
static void taskStart (void  *parg)
{
    (void)parg;
		tick_init();
    #if OS_TASK_STAT_EN > 0
    OSStatInit();                                                      /*  ʹ��ͳ�ƹ���                */
    #endif

  	/*
     *  Create the other tasks here. �����ﴴ����������
     */
                                 								 
    OSTaskCreate (taskDisplay, 
								 (void *)0,   		
			            &GstkDISPLAY[TASK_DISPLAY_STK_SIZE-1], 
				          TASK_DISPLAY_PRIO); 	
								 
    OSTaskCreate (taskMeter, 
								 (void *)0,   		
			            &GstkMETER[TASK_METER_STK_SIZE-1], 
				          TASK_METER_PRIO);
								 
	  OSTaskCreate (taskBMS, 
								 (void *)0,   		
			            &GstkBMS[TASK_BMS_STK_SIZE-1], 
				          TASK_BMS_PRIO);	
								 
    OSTaskCreate (taskWelink, 
								 (void *)0,   		
			            &GstkWelink[TASK_Welink_STK_SIZE-1], 
				          TASK_Welink_PRIO); 										 
						 
	while (1) 
		{  
			  if(PRINT_STRING)
		  	Print("%s\n", "OS Start Finished !");    
        OSTaskSuspend(OS_PRIO_SELF);                                    /*  The start task can be pended*/
                                                                        /*  here. ������������������  */
    }
}

/*********************************************************************************************************
** Function name:           other tasks	   
** Descriptions:            other tasks	
** input parameters:        *parg
** output parameters:       ��      
** Returned value:          �� 
*********************************************************************************************************/

static  void  taskDisplay(void  *parg)
{
	(void)parg;
	 if(PRINT_STRING)
	 Print("%s\n", "DisplayMain Task Action !");
    while (1) 
	{
    OSTimeDlyHMSM(0,0,0,1);
			DisplayMain();		
	}
}

static  void  taskMeter(void  *parg)
{
	(void)parg;
	if(PRINT_STRING)
	 Print("%s\n", "MeterMain Task Action !");
    while (1) 
	{	
		OSTimeDlyHMSM(0,0,0,1);
		MeterMain();		
	}
}

static  void  taskBMS(void  *parg)
{
	(void)parg;
	 if(PRINT_STRING)
	 Print("%s\n", "BMSMain Task Action !");
	 BMSMain();
}


static  void  taskWelink(void  *parg)
{
	(void)parg;		
   WelinkMain();		
}


/*********************************************************************************************************
** Function name:           SystemParameterInit	   
** Descriptions:            ϵͳ������ʼ��	
** input parameters:        ��
** output parameters:       ��      
** Returned value:          �� 
*********************************************************************************************************/

static void SystemParameterInit(void)
{	

    FramRead((uint8_t *)&BMSMessage,BMS_MSG_PARA_ADDR,sizeof(BMSMessage));
//     if (BMSMessage.CS != CheckSum((uint8_t *)&BMSMessage,(uint8_t *)&BMSMessage+sizeof(BMSMessage)-1))
//		 {
//			    memset(&BMSMessage,0,sizeof(BMSMessage));
//			    BMSParameterInit();	 
//		 }
		 
	  FramRead((uint8_t *)&Welink_PARA,WELINK_PARA_ADDR,sizeof(Welink_PARA));   
//		 if (Welink_PARA.CS != CheckSum((uint8_t *)&Welink_PARA,(uint8_t *)&Welink_PARA+sizeof(Welink_PARA)-1))
//		 {
//			    memset(&Welink_PARA,0,sizeof(Welink_PARA));			 
//			    SysLinkParaInit();			 
//		 }		 
		 FramRead((uint8_t *)&Sys_PARA,SYS_PARA_ADDR,sizeof(Sys_PARA));             
		 		 
  	if(PRINT_STRING)
	    Print("%s\n", "PARA Read !"); 
				
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
