#ifndef __UDA1380_H__
#define __UDA1380_H__

extern int UDA1380_Init(void);
extern void UDA1380_Reset(void);

#define UDA1380_I2CADDR		(0x30)
#define UDA1380_RETFUNC_OK					(0)
#define UDA1380_RETFUNC_NOT_INITIALIZED		(-1)
#define UDA1380_RETFUNC_FAILED_OP			(-2)

// Define all UDA1380 internal registers
#define UDA1380_REG_CLOCKS   (0x00)
#define UDA1380_REG_I2S      (0x01)
#define UDA1380_REG_PWR_CTRL (0x02)
#define UDA1380_REG_ANALOG   (0x03)
#define UDA1380_REG_HPHONE   (0x04)

#define UDA1380_REG_MASTER_VOL  (0x10)
#define UDA1380_REG_MIXER_VOL   (0x11)
#define UDA1380_REG_MODE_SEL    (0x12)
#define UDA1380_REG_MASTER_MUTE (0x13)
#define UDA1380_REG_MIXER_SET   (0x14)

#define UDA1380_REG_DEC_VOL     (0x20)
#define UDA1380_REG_PGA_SET     (0x21)
#define UDA1380_REG_ADC_SET     (0x22)
#define UDA1380_REG_AGC_SET     (0x23)

#define UDA1380_REG_RESET    (0x7F)

#define L3_DEVICEADDR	0x60
#define I2C_DEVICEADDR

#define DEVICEADRESS0	0x30
#define DEVICEADRESS1	0x34

#define WRITE_MODE_MASK 0x11111110
#define READ_MODE_MASK  0x00000001

//some definition for masks

#define EN_ADC			(1<<11)
#define EN_DEC			(1<<10)
#define EN_DAC			(1<<9)
#define EN_INTERPOLATOR	(1<<8)
#define ADC_CLK			(1<<5)
#define DAC_CLK			(1<<4)
#define PLL_25K_TO_50K  (2<<0)

#define PON_ADCR		(1<<0)
#define PON_PGAR		(1<<1)
#define PON_ADCL		(1<<2)
#define PON_PGAL		(1<<3)
#define PON_LNA			(1<<4)
#define PON_AVC			(1<<6)
#define PON_EN_AVC		(1<<7)
#define PON_BIAS		(1<<8)
#define PON_DAC			(1<<10)
#define PON_HP			(1<<13)
#define PON_PLL			(1<<15)

#define I2SO_I2S_FMT		(0<<0)
#define I2SI_I2S_FMT		(0<<8)
#define I2SO_MSB_JUSTIFIED	(5<<0)
#define I2SI_MSB_JUSTIFIED	(5<<8)
#define UDA1380_SIM (1<<4)	//0: Data output from UDA to uController is slave, 1: master
#define SEL_SOURCE_DIGITAL_MIX	(1<<6)

#define MUTE_CH1	(1<<3)
#define MUTE_CH2	(1<<11)
#define MUTE_MASTER	(1<<14)

#define SDET_ON		(1<<6)
#define SD_3200		(0<<4)
#define OS_SINGLE	(0<<0)
#define MIX_AFTER   (3<<12)

//UDA1380_ADC (0x22)
#define SEL_MIC	(1<<2)
#define SEL_LNA (1<<3)  //Set the SEL_MIC bit to zero to use this, or you'll get the line in on the left channel duplicated on the right channel
#define EN_DCFIL (1)	//Must set 0 to SKIP_DCFIL bit (1)
#define SKIP_DCFIL (1<<1) //must set 0 to EN_DCFIL bit (0)

extern void UDA1380_MasterVolCtrl (uint8_t left, uint8_t right);

#endif
