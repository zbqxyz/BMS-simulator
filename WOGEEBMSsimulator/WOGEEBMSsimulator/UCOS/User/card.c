/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                 http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               card.c
** Last modified Date:      2018.06.28
** Last Version:            1.0
** Description:             �������ض�����Э��
*********************************************************************************************************/
#include "card.h"
 
#define TOMORN_CARD     1                 //ʹ�ÿ��Ƿ�Ϊ���쿨
 //STX��ETX����
#define MT_STX      0x02
#define MT_ETX      0x03

//�������

#define MT_TYPE_32  0x32
#define MT_TYPE_34  0x34

 //�������
 
#if TOMORN_CARD                   //����M1��

#define MT_TYPE_51                 0x34 //���������
#define MT_CMD_FIND_CARD           0x30 //Ѱ��
#define MT_CMD_READ_CARD_NO        0x31 //��ȡ����
#define MT_CMD_PASSWORD_A          0x32 //��֤����
#define MT_CMD_M1_READ             0x33 //����������
#define MT_CMD_M1_WRITE            0x34 //д��������

#else

#define MT_TYPE_51              0x51 //���������
#define MT_CMD_FIND_CARD        0x37 //Ѱ��	
#define MT_CMD_PRE_PROCESS      0x36 //Ԥ����
#define MT_CMD_START_POWER      0x30 //��ʼ�ӵ�
#define MT_CMD_STOP_POWER       0x34 //�����ӵ�
#define MT_CMD_SUPPLEMENT_TRADE 0x35 //���佻��
#define MT_CMD_VERIFY           0x56 //��֤��Ƭ�Ϸ���
#define MT_CMD_POPUP            0x40 //���� MT610ר��
#define MT_CMD_SIM              0x41 //SIM��λ��ѡ��
#define MT_CMD_CLOSE_RF         0x42 //�ر���Ƶ����


#endif
#define WAIT_MT_RESPONSE_TIMEOUT	(2000)//�ȴ�MTģ��Ӧ��ʱʱ�� ��λ���� �ٷ��Ƽ�2��

#define      BLOCKADDR	17
uint8_t 		RFBUF[BLOCKADDR];


//����״̬��
typedef enum
{
     MT_TIMEOUT = 0x00,//��ʱ
     MT_SUCCESS = 0x59,//�ɹ�
     STATUS_FAIL= 0x4E,//ʧ��
     PSAM_RESET_FAIL= 0x47,//PSAM��λʧ��
     CARD_RESET_FAIL= 0x48,//�û�����λʧ��
     MAC3_FAIL      = 0x43,//MAC3У��ʧ�ܻ�δУ��
     VERIFY_FAIL    = 0x23,//׮����֤����˵����Ƭ������
     NO_CARD        = 0x45//�������޿���ֻ����MT610
}MT_STATUS;

 //�������ؿ��ṹ
 typedef struct 
 {
 #if TOMORN_CARD       
     uint8_t state;      //״̬��P 
     uint8_t UID[4];     //����   
 #else    
     uint8_t S1;        //�Ƿ��п� 0x30�п� 0x31�޿�
     uint8_t S2;        //�Ƿ���ϵͳ�� 0x30ϵͳ�� 0x31��ϵͳ��
     uint8_t ICtype;    //��Ƭ���� 0x01�û���
     uint8_t state;     //����״̬ 0x30������ 0x31�ҿ� 0x32����״̬��û�ж�ȡ
     uint8_t IC[8];     //����
     uint32_t     RM;        //ʣ���� ��λ��
     uint8_t  tserial;   //�ѻ����״���
     uint8_t  serial;    //�������״���
#endif     
 }MT_STR;
 
 //�������ؿ����佻�׼�¼����
typedef struct 
{
     uint8_t IC[8];      //����
     uint8_t ICC[4];     //α�����
     uint8_t serial[2];  //����Ǯ�������������
     uint8_t left[4];    //����Ǯ�����
     uint8_t money[4];   //���ν��׽��
     uint8_t type;       //�������ͱ�ʾ
     uint8_t eterm[6];   //׮���
     uint8_t time[7];    //���ʱ��(�����ص�ʱ��)
     uint8_t tserial[2]; //�ѻ��������
     uint8_t jmac3[4];   //JMAC3
}MT_SUPPLEMENT_TRADE;



