/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               calculator.c
** Last modified Date:      2018.08.20
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <calculator.h>
/*********************************************************************************************************
1字节2位BCD数据转换成整数
**********************************************************************************************************/
uint8_t BCD_To_HEX(uint8_t bcd)
{
	return (bcd/16*10 + bcd%16);
}


/*********************************************************************************************************
1字节整数转换成BCD数据
*********************************************************************************************************/
uint8_t HEX_To_BCD(uint8_t hex)
{
	return (hex/10*16 + hex%10);	
	
}	
	
/*********************************************************************************************************
N字节2位BCD数组转换成 1字节32位整数(BCD数组低位在前)
*********************************************************************************************************/
unsigned int NBCD_To_HEX_L(unsigned char *dbuf,unsigned int len)
{	
	unsigned int i,ret=0;
	
	for(i=0;i<len;i++){
		ret = ret*100+BCD_To_HEX(dbuf[len-i-1]);
	}
	return ret;
}

/*********************************************************************************************************
N字节2位BCD数组转换成 1字节32位整数(BCD数组高位在前)
*********************************************************************************************************/
unsigned int NBCD_To_HEX_H(unsigned char *dbuf,unsigned int len)
{	
	unsigned int i,ret=0;

	ret = BCD_To_HEX(dbuf[0]);

	for(i=1;i<len;i++)
	{
	 	ret = ret*100 + BCD_To_HEX(dbuf[i]);
	}
	return ret;
}

/*********************************************************************************************************
** Function name:           CheckSum	   
** Descriptions:            系统参数初始化	
** input parameters:        *start	:	指向数据的指针
**			                  	*end	:	计算的数据的尾指针
** output parameters:       无      
** Returned value:          无 
*********************************************************************************************************/
uint8_t	CheckSum(uint8_t *start, uint8_t *end)
{
	uint8_t	cs;	
	for(cs = 0; start < end; start++){
		cs	+= *start;
	}
	return cs;
}

/*********************************************************************************************************
** Function name:           ASCII_To_HEX	   
** Descriptions:            系统参数初始化	
** input parameters:        len　数据长度
                            dot　小数点位数返回
** output parameters:       无      
** Returned value:          无 
*********************************************************************************************************/
uint32_t ASCII_To_HEX(uint8_t *data,uint8_t len,uint8_t dot)
{
		uint8_t i,cnt=0,ndot=0,dotflag=0;
		uint32_t tradata=0,xx=0;
	  uint8_t dotlen = 0;
		if(len>8)			return tradata;
		for(i=0;i<len;i++)
		{
			if((*(data+i)>=0x30&&*(data+i)<=0x39)||(*(data+i)==0x2e))
			{
				if(*(data+i)!=0x2e)
				{
					if(dotflag)
					{
						dotflag = 0;
						ndot++;
					}
					if(ndot>1)
						return 0xffffffff;
					tradata *=10;
					tradata +=(*(data+i)-0x30);
				}
				else
				{
					cnt++;
					dotlen = i;
					dotflag=1;
				}
			}
			else 
				return 0xffffffff;
		}
		if(cnt>1) 
			return 0xffffffff;
		
	if(dotlen==0) //输入的是整数
	{
		if(dot>0)
		{
			xx = 1;
			for(i=0;i<dot;i++)
				xx *= 10;
			tradata *= xx;
		}
	}
	else
	{
		len-=dotlen+1;
		if(dot > len)  //输入小数位不够
		{
			dot -= len;
			xx = 1;
			for(i=0;i<dot;i++)
				xx *= 10;
			tradata *= xx;
		}
		else if(dot < len) //输入小数位大于需要的，则除去
		{
			len -= dot;
			xx = 1;
			for(i=0;i<len;i++)
				xx *= 10;
			tradata /= xx;
			
		}
	}
	return tradata;

}

/*********************************************************************************************************
** Function name:           ASCII_To_HEX_Password	   
** Descriptions:            系统参数初始化	
** input parameters:        len　数据长度
                            dot　小数点位数返回
** output parameters:       无      
** Returned value:          无 
*********************************************************************************************************/

uint32_t ASCII_To_HEX_Password(uint8_t type,uint8_t *data,uint8_t len,uint8_t *dotcnt)
{
	uint8_t i,cnt=0;
	uint32_t tradata=0;
	if(type==0)
	{
		if(len>8)
			return tradata;
		for(i=0;i<len;i++)
		{
			if((*(data+i)>=0x30&&*(data+i)<=0x39)||(*(data+i)==0x2e))
			{
				if(*(data+i)!=0x2e)
				{
					tradata *=10;
					tradata +=(*(data+i)-0x30);
				}
				else
					cnt++;
			}
			else 
				return 0;
		}
		if(cnt>1) 
			return 0;
	}
	else 
	return tradata;
	
}


