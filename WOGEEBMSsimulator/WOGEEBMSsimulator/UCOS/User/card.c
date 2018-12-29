/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                 http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               card.c
** Last modified Date:      2018.06.28
** Last Version:            1.0
** Description:             改自铭特读卡器协议
*********************************************************************************************************/
#include "card.h"
 
#define TOMORN_CARD     1                 //使用卡是否为明天卡
 //STX和ETX定义
#define MT_STX      0x02
#define MT_ETX      0x03

//命令类别

#define MT_TYPE_32  0x32
#define MT_TYPE_34  0x34

 //命令参数
 
#if TOMORN_CARD                   //明天M1卡

#define MT_TYPE_51                 0x34 //命令控制字
#define MT_CMD_FIND_CARD           0x30 //寻卡
#define MT_CMD_READ_CARD_NO        0x31 //读取卡号
#define MT_CMD_PASSWORD_A          0x32 //验证密码
#define MT_CMD_M1_READ             0x33 //读扇区数据
#define MT_CMD_M1_WRITE            0x34 //写扇区数据

#else

#define MT_TYPE_51              0x51 //命令控制字
#define MT_CMD_FIND_CARD        0x37 //寻卡	
#define MT_CMD_PRE_PROCESS      0x36 //预处理
#define MT_CMD_START_POWER      0x30 //开始加电
#define MT_CMD_STOP_POWER       0x34 //结束加电
#define MT_CMD_SUPPLEMENT_TRADE 0x35 //补充交易
#define MT_CMD_VERIFY           0x56 //验证卡片合法性
#define MT_CMD_POPUP            0x40 //弹卡 MT610专用
#define MT_CMD_SIM              0x41 //SIM卡位置选择
#define MT_CMD_CLOSE_RF         0x42 //关闭射频天线


#endif
#define WAIT_MT_RESPONSE_TIMEOUT	(2000)//等待MT模块应答超时时间 单位毫秒 官方推荐2秒

#define      BLOCKADDR	17
uint8_t 		RFBUF[BLOCKADDR];


//返回状态字
typedef enum
{
     MT_TIMEOUT = 0x00,//超时
     MT_SUCCESS = 0x59,//成功
     STATUS_FAIL= 0x4E,//失败
     PSAM_RESET_FAIL= 0x47,//PSAM复位失败
     CARD_RESET_FAIL= 0x48,//用户卡复位失败
     MAC3_FAIL      = 0x43,//MAC3校验失败或未校验
     VERIFY_FAIL    = 0x23,//桩体认证错误，说明卡片不适配
     NO_CARD        = 0x45//卡机内无卡，只适用MT610
}MT_STATUS;

 //定义铭特卡结构
 typedef struct 
 {
 #if TOMORN_CARD       
     uint8_t state;      //状态字P 
     uint8_t UID[4];     //卡号   
 #else    
     uint8_t S1;        //是否有卡 0x30有卡 0x31无卡
     uint8_t S2;        //是否是系统卡 0x30系统卡 0x31非系统卡
     uint8_t ICtype;    //卡片类型 0x01用户卡
     uint8_t state;     //灰锁状态 0x30正常卡 0x31灰卡 0x32灰锁状态字没有读取
     uint8_t IC[8];     //卡号
     uint32_t     RM;        //剩余金额 单位分
     uint8_t  tserial;   //脱机交易次数
     uint8_t  serial;    //联机交易次数
#endif     
 }MT_STR;
 
 //定义铭特卡补充交易记录类型
typedef struct 
{
     uint8_t IC[8];      //卡号
     uint8_t ICC[4];     //伪随机数
     uint8_t serial[2];  //电子钱包联机交易序号
     uint8_t left[4];    //电子钱包金额
     uint8_t money[4];   //本次交易金额
     uint8_t type;       //交易类型标示
     uint8_t eterm[6];   //桩体号
     uint8_t time[7];    //解扣时间(结束回电时间)
     uint8_t tserial[2]; //脱机交易序号
     uint8_t jmac3[4];   //JMAC3
}MT_SUPPLEMENT_TRADE;