/*********************************************************************************************************
** Function name:		MT_BCC
** Descriptions:		����У����
** Input parameters:	str У�����׵�ַ
						len:      len У���ֽڳ���
** Output parameters:	
** Returned value:		����У����
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
** Descriptions:		��������
** Input parameters:	
					
** Output parameters:	pOutdata�����������
** Returned value:		���ճɹ�����1 ���򷵻�0
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
	//��ս�����ز���
	UART4RxbufClear();
	//��������
	UART4SendStr(tmp, i);
}
/*********************************************************************************************************
** Function name:		Wait_MT_Response
** Descriptions:		����-�ȴ�MTӦ��
** Input parameters:	timeout_ms����ʱʱ�� ��λ����
						cmd��ƥ�����������
** Output parameters:	pOutdata�����������
** Returned value:		���ճɹ�����1 ���򷵻�0
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
			
			len = (mUartBuf[1]<<8)|mUartBuf[2];//�õ����ݳ���

			if ((mUartBuf[0]==0x02)&&(mUartBuf[3+len]==0x03))
			{   //STX��ETX��ȷ
				if (calXOR(mUartBuf, len + 4) == mUartBuf[4+len])
				{//У����ȷ
					if (mUartBuf[4] == cmd)
					{//�����������ȷ
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
** Descriptions:		����-Ѱ��
** Input parameters:	pstr�����ṹָ��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_FindCard(MT_STR *pstr)
{
	uint8_t  buf[128],*p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //���ݳ���
	uint8_t  ftype= MT_TYPE_51;                  //CMD������
	uint8_t  fcmd = MT_CMD_FIND_CARD;            //���������

    //�������
    buf[i++] = ftype;
    buf[i++] = fcmd;

    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
//	uint16_t flen = 0x02;                      //���ݳ���
	uint8_t  ftype= MT_TYPE_51;                  //CMD������
	uint8_t  fcmd = MT_CMD_READ_CARD_NO;            //���������

    //�������
    buf[i++] = ftype;
    buf[i++] = fcmd;

    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
//	uint16_t flen = 0x02;                      //���ݳ���
	uint8_t  ftype= MT_TYPE_51;                  //CMD������
	uint8_t  fcmd = MT_CMD_PASSWORD_A;            //���������

    //�������
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = blockNum / 4;
    memset(&buf[i], 0xff, 6);           //Ĭ������
    i += 6;
    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//����Ӧ������
	p = buf;
	
	pstr->state = p[6];

	return MT_SUCCESS;
}
//
MT_STATUS MT_readDat(uint8_t *str, uint8_t blockNum, MT_STR *pstr)
{
    uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;                      //���ݳ���
	uint8_t  ftype= MT_TYPE_51;                  //CMD������
	uint8_t  fcmd = MT_CMD_M1_READ;            //���������

    //�������
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = blockNum / 4;
    buf[i++] = blockNum % 4;

    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
//	uint16_t flen = 0x02;                      //���ݳ���
	uint8_t  ftype= MT_TYPE_51;                  //CMD������
	uint8_t  fcmd = MT_CMD_M1_WRITE;            //���������

    //�������
    buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = blockNum / 4;
    buf[i++] = blockNum % 4;
    
    memcpy(&buf[i], RFBUF, 16);
    i += 16;

    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf, 0, sizeof(buf));
	
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT,fcmd))
	{
	
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
** Descriptions:		����-Ԥ����
** Input parameters:	pstr�����ṹָ��
						ptime��ϵͳʱ��
						pin��pin��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_Preproccess(MT_STR *pstr, DATETime *ptime, uint8_t *pin)
{
#if  !TOMORN_CARD   
	uint8_t  buf[128], *p;
	uint8_t  i = 0; 
	uint8_t  temp;
	//uint16_t flen = 0x0C;                //���ݳ���
	uint8_t  ftype= MT_TYPE_51;            //CMD������
	uint8_t  fcmd = MT_CMD_PRE_PROCESS;    //���������

	//�������
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
	//�ȴ�Ӧ������
	memset(p, 0, sizeof(buf));
	if (!Wait_MT_Response(p, WAIT_MT_RESPONSE_TIMEOUT, fcmd))
	{
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
** Descriptions:		����-��ʼ�ӵ�
** Input parameters:	pstr�����ṹָ��
						ptime��ϵͳʱ��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_StartPower(MT_STR *pstr, DATETime *ptime)
{
#if !TOMORN_CARD    
	uint8_t  buf[128],*p;
    uint8_t  i = 0;
	uint8_t  temp;
//	uint16_t flen = 0x09;//���ݳ���
	uint8_t  ftype = MT_TYPE_51;//CMD������
	uint8_t  fcmd  = MT_CMD_START_POWER;//���������

    //�������
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
	//�ȴ�Ӧ������
	p = buf;
	memset(buf, 0, sizeof(buf));
	if (!Wait_MT_Response(buf, WAIT_MT_RESPONSE_TIMEOUT, fcmd))
	{
		return MT_TIMEOUT;
	}
	//����Ӧ������
	
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
** Descriptions:		����-�����ӵ�
** Input parameters:	pstr�����ṹָ��
						ptime��ϵͳʱ��
						money�����ѽ�� ��λ��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_StopPower(MT_STR *pstr, DATETime *ptime, uint32_t money)
{
#if TOMORN_CARD
	
	  return MT_SUCCESS;
#else
	
	uint8_t  buf[128],*p;
	uint8_t  i = 0;
	uint8_t  temp;
//	uint16_t flen = 0x0D;            //���ݳ���
	uint8_t  ftype= MT_TYPE_51;      //CMD������
	uint8_t  fcmd = MT_CMD_STOP_POWER;//���������

	//�������
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
	//�ȴ�Ӧ������
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
** Descriptions:		����-���佻��
** Input parameters:	pstr�����ṹָ��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_SupplementTrade(MT_SUPPLEMENT_TRADE *pstr)
{
#if !TOMORN_CARD    
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
///	uint16_t flen = 0x02;//���ݳ���
	uint8_t  ftype = MT_TYPE_51;//CMD������
	uint8_t  fcmd  = MT_CMD_SUPPLEMENT_TRADE;//���������

	//�������
    buf[i++] = ftype;
    buf[i++] = fcmd;

    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf, 0, sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
** Descriptions:		����-SIM��λ��ѡ��
** Input parameters:	pstr�����ṹָ��
						SIMx��0x30SIM����1 0x31SIM����2 0x32SIM����3 0x33SIM����4 
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_SelectSIMx(MT_STR *pstr, uint8_t  SIMx)
{
#if  !TOMORN_CARD    
	uint8_t  buf[128];
	uint8_t  i = 0;
//	uint16_t flen=0x03;//���ݳ���
	uint8_t  ftype=MT_TYPE_32;//CMD������
	uint8_t  fcmd=MT_CMD_SIM;//���������

	//�������
	buf[i++] = ftype;
    buf[i++] = fcmd;
    buf[i++] = SIMx;
    
    MT_dataSend(buf, i);

	//�ȴ�Ӧ������
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//����Ӧ������
	//�����ݷ���
#endif
	return MT_SUCCESS;
}

/*********************************************************************************************************
** Function name:		MT_Authentication
** Descriptions:		����-��֤��Ƭ�Ϸ��Բ���ȡ����
** Input parameters:	pstr�����ṹָ��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_Authentication(MT_STR *pstr)
{
#if !TOMORN_CARD
	
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;//���ݳ���
	uint8_t  ftype= MT_TYPE_51;//CMD������
	uint8_t  fcmd = MT_CMD_READ_CARD_NO;//���������

	//�������
	  buf[i++] = ftype;
    buf[i++] = fcmd;
    
    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//����Ӧ������
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
** Descriptions:		����-�ر���Ƶ����
** Input parameters:	��
** Output parameters:	��
** Returned value:		����ֵ��MT_STATUS����
*********************************************************************************************************/
MT_STATUS MT_CloseRF(void)
{
#if !TOMORN_CARD    
	uint8_t  buf[128], *p;
	uint8_t  i = 0;
//	uint16_t flen = 0x02;//���ݳ���
	uint8_t  ftype= MT_TYPE_34;//CMD������
	uint8_t  fcmd = MT_CMD_CLOSE_RF;//���������

	//�������
	buf[i++] = ftype;
    buf[i++] = fcmd;
    
    MT_dataSend(buf, i);
	//�ȴ�Ӧ������
	memset(buf,0,sizeof(buf));
	if(!Wait_MT_Response(buf,WAIT_MT_RESPONSE_TIMEOUT,fcmd)){
		return MT_TIMEOUT;
	}

	//����Ӧ������
	p = buf;
	//�����ݷ���

	return (MT_STATUS)p[5];
#else
    return 0;
#endif    
}
extern const 	uint8_t 	zbyte[];
/*****************************************************************************
 �� �� ��  : MT_readCard
 ��������  : ���ض���
 �������  : uint8_t *str  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��11��
    ��    ��   : Wangj
    �޸�����   : �����ɺ���

*****************************************************************************/
uint8_t MT_readCard(uint8_t *str)
{
    MT_STR pStr;
    DATETime ptim;
    uint8_t pinCode[3] = {0x12, 0x34, 0x56};                      //�û��ܳ�
    uint8_t tmp;
    
		
		 
    if (MT_SUCCESS == MT_FindCard(&pStr))   
    {//Ѱ���ɹ�
#if	TOMORN_CARD  
//		if ('Y' == pStr.state)                                 // �������п�
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
		if (0x30 == pStr.S1)                                 // �������п�
        {			
            if (MT_SUCCESS == MT_Authentication(&pStr))      //��֤��Ƭ�Ϸ��Բ���ȡ���� �ɹ�
            {
                ptim.year = RTCtimedate.year;
                ptim.month= RTCtimedate.month;
                ptim.day  = RTCtimedate.day;
                ptim.hour = RTCtimedate.hour;
                ptim.minute = RTCtimedate.minute;
                ptim.second = RTCtimedate.second;
                
                if (MT_SUCCESS == MT_Preproccess(&pStr, &ptim, pinCode))  //Ԥ����   ȡ������Ϣ
                {   //zbyte[16] = {0x00,0x00,0x00,0x00,1,2,3,4,5,6,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
                    if (0x01 == pStr.ICtype)        //�û������ܼӵ硢�����ŵ�
                    {
                        memcpy(str, zbyte, 16);

                        tmp = pStr.RM / 10000;
                        str[1] = hex2bcd(tmp);
                        tmp = (pStr.RM % 10000)/100;
                        str[2] = hex2bcd(tmp);
                        tmp = pStr.RM % 100;
                        str[3] = hex2bcd(tmp);      //��

                        memcpy(UID, pStr.IC, 8);    //����
											
											 memcpy(FMRevBuffer,str,16)  ;
                        
                        if (0x30 == pStr.state)     //���������Կ�ʼ�ӵ�
                        {
                            str[10] = 0xaa;      //0xaa���Գ��
													  Card_read_flag=0xaa;                 //0��ʼ״̬ AA����־  BB�����־ 
                        }
                        else if (0x31 == pStr.state)
                        {
                            str[10] = 0xab;         //0xab���Խ���
													  Card_read_flag=0xab;                 //0��ʼ״̬ AA����־  BB�����־ 
                        }
                        else 
                        {
                            str[10] = 0xbb;         //��Ч״̬
                        }
                        
                        return 0;
                    }
                }
            }
        }
    #endif			 
    }

    //����
    return 1;
}
/*****************************************************************************
 �� �� ��  : MT_startChg
 ��������  : �������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��12��
    ��    ��   : Wangj
    �޸�����   : �����ɺ���

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
 �� �� ��  : MT_banlance
 ��������  : ����д��
 �������  : uint32_t money  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��12��
    ��    ��   : Wangj
    �޸�����   : �����ɺ���

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
    //����            
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

