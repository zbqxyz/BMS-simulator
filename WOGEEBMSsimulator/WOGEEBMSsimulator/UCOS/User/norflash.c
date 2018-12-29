/****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                   http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Norflash.c
** Last modified Date:      2018.09.08
** Last Version:            1.0
** Description:             
*********************************************************************************************************/

/********************************************************************************************************
  MX25L1635E    ����31��BLOCK, 512��SECTOR, 8192��page     
                ÿ��BLOCK��С:64K byte    ÿ��SECTOR��С��4K byte    ÿ��page��С��256 byte
                һ��2M�ֽ�,��2048k byte    16M bit
	�洢��ʽ����							
Block Sector   Address Range
----------------------------------
31    511     1FF000h 1FFFFFh   
      :        :      :
			496     1F0000h 1F0FFFh
----------------------------------			
30    495     1EF000h 1EFFFFh 
      :        :      :
			480     1E0000h 1E0FFFh
----------------------------------
29    479     1DF000h 1DFFFFh 
      :        :      :
			464     1D0000h 1D0FFFh
-----------------------------------
28    463     1CF000h 1CFFFFh 
      :        :      :
			448     1C0000h 1C0FFFh
-----------------------------------
       ��      ��     ��
			 ��      ��     ��
-----------------------------------
2     47      02F000h 02FFFFh 
      :        :       :
			32      020000h 020FFFh
-----------------------------------
1     31      01F000h 01FFFFh 
      :        :       :
			16      010000h 010FFFh
-----------------------------------			
0     15      00F000h 00FFFFh 
      :        :       : 
			2       002000h 002FFFh      12k
			1       001000h 001FFFh       8k
			0       000000h 000FFFh       4k
-----------------------------------
*************************************************************************************************/


#include <norflash.h>

/******************************************
*��������� MX25L1635E SPI COMMAND
*******************************************/

/*  
** SPI Flash����(���MX25L1605)  
 */
#define   SPI_FLASH_WREN                  0x06                          /* write enable                 */
#define   SPI_FLASH_WRDI                  0x04                          /* write disable                */
#define   SPI_FLASH_RDID                  0x9F                          /* read identification          */
#define   SPI_FLASH_RDSR                  0x05                          /* read status register         */
#define   SPI_FLASH_WRSR                  0x01                          /* write status register        */
#define   SPI_FLASH_READ                  0x03                          /* read data                    */
#define   SPI_FLASH_FAST_READ             0x0B                          /* fast read data               */
#define   SPI_FLASH_SE                    0x20                          /* sector erase                 */
#define   SPI_FLASH_BE                    0xD8                          /* block erase                  */
#define   SPI_FLASH_CE                    0x60                          /* chip  erase                  */
#define   SPI_FLASH_PP                    0x02                          /* page program                 */
#define   SPI_FLASH_CP                    0xAD                          /* continuously program mode    */
#define   SPI_FLASH_DP                    0xB9                          /* deep power down              */
#define   SPI_FLASH_RDP                   0xAB                          /* release deep power down      */
#define   SPI_FLASH_RES                   0x90                          /* read electronic ID           */
#define   SPI_FLASH_EWSR                  0x50                          /* read electronic manufact-    */
                                                                        /* -urer & device ID            */
#define   SPI_FIFO_DEPTH    	  64

#define   SPI_FLASH_PageSize    256                                       //ҳ��С   256 byte 
#define   SPI_SECTOR_PageSize   4096                                      //������С 4k  byte

#define   MAX_ADDR            0x1FFFFF                                  /* оƬ�ڲ�����ַ             */
#define   SEC_SIZE            0x1000                                    /* ������С                     */
#define   PAGE_SIZE           256                                       /* ������С                     */
#define   MAX_BLOCK_NUM       31                                        /* �����                     */
#define   MAX_SECTOR_NUM   	  511                                       /* ���������                   */ 

#define dlytime  1000
/*********************************************************************************************************
** Function name:		Nordelay
** Descriptions:		Norflash��д��ʱ����
** input parameters:	��
** output parameters:	��
**********************************************************************************************************/
void Nordelay(INT16U dely)
{
   volatile uint32_t i;
   for(i=dely;i>0;i--);
}

//��ȡ״̬�Ĵ���
unsigned char Read_Status_Register(void)
{
	unsigned char status = 0;
	  CLR_CSNOFLASH();	
    Nordelay(500);
    SSP1_SendData(SPI_FLASH_RDSR);
    status = SSP1_SendData(0xff);
	  Nordelay(500);
  	SET_CSNOFLASH();    
	  return status;
} 
/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{

    CLR_CSNOFLASH();
    Nordelay(100);
    /* Send "Write Enable" instruction */
    SSP1_SendData(SPI_FLASH_WREN);
    Nordelay(100);
    /* Deselect the FLASH: Chip Select high */
    SET_CSNOFLASH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status  = 0;
    uint16_t spi_time_out = 100;

    //spi_time_out = 100;
    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        FLASH_Status = Read_Status_Register();
        spi_time_out--;
        if(spi_time_out==0) break;
    }
    while ((FLASH_Status & SPI_FLASH_PP) == SET); /* Write in progress */    
}

 //��������
