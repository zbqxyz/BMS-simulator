#ifndef __LINK_H_
#define __LINK_H_
#include "stdlib.h"
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include <ucos_ii.h>
#include <welink.h>
#include <rtc.h>
#include <calculator.h>



extern uint16_t cmd_02_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_04_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_06_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_08_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_10_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_102_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_104_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_106_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_108_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_110_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_202_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_204_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_206_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_302_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_304_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_402_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1002_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1004_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1006_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1008_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1010_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1012_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1102_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1104_upload(uint8_t *pbgn,uint8_t port);
extern uint16_t cmd_1202_upload(uint8_t *pbgn,uint8_t port);


extern void cmd_1_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_3_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_5_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_7_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_9_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_101_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_103_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_105_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_107_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_109_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_201_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_203_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_205_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_301_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_303_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_401_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1001_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1003_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1005_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1007_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1009_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1011_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1101_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1103_proess(uint8_t *p,uint16_t len,uint8_t port);
extern void cmd_1201_proess(uint8_t *p,uint16_t len,uint8_t port);


#endif

