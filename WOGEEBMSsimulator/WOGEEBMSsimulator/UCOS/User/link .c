/****************************************Copyright (c)****************************************************
**                                    Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               link.c
** Last modified Date:      2018.06.30
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include "link.h"

static uint16_t HeartBeatCnt;

/*********************************************************************************************************
** Function name:       cmd_02_upload
** Descriptions:        充电桩的整形参数和设置应答给服务器
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_02_upload(uint8_t *pbgn,uint8_t port)
 {
	 return 0;
 }
 
 /*********************************************************************************************************
** Function name:       cmd_04_upload
** Descriptions:        充电桩的字符参数和设置应答给服务器
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_04_upload(uint8_t *pbgn,uint8_t port)
 {
	 
	 
	 return 0;
 }
 
 /*********************************************************************************************************
** Function name:       cmd_06_upload
** Descriptions:        充电桩的后台控制命令应答
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_06_upload(uint8_t *pbgn,uint8_t port)
 {
	  uint8_t *pdat;
  	uint8_t i;
	
	 pdat = pbgn;	 
	
	*pdat++ = 0x00;                                                //业务数据              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //预留
	*pdat++ = 0;
	 
	 for(i=0;i<32;i++)                                             //桩编号  32字节
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }	 
	 
	*pdat++ = 0;                                                   //充电枪口 1字节
	 
	*pdat++ = 0;	                                                 //命令起始标志 4字节
	*pdat++ = 0;	                                                 //命令起始标志 4字节	 
	*pdat++ = 0;	                                                 //命令起始标志 4字节 
	*pdat++ = 0;	                                                 //命令起始标志 4字节	 
	 	 
	*pdat++ = 0;	                                                 //命令个数 
	*pdat++ = 0;	                                                 //命令执行结果
	 
	return pdat-pbgn;
 }
 
 /*********************************************************************************************************
** Function name:       cmd_08_upload
** Descriptions:        应答后台控制开启充电命令
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_08_upload(uint8_t *pbgn,uint8_t port)
 {
  uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	 
	
	*pdat++ = 0x00;                                                //业务数据              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //预留
	*pdat++ = 0;
	 
	 for(i=0;i<32;i++)                                             //桩编号  32字节
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }	 
	 
	*pdat++ = 0;                                                   //充电枪口 1字节
	 
	*pdat++ = 0;	                                                 //命令执行结果 4字节
	*pdat++ = 0;	                                                 //命令执行结果 4字节	 
	*pdat++ = 0;	                                                 //命令执行结果 4字节	 
	*pdat++ = 0;	                                                 //命令执行结果 4字节	 
	 	 
	return pdat-pbgn;
 }
 /*********************************************************************************************************
** Function name:       cmd_10_upload
** Descriptions:        上传命令请求
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_10_upload(uint8_t *pbgn,uint8_t port)
 {
	 return 0;
 }

/*********************************************************************************************************
** Function name:       cmd_102_upload
** Descriptions:        主动上报心跳
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_102_upload(uint8_t *pbgn,uint8_t port)
{
	uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	
	
	*pdat++ = 0xff;                                                //业务数据              
	*pdat++ = 0xff;
	
	*pdat++ = 0;                                                   //预留
	*pdat++ = 0;
	
	 for(i=0;i<32;i++)                                             //桩编号  32字节
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	 
	  HeartBeatCnt++;
	 *pdat++ = HeartBeatCnt%256;                                  //发送心跳计数         
	 *pdat++ = HeartBeatCnt/256;
	 
	return pdat-pbgn;
	 
}
/*********************************************************************************************************
** Function name:       cmd_104_upload
** Descriptions:        充电桩状态信息上报
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
 uint16_t cmd_104_upload(uint8_t *pbgn,uint8_t port)
 {
	uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	
	
	*pdat++ = 0x0d;                                                //业务数据              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //预留
	*pdat++ = 0;
	
	 for(i=0;i<32;i++)                                             //桩编号  32字节
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	 *pdat++ = 1;                                                  //充电枪数量 1个字节
	 *pdat++ = 1;                                                  //充电口     1个字节 
	 *pdat++ = 1;                                                  //充电桩类型 1个字节
	 *pdat++ = 0;                                                  //充电桩工作状态  待机 or 工作中 1个字节
	 *pdat++ = 0;                                                  //当前SOC    1个字节

	 *pdat++ = 0;                                                  //告警编码   4个字节
	 *pdat++ = 0;                                                  //告警编码   4个字节
	 *pdat++ = 0;                                                  //告警编码   4个字节
	 *pdat++ = 0;                                                  //告警编码   4个字节
 
	 *pdat++ = 0;                                                  //车连接状态 1个字节

	 *pdat++ = 0;                                                  //本次充电累计充电费用  4个字节	 
	 *pdat++ = 0;                                                  //本次充电累计充电费用  4个字节
	 *pdat++ = 0;                                                  //本次充电累计充电费用  4个字节
	 *pdat++ = 0;                                                  //本次充电累计充电费用  4个字节

	 *pdat++ = 0;                                                  //内部变量 8个字节
	 *pdat++ = 0;                                                  //内部变量 8个字节
	 *pdat++ = 0;                                                  //内部变量 8个字节
	 *pdat++ = 0;                                                  //内部变量 8个字节
	 *pdat++ = 0;                                                  //内部变量 8个字节
	 *pdat++ = 0;                                                  //内部变量 8个字节	 
	 *pdat++ = 0;                                                  //内部变量 8个字节	 
	 *pdat++ = 0;                                                  //内部变量 8个字节
	 
	 *pdat++ = 0;                                                  //直流充电电压 2个字节
	 *pdat++ = 0;                                                  //直流充电电压 2个字节	 
	 
	 *pdat++ = 0;                                                  //直流充电电流 2个字节
	 *pdat++ = 0;                                                  //直流充电电流 2个字节	 	 
	 
	 *pdat++ = 0;                                                  //BMS需求电压  2个字节
	 *pdat++ = 0;                                                  //BMS需求电压  2个字节
	 
	 *pdat++ = 0;                                                  //BMS需求电流  2个字节
	 *pdat++ = 0;                                                  //BMS需求电流  2个字节 
	 	 
	 *pdat++ = 0;                                                  //BMS充电模式	 
	 
	 *pdat++ = 0;                                                  //交流A相充电电压 2个字节
	 *pdat++ = 0;                                                  //交流A相充电电压 2个字节
	 
	 *pdat++ = 0;                                                  //交流B相充电电压 2个字节
	 *pdat++ = 0;                                                  //交流B相充电电压 2个字节	 	 
	 
	 *pdat++ = 0;                                                  //交流C相充电电压 2个字节
	 *pdat++ = 0;                                                  //交流C相充电电压 2个字节
	 
	 *pdat++ = 0;                                                  //交流A相充电电流 2个字节
	 *pdat++ = 0;                                                  //交流A相充电电流 2个字节	 
	 
	 *pdat++ = 0;                                                  //交流B相充电电流 2个字节
	 *pdat++ = 0;                                                  //交流B相充电电流 2个字节	 
	 
	 *pdat++ = 0;                                                  //交流C相充电电流 2个字节
	 *pdat++ = 0;                                                  //交流C相充电电流 2个字节	 
	 
	 *pdat++ = 0;                                                  //剩余充电时间    2个字节
	 *pdat++ = 0;                                                  //剩余充电时间    2个字节 

	 *pdat++ = 0;                                                  //充电时间    4个字节
	 *pdat++ = 0;                                                  //充电时间    4个字节 
	 *pdat++ = 0;                                                  //充电时间    4个字节
	 *pdat++ = 0;                                                  //充电时间    4个字节

	 *pdat++ = 0;                                                  //充电电量   4个字节
	 *pdat++ = 0;                                                  //充电电量   4个字节
	 *pdat++ = 0;                                                  //充电电量   4个字节	 
	 *pdat++ = 0;                                                  //充电电量   4个字节	 
	 
	 *pdat++ = 0;                                                  //充电前电表读数   4个字节	 
	 *pdat++ = 0;                                                  //充电前电表读数   4个字节	
	 *pdat++ = 0;                                                  //充电前电表读数   4个字节	
	 *pdat++ = 0;                                                  //充电前电表读数   4个字节	
	 
	 *pdat++ = 0;                                                  //当前电表读数  4个字节	
	 *pdat++ = 0;                                                  //当前电表读数  4个字节	
	 *pdat++ = 0;                                                  //当前电表读数  4个字节	
	 *pdat++ = 0;                                                  //当前电表读数  4个字节		

	 *pdat++ = 0;                                                  //充电启动方式

	 *pdat++ = 0;                                                  //充电策略 1个字节
	 *pdat++ = 0;                                                  //充电参数	4个字节
	 *pdat++ = 0;                                                  //充电参数	4个字节	 
	 *pdat++ = 0;                                                  //充电参数	4个字节	 
	 *pdat++ = 0;                                                  //充电参数	4个字节	 
	 
	 *pdat++ = 0;                                                  //预约标志	 
	 
	 for(i=0;i<32;i++)                                             //充电卡号  32字节
	 {
	   *pdat++ = 0;
	 }
	 
	 *pdat++ = 0;                                                  //预约超时时间 1字节
	 
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节	 
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节	 
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节	 
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节
	 *pdat++ = 0;                                                  //预约开始充电时间 8字节

	 *pdat++ = 0;                                                  //充电前卡余额  4字节	 
	 *pdat++ = 0;                                                  //充电前卡余额  4字节	 	 	 
	 *pdat++ = 0;                                                  //充电前卡余额  4字节	
	 *pdat++ = 0;                                                  //充电前卡余额  4字节		 	 
	 
	 *pdat++ = 0;                                                  //预留  4字节	 
	 *pdat++ = 0;                                                  //预留  4字节	 
	 *pdat++ = 0;                                                  //预留  4字节	 	 
	 *pdat++ = 0;                                                  //预留  4字节	
	 
	 *pdat++ = 0;                                                  //充电功率 4字节	 
	 *pdat++ = 0;                                                  //充电功率 4字节	 	 	 	 
	 *pdat++ = 0;                                                  //充电功率 4字节	 
	 *pdat++ = 0;                                                  //充电功率 4字节	 	

	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节
	 *pdat++ = 0;                                                  //系统变量 12个字节	 
	 
	 *pdat++ = 0;                                                  //出风口温度，偏移量-50  1字节
	 *pdat++ = 0;                                                  //环境温度，  偏移量-50  1字节
	 *pdat++ = 0;	                                                 //进风口温度，偏移量-50  1字节                                          
 
	return pdat-pbgn;
	  
 }

/*********************************************************************************************************
** Function name:       cmd_106_upload
** Descriptions:        充电桩签到信息上报
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_106_upload(uint8_t *pbgn,uint8_t port)
{
	uint8_t *pdat;
	uint16_t i;
	
	 pdat = pbgn;
	*pdat++ = 0x00;                                               //AES 加密是有效  2字节
	*pdat++ = 0x00;
	*pdat++ = 0xff;                                               //预留  2字节
	*pdat++ = 0xff;
	
	 for(i=0;i<32;i++)                                            //桩编号  32字节
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	  *pdat++ = Welink_PARA.SicFlag;
    *pdat++ = Welink_PARA.SoftVer%256;                         //软件版本号 4字节
    *pdat++ = (Welink_PARA.SoftVer>>4)&0xff;
	  *pdat++ = (Welink_PARA.SoftVer>>8)&0xff;
	  *pdat++ = (Welink_PARA.SoftVer>>16)&0xff;
	 
	  *pdat++ = Welink_PARA.ItemType%256;                        //充电桩项目类型 2字节    
	  *pdat++ = Welink_PARA.ItemType/256;
	 
    *pdat++ = Welink_PARA.StartTimes%256;                      //启动次数 4字节
    *pdat++ = (Welink_PARA.StartTimes>>4)&0xff;
	  *pdat++ = (Welink_PARA.StartTimes>>8)&0xff;
	  *pdat++ = (Welink_PARA.StartTimes>>16)&0xff;	    
	  
	  *pdat++ = Welink_PARA.UploadMode;                           //数据上传模式
	 
	  *pdat++ = Welink_PARA.CheckInTime%256;                     //签到间隔时间
	  *pdat++ = Welink_PARA.CheckInTime/256;
	 
	  *pdat++ = Welink_PARA.InternalVariable;                     //运行内部变量	 
	  *pdat++ = Welink_PARA.ChargePort;                           //充电枪数量
	  *pdat++ = Welink_PARA.HeartRate;                            //心跳周期
	  *pdat++ = Welink_PARA.HeartRateTimeOut;                     //心跳周期超时次数
		
	  *pdat++ = Welink_PARA.ReportNo%256;                      //启动次数 4字节
    *pdat++ = (Welink_PARA.ReportNo>>4)&0xff;
	  *pdat++ = (Welink_PARA.ReportNo>>8)&0xff;
	  *pdat++ = (Welink_PARA.ReportNo>>16)&0xff;
				
	  *pdat++ = HEX_To_BCD((RTCtimedate.year>>4)&0xff);           //当前系统时间
		*pdat++ = HEX_To_BCD(RTCtimedate.year%256);
	  *pdat++ = HEX_To_BCD(RTCtimedate.month);
		*pdat++ = HEX_To_BCD(RTCtimedate.day);
		*pdat++ = HEX_To_BCD(RTCtimedate.hour);
	  *pdat++ = HEX_To_BCD(RTCtimedate.minute);
		*pdat++ = HEX_To_BCD(RTCtimedate.second);
		*pdat++ = 0xff;                                             //保留，填充
	
	  for(i=0;i<24;i++)                                            //预留  32字节
	 {
	   *pdat++ = 0x00;
	 }	
	 
    for(i=0;i<4;i++)                                            //随机数  4字节
	 {
	   *pdat++ =  rand()%256;
	 }	
				
	return pdat-pbgn;
	
}

/*********************************************************************************************************
** Function name:       cmd_108_upload
** Descriptions:        充电桩的告警信息上报
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_108_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}

/*********************************************************************************************************
** Function name:       cmd_110_upload
** Descriptions:        充电启动完成
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_110_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}

/*********************************************************************************************************
** Function name:       cmd_202_upload
** Descriptions:        充电记录上报
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_202_upload(uint8_t *pbgn,uint8_t port)
{
	uint8_t *pdat;
	uint8_t i;
	
	 pdat = pbgn;	
	
	*pdat++ = 0x0d;                                                //业务数据              
	*pdat++ = 0x00;
	
	*pdat++ = 0;                                                   //预留
	*pdat++ = 0;
	
	 for(i=0;i<32;i++)                                             //桩编号  32字节
	 {
	   *pdat++ = Welink_PARA.TermNo[i];
	 }
	 *pdat++ = 1;                                                  //充电枪位置类型 1个字节
	 *pdat++ = 1;                                                  //充电口     1个字节 

	 for(i=0;i<32;i++)                                             //充电卡号  32字节
	 {
	   *pdat++ = 0;
	 }

	 *pdat++ = 0;                                                  //充电开始时间 8字节
	 *pdat++ = 0;                                                  //充电开始时间	8字节
	 *pdat++ = 0;                                                  //充电开始时间	8字节
	 *pdat++ = 0;                                                  //充电开始时间 8字节
	 *pdat++ = 0;                                                  //充电开始时间	8字节
	 *pdat++ = 0;                                                  //充电开始时间 8字节
	 *pdat++ = 0;                                                  //充电开始时间 8字节
	 *pdat++ = 0;                                                  //充电开始时间 8字节

	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节
	 *pdat++ = 0;                                                  //充电结束时间 8字节

	 *pdat++ = 0;                                                  //充电时间长度 4字节 
	 *pdat++ = 0;                                                  //充电时间长度 4字节
	 *pdat++ = 0;                                                  //充电时间长度 4字节
	 *pdat++ = 0;                                                  //充电时间长度 4字节

	 *pdat++ = 0;                                                  //开始SOC
	 *pdat++ = 0;                                                  //结束SOC
	 
	 *pdat++ = 0;                                                  //充电结束原因 4字节
	 *pdat++ = 0;                                                  //充电结束原因 4字节
	 *pdat++ = 0;                                                  //充电结束原因 4字节
	 *pdat++ = 0;                                                  //充电结束原因 4字节

	 *pdat++ = 0;                                                  //本次充电电量 4字节
	 *pdat++ = 0;                                                  //本次充电电量 4字节	 
	 *pdat++ = 0;                                                  //本次充电电量 4字节	 
	 *pdat++ = 0;                                                  //本次充电电量 4字节	 
	 
	 *pdat++ = 0;                                                  //充电前电表读数 4字节 
	 *pdat++ = 0;                                                  //充电前电表读数 4字节 	 
	 *pdat++ = 0;                                                  //充电前电表读数 4字节 
	 *pdat++ = 0;                                                  //充电前电表读数 4字节 	 
	 
	 *pdat++ = 0;                                                  //充电后电表读数 4字节 	 
	 *pdat++ = 0;                                                  //充电后电表读数 4字节 		 
	 *pdat++ = 0;                                                  //充电后电表读数 4字节 		 
	 *pdat++ = 0;                                                  //充电后电表读数 4字节 		 

	 *pdat++ = 0;                                                  //本次充电金额 4字节 	
	 *pdat++ = 0;                                                  //本次充电金额 4字节 
	 *pdat++ = 0;                                                  //本次充电金额 4字节 
	 *pdat++ = 0;                                                  //本次充电金额 4字节 

	 *pdat++ = 0;                                                  //预留
	 *pdat++ = 0;                                                  //预留
	 *pdat++ = 0;                                                  //预留
	 *pdat++ = 0;                                                  //预留

	 *pdat++ = 0;                                                  //充点前卡余额 4字节
	 *pdat++ = 0;                                                  //充点前卡余额 4字节
	 *pdat++ = 0;                                                  //充点前卡余额 4字节
	 *pdat++ = 0;                                                  //充点前卡余额 4字节

	 *pdat++ = 0;                                                  //充电记录索引 4字节
	 *pdat++ = 0;                                                  //充电记录索引 4字节
	 *pdat++ = 0;                                                  //充电记录索引 4字节
	 *pdat++ = 0;                                                  //充电记录索引 4字节

	 *pdat++ = 0;                                                  //总充电记录条目 4字节
	 *pdat++ = 0;                                                  //总充电记录条目 4字节
	 *pdat++ = 0;                                                  //总充电记录条目 4字节
	 *pdat++ = 0;                                                  //总充电记录条目 4字节

	 *pdat++ = 0;                                                  //预留 1字节
	 
	 *pdat++ = 0;                                                  //充电策略 1字节
	 *pdat++ = 0;                                                  //充电策略参数 4字节
	 *pdat++ = 0;                                                  //充电策略参数 4字节
	 *pdat++ = 0;                                                  //充电策略参数 4字节	 
	 *pdat++ = 0;                                                  //充电策略参数 4字节
	 
	 for(i=0;i<17;i++)                                             //车辆VIN  17字节
	 {
	   *pdat++ = 0;
	 }

	 for(i=0;i<8;i++)                                              //车牌号  8字节
	 {
	   *pdat++ = 0;
	 }

	 for(i=0;i<96;i++)                                              //时段电量  96字节
	 {
	   *pdat++ = 0;
	 }

	 *pdat++ = 0;                                                  //启动方式 1字节
	 
	 for(i=0;i<32;i++)                                              //充电流水号  96字节
	 {
	   *pdat++ = 0;
	 }
	 
	 	return pdat-pbgn;	
	
}

/*********************************************************************************************************
** Function name:       cmd_204_upload
** Descriptions:        用户账户查询上报
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_204_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}

/*********************************************************************************************************
** Function name:       cmd_206_upload
** Descriptions:        用户密码验证上报
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
uint16_t cmd_206_upload(uint8_t *pbgn,uint8_t port)
{
	return 0;
}



/*********************************************************************************************************
** Function name:       cmd_1_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_1_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_3_query_2_proess
** Descriptions:        协议通讯时间查询处理函数
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_3_query_2_proess(uint8_t *p,uint16_t len,uint8_t port)
{
	
	
	
}

/*********************************************************************************************************
** Function name:       cmd_3_setup_2_proess
** Descriptions:        协议通讯时间设置处理函数
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_3_setup_2_proess(uint8_t *p,uint16_t len,uint8_t port)
{
	
	
	
}


/*********************************************************************************************************
** Function name:       cmd_3_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_3_proess(uint8_t *p,uint16_t len,uint8_t port)
{ 
   uint32_t  para_data_addr;                            //参数地址
	
	 para_data_addr =(*(p+5))+(*(p+6)*256)+(*(p+7)*256*256)+(*(p+8)*256*256*256);
	
    if(*(p+4)==0)                                        //查询寻数据
		{	
			switch (para_data_addr)
			{
				case 1:                                          //查询充电桩编码
					
					   break;
								
				case 2:                                          //查询标准时钟时间
					   break;	
					 
		   	case 3:                                          //查询管理员密码
			  		cmd_3_query_2_proess(p+10,*(p+9),0);
					   break;
				
		   	case 4:                                          //查询操作员密码
					   break;				
				
		   	case 5:                                          //查询MAC地址
					   break;			
		   	case 6:                                          //预留
					   break;	
		   	case 7:                                          //查询二维码
					   break;		
				
				default:
					   break;				   
				 
			}
		 UpstreamCmdHandle(4,port);                          //发送心跳	
		}		
    else if(*(p+4)==1)                                   //设置参数
		{	
			switch (para_data_addr)
			{
				case 1:                                          //设置充电桩编码
					
					   break;
								
				case 2:                                          //设置标准时钟时间
					  cmd_3_setup_2_proess(p+10,*(p+9),0);
				    
					   break;	
					 
		   	case 3:                                          //设置管理员密码
					   break;
				
		   	case 4:                                          //设置操作员密码
					   break;				
				
		   	case 5:                                          //设置MAC地址
					   break;			
		   	case 6:                                          //预留
					   break;	
		   	case 7:                                          //设置二维码
					   break;		
				
				default:
					   break;				   
				 
			}
				
		}
		
		
}

/*********************************************************************************************************
** Function name:       cmd_5_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_5_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_7_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_7_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_9_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_9_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_101_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_101_proess(uint8_t *p,uint16_t len,uint8_t port)
{	
    
		Ethernet_Reply_Time=0;

}

/*********************************************************************************************************
** Function name:       cmd_103_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_103_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

	 UpstreamCmdHandle(104,port);                          //发送心跳	
}

/*********************************************************************************************************
** Function name:       cmd_0x105_proess
** Descriptions:        解析服务器应答充电桩签到报文
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_105_proess(uint8_t *p,uint16_t len,uint8_t port)
{	
   	uint8_t *pdat;
	
	   pdat = p;	
	   MsaConFlag  = 1;                                      //在线标志位 置位
		 UpstreamCmdHandle(102,port);                          //发送心跳
		
}

/*********************************************************************************************************
** Function name:       cmd_107_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_107_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_109_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_109_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_201_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_201_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_203_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_203_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}

/*********************************************************************************************************
** Function name:       cmd_205_proess
** Descriptions:     
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void cmd_205_proess(uint8_t *p,uint16_t len,uint8_t port)
{	

		
}