/*********************************************************************************************************
** Function name:		MT_BCC
** Descriptions:		计算校验码
** Input parameters:	str 校验区首地址
						len:      len 校验字节长度
** Output parameters:	
** Returned value:		返回校验码
*********************************************************************************************************/
static uint8_t calXOR(uint8_t const *str, uint16_t len)
{
    uint8_t sum = 0;

    while (len--)
    {
        sum ^= *str++;
    }
    
    return sum;
}
/*********************************************************************************************************
** Function name:		MT_dataSend
** Descriptions:		发送数据
** Input parameters:	
					
** Output parameters:	pOutdata：输出缓冲区
** Returned value:		接收成功返回1 否则返回0
*********************************************************************************************************/
static void MT_dataSend(uint8_t const *str, uint16_t len)
{
    uint8_t  tmp[256];
    uint8_t  temp;
    uint16_t i = 0;

    tmp[i++] = MT_STX;
    tmp[i++] = len/256;
    tmp[i++] = len%256;
    memcpy(&tmp[i], str, len);
    i += len;
    tmp[i++] = MT_ETX;
    temp = calXOR(tmp, i);
    tmp[i++] = temp;
	//清空接收相关参数
	UART4RxbufClear();
	//发送数据
	UART4SendStr(tmp, i);
}
/*********************************************************************************************************
** Function name:		Wait_MT_Response
** Descriptions:		铭特-等待MT应答
** Input parameters:	timeout_ms：超时时间 单位毫秒
						cmd：匹配命令参数字
** Output parameters:	pOutdata：输出缓冲区
** Returned value:		接收成功返回1 否则返回0
*********************************************************************************************************/
static uint8_t Wait_MT_Response(uint8_t *pOutdata, uint16_t timeout_ms, uint8_t cmd)
{
	uint8_t ret=0;
	uint16_t t, timelimit, mlen,len;
  uint8_t mUartBuf[1024];
	timelimit = timeout_ms/100+1;

	for (t=0; t<timelimit; t++)
	{
		OSTimeDly(OS_TICKS_PER_SEC/10);
	 
		mlen =RcvLenU0 ;
		
		if (mlen > 4)
		{
			memcpy(mUartBuf,RcvBufU0,mlen);
			
			len = (mUartBuf[1]<<8)|mUartBuf[2];//得到数据长度

			if ((mUartBuf[0]==0x02)&&(mUartBuf[3+len]==0x03))
			{   //STX和ETX正确
				if (calXOR(mUartBuf, len + 4) == mUartBuf[4+len])
				{//校验正确
					if (mUartBuf[4] == cmd)
					{//命令参数字正确
						memcpy(pOutdata, mUartBuf, 5+len);
						ret = 1;
                        break;
					}
				}
			}
		}
	}
	
	return ret;
}

