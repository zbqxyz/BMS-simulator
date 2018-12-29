#include <stdint.h>
#include "lpc177x_8x_i2c.h"
#include "debug_frmwrk.h"
#include "UDA1380.h"

#define UDA1380_I2C		I2C_0

I2C_M_SETUP_Type i2cData;

uint8_t i2c_tx_Buf[128];
uint8_t i2c_rx_Buf[128];

/* ³õÊ¼»¯UDA1380 */
int  UDA1380_Init(void)
{  
	// Config Pin for I2C_SDA and I2C_SCL of I2C0
	PINSEL_ConfigPin (0, 27, 1);
	PINSEL_ConfigPin (0, 28, 1);
	
	I2C_Init(UDA1380_I2C, 100000);

	/* Enable I2C0 operation */
	I2C_Cmd(UDA1380_I2C, ENABLE);

	i2cData.sl_addr7bit = UDA1380_I2CADDR >> 1;
	i2cData.tx_data = i2c_tx_Buf;
//	i2cData.tx_length = 0;
	i2cData.rx_data = i2c_rx_Buf;
	i2cData.rx_length = 0;
	i2cData.retransmissions_max = 3;

	UDA1380_Reset();

	/*
	* Setup default values for the system settings 
	*/
	
	/* evaluation modes and clock settings */
	i2c_tx_Buf[0] = UDA1380_REG_CLOCKS;
	i2c_tx_Buf[1] = 0x0F; /* EN_DEC, EN_INT */
	i2c_tx_Buf[2] = 0x08; //0x02; /* PLL1 */
	
	/* I2S input and output settings */
	i2c_tx_Buf[3] = 0x00;
	i2c_tx_Buf[4] = 0x00;//0x10;
	
	/* power control */
	i2c_tx_Buf[5] = 0xA5;
	i2c_tx_Buf[6] = 0xDF;
	
	/* analog mixer settings */
	i2c_tx_Buf[7]  = 0x22;  /* left most volume attenuation */
	i2c_tx_Buf[8]  = 0x22;  /* right most volume attenuation */
	
	i2cData.tx_length = 9;

	if (I2C_MasterTransferData(UDA1380_I2C, &i2cData, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		_DBG("Init UDA1380 registers step 1 OK!\n\r");
	}
	else
	{
		_DBG("Init UDA1380 registers  step 1 Failed!\n\r");
	}
	
	/*
	* Interpolation filter
	*/
	
	/* master volume control */
	i2c_tx_Buf[0] = UDA1380_REG_MASTER_VOL;
	i2c_tx_Buf[1] = 0x40; /* right channel volume attenuation */
	i2c_tx_Buf[2] = 0x40; /* left channel volume attenuation */
	
	/* mixer volume control */
	i2c_tx_Buf[3]  = 0x00;//0xff;	/* channel 2 volume attenuation */
	i2c_tx_Buf[4]  = 0x00;	/* channel 1 volume attenuation */
	
	/* mode selection */
	i2c_tx_Buf[5]  = 0x00;
	i2c_tx_Buf[6]  = 0x00;
	
	/* master mute */
	i2c_tx_Buf[7]  = 0x00;
	i2c_tx_Buf[8]  = 0x00;
	
	i2c_tx_Buf[9]  = 0x10;
	i2c_tx_Buf[10]  = 0x00;
	
	i2cData.tx_length = 11;

	if (I2C_MasterTransferData(UDA1380_I2C, &i2cData, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		_DBG("Init UDA1380 registers step 2 OK!\n\r");
	}
	else
	{
		_DBG("Init UDA1380 registers  step 2 Failed!\n\r");
	}
	
	/*
	* Decimator
	*/
	
	/* decimator volume control */
	i2c_tx_Buf[0]  = UDA1380_REG_DEC_VOL;
	i2c_tx_Buf[1]  = 0x00; /* ADC volume control left */
	i2c_tx_Buf[2]  = 0x00; /* ADC volume control rigth */
	
	/* PGA settings and mute */
	i2c_tx_Buf[3]  = 0x00;
	i2c_tx_Buf[4]  = 0x00;
	
	/* ADC settings */
	i2c_tx_Buf[5]  = 0x00;
	i2c_tx_Buf[6]  = 0x0f;
	
	/* AGC settings */
	i2c_tx_Buf[7]  = 0x00;
	i2c_tx_Buf[8]  = 0x00;
	
	i2cData.tx_length = 9;
	if (I2C_MasterTransferData(UDA1380_I2C, &i2cData, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		_DBG("Init UDA1380 registers step 3 OK!\n\r");
	}
	else
	{
		_DBG("Init UDA1380 registers  step 3 Failed!\n\r");
	}
}

void UDA1380_Reset(void)
{
	i2c_tx_Buf[0] = UDA1380_REG_RESET;
	i2c_tx_Buf[1] = 0xff;
	i2c_tx_Buf[2] = 0xff;

	i2cData.tx_length = 3;
	
	if (I2C_MasterTransferData(UDA1380_I2C, &i2cData, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		_DBG("UDA1380 Soft Reset OK!\n\r");
	}
	else
	{
		_DBG("UDA1380 Soft Reset Failed!\n\r");
	}
}


/*********************************************************************************************************
  End Of File
********************************************************************************************************/