/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16进制数转换为ASCII码
* Input			: 
* Output		: 
* Note(s)		: 注意，转化的ascii数组最多是十个字节
***********************************************************************************************/
void  HEX_TO_ASCII(uint32_t hexdata,uint8_t *data)
{
	uint32_t temp[10]={0};
	uint32_t hex;
	uint8_t i,j,k;
	uint32_t chushu=10;
	hex=hexdata	;
	if(hex<0xffffffff)
	{
		for(i=0;i<10;i++)
		{
			chushu=10;
			if(i==9)
			{
				for(j=0;j<i-1;j++)
					chushu*=10;
			    temp[9-i] = hex/chushu;
				temp[9-i]+='0';	
			}
			else
			{				
				for(j=0;j<i;j++)
					chushu*=10;
				temp[9-i] = hex%((uint32_t)chushu);
				hex-=temp[9-i];
				for(k=0;k<i;k++)
					temp[9-i]=temp[9-i]/10;
				temp[9-i]+='0';
			}
		}
	}
	for(i=0;i<10;i++)
		data[i]=(uint8_t)temp[i];
}


/**************************************************************************************************
**	函数名称:	MODBUS_CRC()
**	功	能	:	CRC16计算
**	入口参数:	*pc		:	指向计算数据的指针
**				Length	：	数据队列的长度
**	出口参数:	
**  返回值  ： 	计算出的CRC16的值
**************************************************************************************************/
uint8_t  const CRC_TABLE_H[256] = 
{

0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
};

uint8_t  const CRC_TABLE_L[256] =
{
0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,0x07,0xC7,0x05,0xC5,0xC4,0x04,
0xCC,0x0C,0x0D,0xCD,0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,0x08,0xC8,
0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,
0x14,0xD4,0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,0x11,0xD1,0xD0,0x10,
0xF0,0x30,0x31,0xF1,0x33,0xF3,0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,0x3B,0xFB,0x39,0xF9,0xF8,0x38,
0x28,0xE8,0xE9,0x29,0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,0xEC,0x2C,
0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,
0xA0,0x60,0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,0xA5,0x65,0x64,0xA4,
0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,
0xB4,0x74,0x75,0xB5,0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,0x70,0xB0,
0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,
0x9C,0x5C,0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,0x99,0x59,0x58,0x98,
0x88,0x48,0x49,0x89,0x4B,0x8B,0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,0x43,0x83,0x41,0x81,0x80,0x40,
}; 

uint16_t  MODBUS_CRC(uint8_t  *pc, uint32_t  Length)                   //input:    *p--->First Data 
{
	uint8_t  i ;
	uint8_t  R0 = 0xff ;                                                 //Set CRC16 High
	uint8_t  R1 = 0xff ;                                                 //Set CRC16 Low

	do
	{
		i	= *pc ^ R1 ;
		pc++;
		R1	= R0 ^ CRC_TABLE_H[i];
		R0	= CRC_TABLE_L[i] ;
	}while(--Length !=  0) ;

	return(R0 * 256 + R1);
}


/**************************************************************************** 
*  名    称： GetCRC16() 
*  功    能：  获取 CRC校验码 
*  入口参数： *pBuf:计算 CRC 的数据区,size:计算大小 crc16:校验码  
*  出口参数：  返回 CRC16 的校验码  
****************************************************************************/ 
uint16_t Crc16Tab[256] = 
{ 
  0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 
  0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 
  0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,   
  0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 
  0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,   
  0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,   
  0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 
  0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 
  0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 
  0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,   
  0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,   
  0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 
  0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,   
  0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 
  0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,   
  0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 
  0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,   
  0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,   
  0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 
  0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,   
  0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 
  0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 
  0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,   
  0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,   
  0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,   
  0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 
  0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,     
  0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 
  0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 
  0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 
  0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 
  0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};

uint16_t GetCRC16(uint8_t *pBuf, uint32_t size, uint16_t crc16) 
{ 
	uint16_t c, tmp; 
	uint32_t i; 
 
	for(i = 0; i < size; i++) 
	{ 
		c = pBuf[i]; 
		tmp = crc16^c; 
		crc16=(crc16>>8)^Crc16Tab[tmp & 0xff]; 
	} 
	return crc16; 
} 

/*********************************************************************************************************
ACS数据帧转换成HEX数组(限BCD数字码)
*********************************************************************************************************/
void AscToHexStr(uint8_t *ASCbuf,uint8_t *HEXbuf,uint8_t len)
{
	unsigned int i;
	
	for(i=0;i<len/2;i++)	{
	  	*(HEXbuf+i) = ((*(ASCbuf+2*i)-'0')<<4) + (*(ASCbuf+2*i+1)-'0');
	}
}

/*********************************************************************************************************
HEX数组转换成ACS数据帧(限BCD数字码)
*********************************************************************************************************/
void HexToAscStr(uint8_t *HEXbuf,uint8_t *ASCbuf,uint8_t len)
{
	unsigned int i;
		for(i=0;i<len*2;i+=2)	{	  
			*(ASCbuf+i)   = (*(HEXbuf+i/2)>>4) + '0';
			*(ASCbuf+i+1) = (*(HEXbuf+i/2)%16)  + '0';			
	}	
}
