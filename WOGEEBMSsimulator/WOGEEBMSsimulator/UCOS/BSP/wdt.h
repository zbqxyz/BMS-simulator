#ifndef __WDT_H_
#define __WDT_H_
#include "LPC177x_8x.h"
#include "lpc_types.h"

#define WDT_TIMEOUT     200000                                         /* ��ʱ��ֵΪ2��                */

#define WDT_GET_FROM_USEC(time)        (time/((WWDT_US_INDEX * 4)/WDT_OSC))
#define WDT_GET_USEC(counter)          (counter * ((WWDT_US_INDEX * 4)/WDT_OSC))

#define WWDT_WDMOD_WDEN             ((uint32_t)(1<<0))   /* WWDT interrupt enable bit     */

#define WWDT_WDMOD_WDRESET          ((uint32_t)(1<<1))    /* WWDT interrupt enable bit     */

#define WWDT_WDMOD_WDTOF            ((uint32_t)(1<<2))      /* WWDT time out flag bit        */

#define WWDT_WDMOD_WDINT            ((uint32_t)(1<<3))      /* WDT Time Out flag bit         */

#define WWDT_WDMOD_WDPROTECT        ((uint32_t)(1<<4))      /* WWDT Protect flag bit         */

                                                            /* Define divider index for microsecond ( us ) */
#define WWDT_US_INDEX        ((uint32_t)(1000000))


#define WWDT_TIMEOUT_MIN    ((uint32_t)(0xFF))              /* WWDT Time out minimum value   */

#define WWDT_TIMEOUT_MAX    ((uint32_t)(0x00FFFFFF))        /* WWDT Time out maximum value   */


#define WWDT_WARNINT_MIN    ((uint32_t)(0xFF))              /* WWDT Warning minimum value    */

#define WWDT_WARNINT_MAX    ((uint32_t)(0x000003FF))        /* WWDT Warning maximum value    */


#define WWDT_WINDOW_MIN     ((uint32_t)(0xFF))              /* WWDT Windowed minimum value   */

#define WWDT_WINDOW_MAX     ((uint32_t)(0x00FFFFFF))        /* WWDT Windowed minimum value   */

#define WWDT_WDTC_MASK      ((uint32_t)(0x00FFFFFF))        /* WWDT timer constant register mask */

#define WWDT_WDWARNINT_MASK   ((uint32_t)(0x000003FF))      /* WWDT warning value register mask  */

#define WWDT_WDFEED_MASK         ((uint32_t)(0x000000FF))   /* WWDT feed sequence register mask  */


#define WWDT_WARNINT_FLAG        ((uint8_t)(0))             /* WWDT flag                         */
#define WWDT_TIMEOUT_FLAG        ((uint8_t)(1))

                                                            /* WWDT mode definitions             */
#define WWDT_PROTECT_MODE        ((uint8_t)(0))
#define WWDT_RESET_MODE            ((uint8_t)(1))


                                                            /* WWDT Timer value definition (us) */
#define WWDT_TIMEOUT_USEC_MIN            ((uint32_t)(WDT_GET_USEC(WWDT_TIMEOUT_MIN)))
#define WWDT_TIMEOUT_USEC_MAX            ((uint32_t)(WDT_GET_USEC(WWDT_TIMEOUT_MAX)))

#define WWDT_TIMEWARN_USEC_MIN            ((uint32_t)(WDT_GET_USEC(WWDT_WARNINT_MIN)))
#define WWDT_TIMEWARN_USEC_MAX            ((uint32_t)(WDT_GET_USEC(WWDT_WARNINT_MAX)))

#define WWDT_TIMEWINDOWED_USEC_MIN        ((uint32_t)(WDT_GET_USEC(WWDT_WINDOW_MIN)))
#define WWDT_TIMEWINDOWED_USEC_MAX        ((uint32_t)(WDT_GET_USEC(WWDT_WINDOW_MAX)))

typedef struct Wdt_Config
{
    uint8_t wdtEnable;                                 /* if ENABLE -> the enable bit is enabled       */
    uint8_t wdtReset;                                  /* if ENABLE -> the Reset bit is enabled        */
    uint8_t wdtProtect;                                /* if ENABLE -> the Protect bit is enabled      */
    uint32_t wdtTmrConst;                              /* Set the constant value to timeout the WDT    */
    uint32_t wdtWarningVal;                            /* Set the value to warn the WDT with interrupt */
    uint32_t wdtWindowVal;                             /* Set a window vaule for WDT                   */
}st_Wdt_Config;

uint8_t WWDT_Init(uint32_t TimeOut);
uint8_t WWDT_Start(uint32_t TimeOut);
void WWDT_SetMode(uint8_t mode, FunctionalState NewState);
void WWDT_SetTimerConstant(uint32_t constVal);
void WWDT_Enable(FunctionalState NewState);
void WWDT_Cmd(FunctionalState NewState);
uint8_t WWDT_SetWarningRaw(uint32_t warnVal);
uint8_t WWDT_SetWarning(uint32_t WarnTime);
uint8_t WWDT_SetWindowRaw(uint32_t wndVal);
uint8_t WWDT_SetWindow(uint32_t WindowedTime);
void WWDT_UpdateTimeOut(uint32_t TimeOut);
FlagStatus WWDT_GetStatus (uint8_t Status);
void WWDT_ClearStatusFlag (uint8_t flag);
void WWDT_ClrTimeOutFlag (void);
void WWDT_FeedStdSeq (void);
void WWDT_Feed (void);
uint32_t WWDT_GetCurrentCount(void);



#endif

