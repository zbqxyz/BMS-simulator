#ifndef __SSP_H_
#define __SSP_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"

extern void  SSP0_Init(void);
extern  void SSP0_SendData(uint8_t data);
extern  uint8_t  SSP0_ReadData(void );

extern void  SSP1_Init(void);
extern uint16_t  SSP1_SendData(uint8_t data);
extern uint8_t  SSP1_ReadData(void );

extern void  SSP2_Init(void);
extern void  SSP2_SendData(uint8_t data);
extern uint8_t  SSP2_ReadData(void );

#endif