void NorEraseSector(uint32_t addr)
{
	uint8_t temp;
	
	CLR_CSNOFLASH();
	SSP1_SendData(SPI_FLASH_WREN);			//дʹ��
	SET_CSNOFLASH();
	
	CLR_CSNOFLASH();
	SSP1_SendData(SPI_FLASH_SE);				//��������
	temp=addr>>16;										//������ַ
	SSP1_SendData(temp);
	temp=addr>>8;
	SSP1_SendData(temp);
	temp=addr&0xff;
	SSP1_SendData(temp);
	SET_CSNOFLASH();
	Nordelay(3000);//OSTimeDly(OS_TICKS_PER_SEC/3);	 	//300ms����
}

//��NorFlash
void Read_Norflash(uint8_t *p,uint32_t addr,uint16_t len)
{
    uint32_t i;
    CLR_CSNOFLASH();
    Nordelay(200);	 									//15ms
    SSP1_SendData(SPI_FLASH_FAST_READ);	
	  SSP1_SendData(((addr & 0xFFFFFF) >> 16));	            // ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����
	  SSP1_SendData(((addr & 0xFFFF) >> 8));
	  SSP1_SendData(addr & 0xFF);
	  SSP1_SendData(0xFF);						                      // ����һ�����ֽ��Զ�ȡ����
    
	  for(i=0;i<len;i++)
        *p++ = SSP1_ReadData();
	
    Nordelay(200);	
    SET_CSNOFLASH();
    Nordelay(200);		 						                   			//15ms

}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t i2,i1,i0;
    uint16_t k,i;
    __disable_irq();
    /* Enable the write access to the FLASH */    
    SPI_FLASH_WriteEnable();
    /* Select the FLASH: Chip Select low */
    CLR_CSNOFLASH();
    Nordelay(100);     //OSTimeDly(OS_TICKS_PER_SEC/100);		//100ms  
    /* Send "Write to Memory " instruction */
    SSP1_SendData(SPI_FLASH_PP);				//д����
    //OSTimeDly(OS_TICKS_PER_SEC/100);		//100ms  
    /* Send WriteAddr high nibble address byte to write to */
    SSP1_SendData((WriteAddr >> 16)&0xff);
    SSP1_SendData((WriteAddr >>  8)&0xff);
    SSP1_SendData((WriteAddr >>  0)&0xFF);
    Nordelay(1);
    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        SSP1_SendData(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }
    /* Deselect the FLASH: Chip Select high */
    Nordelay(100);
    SET_CSNOFLASH();
    
    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();  
    Nordelay(dlytime);
    __enable_irq();
}

void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t wLen=NumByteToWrite;
    uint16_t k;
    uint16_t ADDRr;
    uint16_t pageremain;
    //__disable_irq();
    while (wLen > 0)
    {
        ADDRr = WriteAddr % SPI_SECTOR_PageSize;
        if(ADDRr==0)
          {
            NorEraseSector(WriteAddr);
          }     
					
          pageremain = SPI_FLASH_PageSize - WriteAddr%SPI_FLASH_PageSize;                    //��ҳʣ����ֽ���          
        if (wLen > pageremain)  
					  k = pageremain;
        else                    
					  k = wLen;
        
        Nordelay(20);
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, k);                                        //write_norflash_pagedata(pBuffer, WriteAddr, k, 0); 

        WriteAddr += k;
        pBuffer   += k;

        if (wLen >= k) wLen -= k;
        else wLen=0;   
    }
    //__enable_irq();
}

//дNorFlash
uint32_t addrTmp;
uint8_t *paStr;
uint8_t  rdTmp[4096];
void WriteNorflash(uint8_t *p, uint32_t addr, uint16_t len)
{    
    uint8_t	err;
    INT32U temp = 0,i = 0,StatRgVal = 0;
    paStr   = p;
    addrTmp = addr;
    Nordelay(20); 
    SPI_FLASH_BufferWrite(paStr, addrTmp, len);   
    Read_Norflash(rdTmp, addrTmp, len);
    if (memcmp(rdTmp, paStr, len) != 0)
    {
        SPI_FLASH_BufferWrite(paStr, addrTmp, len); 
    }
    Read_Norflash(rdTmp, addrTmp, len);
    if (memcmp(rdTmp, paStr, len) != 0)
    {
        SPI_FLASH_BufferWrite(paStr, addrTmp, len); 
    }	
	  Read_Norflash(rdTmp, addrTmp, len);
	  rdTmp[0] = rdTmp[1];
	
	}

	