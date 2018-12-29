/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Fram.c
** Last modified Date:      2018.09.08
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <fram.h>

/*********************************************************************************************************
** Function name:		Fdelay
** Descriptions:		FRAM读写延时程序
** input parameters:	无
** output parameters:	无
**********************************************************************************************************/
void Fdelay(INT16U dely)
{
   volatile uint32_t i;
   for(i=dely;i>0;i--);
}
/*********************************************************************************************************
** Functoin name:               FramState()
** Descriptions:  	            查询FRAM的状态
** input paraments:	          *dataptr	:	指向所要发送的数据的指针
**				                     addr		  :	FRAM的起始存储地址
**				                    length		:	数据长度
** output paraments:	       	无	
** Returned values:	   				器件的状态
*********************************************************************************************************/
uint8_t FramState(void)
{	
	uint8_t state;	
	FRAM_CS_L();
	Fdelay(5) ;
	SSP0_SendData(CMD_FRAM_RD_STATUS);   //读器件状态
	state= SSP0_ReadData();	
	Fdelay(5) ;
	FRAM_CS_H();
	return(state);
}
/*********************************************************************************************************
** Functoin name:               FramRead()
** Descriptions:  	            向FRAM写数据
** input paraments:	          *dataptr	:	指向所要发送的数据的指针
**				                     addr		  :	FRAM的起始存储地址
**				                    length		:	数据长度
** output paraments:	    	无	
** Returned values:	   			无
*********************************************************************************************************/
void	FramRead(uint8_t *dataptr,uint16_t addr,uint16_t length)
{	
	uint16_t i;
	
  FRAM_CS_L();
	Fdelay(5) ;
	SSP0_SendData(CMD_FRAM_RD_MEMORY);
	SSP0_SendData((addr >> 8) & 0xff);
	SSP0_SendData(addr & 0xff);	
	for (i = 0; i < length; i++)
	{
		*dataptr	= SSP0_ReadData();
		 dataptr++;
	}
	Fdelay(5) ;
	FRAM_CS_H();
	
}
/*********************************************************************************************************
** Functoin name:               FramWrite()
** Descriptions:  	            向FRAM写数据
** input paraments:	          *dataptr	:	指向所要发送的数据的指针
**				                     addr		  :	FRAM的起始存储地址
**				                    length		:	数据长度
** output paraments:	    	无	
** Returned values:	   			无
*********************************************************************************************************/
void FramWrite(uint8_t *dataptr,uint16_t addr,uint16_t length)
{	
	uint16_t i;
	uint8_t	err;

	err	= FramState();	
	if((err & 0x71) == 0)
	{
		FRAM_CS_L();
		Fdelay(5) ;
		SSP0_SendData(CMD_FRAM_WR_ENABLE);
		Fdelay(5) ;		
		FRAM_CS_H();
		Fdelay(5) ;	
		while((FramState() & 0x80) != 0);
		FRAM_CS_L();
		Fdelay(5) ;
		SSP0_SendData(CMD_FRAM_WR_MEMORY);
		SSP0_SendData((addr >> 8) & 0xff);
		SSP0_SendData(addr & 0xff);
		for (i = 0; i < length; i++)
		{	
			SSP0_SendData(*dataptr);
			dataptr	+= 1;
		}
		Fdelay(5) ;	
		FRAM_CS_H();	
	}
}