/*********************************************************************************************************
** Function name:		MT_FindCard
** Descriptions:		铭特-寻卡
** Input parameters:	pstr：卡结构指针
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_FindCard(MT_STR *pstr)
{
	uint8_t  buf[128],*p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //数据长度
	uint8_t  ftype= MT_TYPE_51;                  //CMD命令字
	uint8_t  fcmd = MT_CMD_FIND_CARD;            //命令参数字

    //打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;

    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//分析应答数据
	p = buf;
	
#if TOMORN_CARD
	pstr->state = p[5];
#else
	pstr->S1 = p[6];
	pstr->S2 = p[7];
#endif
	return MT_SUCCESS;
}
//
MT_STATUS MT_getCardID(MT_STR *pstr)
{
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //数据长度
	uint8_t  ftype= MT_TYPE_51;                  //CMD命令字
	uint8_t  fcmd = MT_CMD_READ_CARD_NO;            //命令参数字

    //打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;

    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//分析应答数据
	p = buf;
	
#if TOMORN_CARD
	pstr->state = p[5];
    memcpy(pstr->UID, &p[6], 4);
#else
	pstr->S1 = p[6];
	pstr->S2 = p[7];
#endif
	return MT_SUCCESS;
}

//
MT_STATUS MT_verifyPWD(uint8_t blockNum, MT_STR *pstr)
{
    uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //数据长度
	uint8_t  ftype= MT_TYPE_51;                  //CMD命令字
	uint8_t  fcmd = MT_CMD_PASSWORD_A;            //命令参数字

    //打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = blockNum / 4;
    memset(&buf[i], 0xff, 6);           //默认密码
    i += 6;
    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//分析应答数据
	p = buf;
	
	pstr->state = p[6];

	return MT_SUCCESS;
}
//
MT_STATUS MT_readDat(uint8_t *str, uint8_t blockNum, MT_STR *pstr)
{
    uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //数据长度
	uint8_t  ftype= MT_TYPE_51;                  //CMD命令字
	uint8_t  fcmd = MT_CMD_M1_READ;            //命令参数字

    //打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = blockNum / 4;
    buf[i++] = blockNum % 4;

    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//分析应答数据
	p = buf;
	
	pstr->state = p[7];
    if ('Y' == pstr->state)
    {
        memcpy(str, &p[8], 16);
    }
    
	return MT_SUCCESS;
}

//
MT_STATUS MT_writeDat(uint8_t blockNum, MT_STR *pstr)
{
	extern uint8_t RFBUF[];
    uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //数据长度
	uint8_t  ftype= MT_TYPE_51;                  //CMD命令字
	uint8_t  fcmd = MT_CMD_M1_WRITE;            //命令参数字

    //打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = blockNum / 4;
    buf[i++] = blockNum % 4;
    
    memcpy(&buf[i], RFBUF, 16);
    i += 16;

    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//分析应答数据
	p = buf;
	
	pstr->state = p[7];
    if ('Y' == pstr->state)
    {
        if (memcpy(RFBUF, &p[8], 16) != 0)
        {
            //return STATUS_FAIL;
        }
    }
    
	return MT_SUCCESS;
}

/*********************************************************************************************************
** Function name:		MT_Preproccess
** Descriptions:		铭特-预处理
** Input parameters:	pstr：卡结构指针
						ptime：系统时间
						pin：pin码
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_Preproccess(MT_STR *pstr, DATETime *ptime, uint8_t *pin)
{
#if  !TOMORN_CARD   
	uint8_t  buf[128], *p;
	uint8_t  i = 0; 
	uint8_t  temp;
	//uint16_t flen = 0x0C;                //数据长度
	uint8_t  ftype= MT_TYPE_51;            //CMD命令字
	uint8_t  fcmd = MT_CMD_PRE_PROCESS;    //命令参数字

	//打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;
    temp = ptime->year/100;
    buf[i++] = hex2bcd(temp);
	temp = ptime->year%100;
    buf[i++] = hex2bcd(temp);
    buf[i++] = hex2bcd(ptime->month);
    buf[i++] = hex2bcd(ptime->day);
    buf[i++] = hex2bcd(ptime->hour);
    buf[i++] = hex2bcd(ptime->minute);
    buf[i++] = hex2bcd(ptime->second);
    memcpy(&buf[i], pin, 3);
    i += 3;
    
	MT_dataSend(buf, i);
	
	p = buf;
	//等待应答数据
	memset(p, 0, sizeof(buf));
	if (!Wait_MT_Response(p, WAIT_MT_RESPONSE_TIMEOUT, fcmd))
	{
		return MT_TIMEOUT;
	}

	//分析应答数据
	if(p[5] != 0x59)
	{
		return (MT_STATUS)p[5];
	}
	
	pstr->ICtype = p[6];
	pstr->state  = p[7];
	
	memcpy(pstr->IC, &p[8], 8);
	pstr->RM     = (p[16]<<24)| (p[17]<<16)|(p[18]<<8)|p[19];
	pstr->tserial= (p[20]<<8) | p[21];
	pstr->serial = (p[22]<<8) | p[23];
#endif
	return MT_SUCCESS;
}

/*********************************************************************************************************
** Function name:		MT_StartPower
** Descriptions:		铭特-开始加电
** Input parameters:	pstr：卡结构指针
						ptime：系统时间
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_StartPower(MT_STR *pstr, DATETime *ptime)
{
#if !TOMORN_CARD    
	uint8_t  buf[128],*p;
    uint8_t  i = 0;
	uint8_t  temp;
//	uint16_t flen = 0x09;//数据长度
	uint8_t  ftype = MT_TYPE_51;//CMD命令字
	uint8_t  fcmd  = MT_CMD_START_POWER;//命令参数字

    //打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;
    temp = ptime->year/100;
    buf[i++] = hex2bcd(temp);
	temp = ptime->year%100;
    buf[i++] = hex2bcd(temp);
    buf[i++] = hex2bcd(ptime->month);
    buf[i++] = hex2bcd(ptime->day);
    buf[i++] = hex2bcd(ptime->hour);
    buf[i++] = hex2bcd(ptime->minute);
    buf[i++] = hex2bcd(ptime->second);
    
	MT_dataSend(buf, i);
	//等待应答数据
	p = buf;
	memset(buf, 0, sizeof(buf));
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT, fcmd))
	{
		return MT_TIMEOUT;
	}
	//分析应答数据
	
	if (p[5] != 0x59)
	{
		return (MT_STATUS)p[5];
	}
	
	pstr->RM = (p[6]<<24)|(p[7]<<16)|(p[8]<<8)|p[9];
#endif
	return MT_SUCCESS;
}

/*********************************************************************************************************
** Function name:		MT_StopPower
** Descriptions:		铭特-结束加电
** Input parameters:	pstr：卡结构指针
						ptime：系统时间
						money：消费金额 单位分
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_StopPower(MT_STR *pstr, DATETime *ptime, uint32_t money)
{
#if TOMORN_CARD
	
	  return MT_SUCCESS;
#else
	
	uint8_t  buf[128],*p;
	uint8_t  i = 0;
	uint8_t  temp;
//	uint16_t flen = 0x0D;            //数据长度
	uint8_t  ftype= MT_TYPE_51;      //CMD命令字
	uint8_t  fcmd = MT_CMD_STOP_POWER;//命令参数字

	//打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = (money>>24)&0xff;
    buf[i++] = (money>>16)&0xff;
    buf[i++] = (money>> 8)&0xff;
    buf[i++] = (money>> 0)&0xff;
    temp = ptime->year/100;
    buf[i++] = hex2bcd(temp);
	temp = ptime->year%100;
    buf[i++] = hex2bcd(temp);
    buf[i++] = hex2bcd(ptime->month);
    buf[i++] = hex2bcd(ptime->day);
    buf[i++] = hex2bcd(ptime->hour);
    buf[i++] = hex2bcd(ptime->minute);
    buf[i++] = hex2bcd(ptime->second);

	MT_dataSend(buf, i);
	//等待应答数据
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//分析应答数据
	p=buf;
	if(p[5]!=0x59){
		return (MT_STATUS)p[5];
	}
	pstr->RM = (p[6]<<24)|(p[7]<<16)|(p[8]<<8)|p[9];

	return MT_SUCCESS;
#endif
}

/*********************************************************************************************************
** Function name:		MT_SupplementTrade
** Descriptions:		铭特-补充交易
** Input parameters:	pstr：卡结构指针
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_SupplementTrade(MT_SUPPLEMENT_TRADE *pstr)
{
#if !TOMORN_CARD    
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
///	uint16_t flen = 0x02;//数据长度
	uint8_t  ftype = MT_TYPE_51;//CMD命令字
	uint8_t  fcmd  = MT_CMD_SUPPLEMENT_TRADE;//命令参数字

	//打包数据
    buf[i++] = ftype;
    buf[i++] = fcmd;

    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf, 0, sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//分析应答数据
	p=buf;
	memcpy(pstr->IC,&p[6],8);
	memcpy(pstr->ICC,&p[14],4);
	memcpy(pstr->serial,&p[18],2);
	memcpy(pstr->left,&p[20],4);
	memcpy(pstr->money,&p[24],4);
	pstr->type=p[28];
	memcpy(pstr->eterm,&p[29],6);
	memcpy(pstr->time,&p[35],7);
	memcpy(pstr->tserial,&p[42],2);
	memcpy(pstr->jmac3,&p[44],4);

	return (MT_STATUS)p[5];
#else
    return 0;
#endif
        
}

/*********************************************************************************************************
** Function name:		MT_SelectSIMx
** Descriptions:		铭特-SIM卡位置选择
** Input parameters:	pstr：卡结构指针
						SIMx：0x30SIM卡座1 0x31SIM卡座2 0x32SIM卡座3 0x33SIM卡座4 
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_SelectSIMx(MT_STR *pstr, uint8_t  SIMx)
{
#if  !TOMORN_CARD    
	uint8_t  buf[128];
	uint8_t  i = 0;
//	uint16_t flen=0x03;//数据长度
	uint8_t  ftype=MT_TYPE_32;//CMD命令字
	uint8_t  fcmd=MT_CMD_SIM;//命令参数字

	//打包数据
	buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = SIMx;
    
    MT_dataSend(buf, i);

	//等待应答数据
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//分析应答数据
	//无数据分析
#endif
	return MT_SUCCESS;
}

/*********************************************************************************************************
** Function name:		MT_Authentication
** Descriptions:		铭特-验证卡片合法性并读取卡号
** Input parameters:	pstr：卡结构指针
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_Authentication(MT_STR *pstr)
{
#if !TOMORN_CARD
	
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;//数据长度
	uint8_t  ftype= MT_TYPE_51;//CMD命令字
	uint8_t  fcmd = MT_CMD_READ_CARD_NO;//命令参数字

	//打包数据
	  buf[i++] = ftype;
    buf[i++] = fcmd;
    
    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//分析应答数据
	p=buf;
	if(p[5]!=0x59){
		return (MT_STATUS)p[5];
	}
	pstr->ICtype=p[6];
	memcpy(pstr->IC,&p[7], 8);

	return MT_SUCCESS;
	
#else
	

	return MT_SUCCESS;
	
#endif
}
/*********************************************************************************************************
** Function name:		MT_CloseRF
** Descriptions:		铭特-关闭射频天线
** Input parameters:	无
** Output parameters:	无
** Returned value:		返回值见MT_STATUS定义
*********************************************************************************************************/
MT_STATUS MT_CloseRF(void)
{
#if !TOMORN_CARD    
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;//数据长度
	uint8_t  ftype= MT_TYPE_34;//CMD命令字
	uint8_t  fcmd = MT_CMD_CLOSE_RF;//命令参数字

	//打包数据
	buf[i++] = ftype;
    buf[i++] = fcmd;
    
    MT_dataSend(buf, i);
	//等待应答数据
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//分析应答数据
	p = buf;
	//无数据分析

	return (MT_STATUS)p[5];
#else
    return 0;
#endif    
}
extern const 	uint8_t 	zbyte[];
/*****************************************************************************
 函 数 名  : MT_readCard
 功能描述  : 铭特读卡
 输入参数  : uint8_t *str  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月11日
    作    者   : Wangj
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t MT_readCard(uint8_t *str)
{
    MT_STR pStr;
    DATETime ptim;
    uint8_t pinCode[3] = {0x12, 0x34, 0x56};                      //用户密匙
    uint8_t tmp;
    
		
		 
    if (MT_SUCCESS == MT_FindCard(&pStr))   
    {//寻卡成功
#if	TOMORN_CARD  
//		if ('Y' == pStr.state)                                 // 卡机内有卡
//        {			
//             if (MT_SUCCESS == MT_getCardID(&pStr))
//             {
//                if ('Y' == pStr.state)
//                {
//                    memcpy(UID, pStr.UID, 4);
//                    if (MT_SUCCESS == MT_verifyPWD(BLOCKADDR, &pStr))
//                    {
//                        if ('Y' == pStr.state)
//                        {
//                            if (MT_SUCCESS == MT_readDat(str, BLOCKADDR, &pStr))
//                            {
//                                if ('Y' == pStr.state)
//                                {
//                                    return 0;  
//                                }
//                            }
//                        }
//                    }    
//                }
//             }               
//		}			
		
#else   
		if (0x30 == pStr.S1)                                 // 卡机内有卡
        {			
            if (MT_SUCCESS == MT_Authentication(&pStr))      //验证卡片合法性并读取卡号 成功
            {
                ptim.year = RTCtimedate.year;
                ptim.month= RTCtimedate.month;
                ptim.day  = RTCtimedate.day;
                ptim.hour = RTCtimedate.hour;
                ptim.minute = RTCtimedate.minute;
                ptim.second = RTCtimedate.second;
                
                if (MT_SUCCESS == MT_Preproccess(&pStr, &ptim, pinCode))  //预处理   取卡卡信息
                {   //zbyte[16] = {0x00,0x00,0x00,0x00,1,2,3,4,5,6,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
                    if (0x01 == pStr.ICtype)        //用户卡才能加电、结束放电
                    {
                        memcpy(str, zbyte, 16);

                        tmp = pStr.RM / 10000;
                        str[1] = hex2bcd(tmp);
                        tmp = (pStr.RM % 10000)/100;
                        str[2] = hex2bcd(tmp);
                        tmp = pStr.RM % 100;
                        str[3] = hex2bcd(tmp);      //分

                        memcpy(UID, pStr.IC, 8);    //卡号
											
											 memcpy(FMRevBuffer,str,16)  ;
                        
                        if (0x30 == pStr.state)     //正常卡可以开始加电
                        {
                            str[10] = 0xaa;      //0xaa可以充电
													  Card_read_flag=0xaa;                 //0初始状态 AA充电标志  BB结算标志 
                        }
                        else if (0x31 == pStr.state)
                        {
                            str[10] = 0xab;         //0xab可以结束
													  Card_read_flag=0xab;                 //0初始状态 AA充电标志  BB结算标志 
                        }
                        else 
                        {
                            str[10] = 0xbb;         //无效状态
                        }
                        
                        return 0;
                    }
                }
            }
        }
    #endif			 
    }

    //错误
    return 1;
}
/*****************************************************************************
 函 数 名  : MT_startChg
 功能描述  : 开启充电
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月12日
    作    者   : Wangj
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t MT_startChg(void)
{
    MT_STR pStr;
    DATETime ptim;
    
    ptim.year = RTCtimedate.year;
    ptim.month= RTCtimedate.month;
    ptim.day  = RTCtimedate.day;
    ptim.hour = RTCtimedate.hour;
    ptim.minute = RTCtimedate.minute;
    ptim.second = RTCtimedate.second;
                
    if (MT_SUCCESS == MT_StartPower(&pStr, &ptim))
    {
        return 1;
    }

    return 0;
}
/*****************************************************************************
 函 数 名  : MT_banlance
 功能描述  : 结算写卡
 输入参数  : uint32_t money  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月12日
    作    者   : Wangj
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t MT_banlance(uint32_t money, uint32_t *result)
{
    MT_STR pStr;
#if !TOMORN_CARD    
    DATETime ptim;
    
    ptim.year = RTCtimedate.year;
    ptim.month= RTCtimedate.month;
    ptim.day  = RTCtimedate.day;
    ptim.hour = RTCtimedate.hour;
    ptim.minute = RTCtimedate.minute;
    ptim.second = RTCtimedate.second;
    //结束            
    if (MT_SUCCESS == MT_StopPower(&pStr, &ptim, money))
    {
        *result = pStr.RM;
        
        return 1;
    }
#else
    if (MT_SUCCESS == MT_writeDat(BLOCKADDR, &pStr))
    {
        if ('Y' == pStr.state)
        {
            return 1;  
        }
    }
#endif
    return 0;
}

