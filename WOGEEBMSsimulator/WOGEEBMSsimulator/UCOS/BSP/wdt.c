/****************************************Copyright (c)****************************************************
**                                     ueee  Co.,LTD.
**                                     
**                                 http://www.ueee.cn
**                                      wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               wdt.c
** Last modified Date:      2018.05.18
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
#include <wdt.h>

uint8_t WWDT_SetTimeOutRaw(uint32_t timeoutVal)
{
    int8_t retval = 0;

    if(timeoutVal < WWDT_TIMEOUT_MIN) {
        timeoutVal = WWDT_TIMEOUT_MIN;
        retval = -1;
    }
    else if (timeoutVal > WWDT_TIMEOUT_MAX) {
        timeoutVal = WWDT_TIMEOUT_MAX;
        retval = -1;
    }

    LPC_WDT->TC = timeoutVal;

    return retval;
}


uint8_t WWDT_SetTimeOut(uint32_t timeout)
{
    return WWDT_SetTimeOutRaw(WDT_GET_FROM_USEC(timeout));
}


uint8_t WWDT_Init(uint32_t TimeOut)
{
    return WWDT_SetTimeOut(TimeOut);
}


void WWDT_Configure(st_Wdt_Config wdtCfg)
{
    WWDT_SetTimeOut(wdtCfg.wdtTmrConst);

    if(wdtCfg.wdtEnable)  {
        LPC_WDT->MOD |= WWDT_WDMOD_WDEN;
    } else {
        LPC_WDT->MOD &= ~WWDT_WDMOD_WDEN;
    }

    if(wdtCfg.wdtReset) {
        LPC_WDT->MOD |= WWDT_WDMOD_WDRESET;
    } else {
        LPC_WDT->MOD &= ~WWDT_WDMOD_WDRESET;
    }

    if(wdtCfg.wdtProtect) {
        LPC_WDT->MOD |= WWDT_WDMOD_WDPROTECT;
    } else {
        LPC_WDT->MOD &= ~WWDT_WDMOD_WDPROTECT;
    }

    WWDT_SetWarning(wdtCfg.wdtWarningVal);

    WWDT_SetWindow(wdtCfg.wdtWindowVal);
}


uint8_t WWDT_Start(uint32_t TimeOut)
{
    uint8_t retval = 0;

    retval = WWDT_SetTimeOut(TimeOut);

    WWDT_Cmd(ENABLE);

    return retval;
}



void WWDT_SetTimerConstant(uint32_t constVal)
{
    LPC_WDT->TC = constVal;
}



void WWDT_SetMode(uint8_t mode, FunctionalState NewState)
{
    if (mode == WWDT_PROTECT_MODE )
    {
        if(NewState == ENABLE) {
            LPC_WDT->MOD |= WWDT_WDMOD_WDPROTECT;
        } else {
        
            LPC_WDT->MOD &= ~WWDT_WDMOD_WDPROTECT;
        }
    }
    else if(mode == WWDT_RESET_MODE) {
        if(NewState == ENABLE) {
            LPC_WDT->MOD |= WWDT_WDMOD_WDRESET;
        } else {
            LPC_WDT->MOD &= ~WWDT_WDMOD_WDRESET;
        }
    }
}


void WWDT_Enable(FunctionalState NewState)
{
    if(NewState == ENABLE)  {
        LPC_WDT->MOD |= WWDT_WDMOD_WDEN;
    }
    else {
        LPC_WDT->MOD &= ~WWDT_WDMOD_WDEN;
    }
}


void WWDT_Cmd(FunctionalState NewState)
{
    if(NewState == ENABLE) {
        LPC_WDT->MOD |= WWDT_WDMOD_WDEN;
        WWDT_Feed();                                /* Load the Feed register to start using WDT */
    } else {
        LPC_WDT->MOD &= ~WWDT_WDMOD_WDEN;
    }
}


uint8_t WWDT_SetWarningRaw(uint32_t warnVal)
{
    int8_t retval = 0;

    if(warnVal < WWDT_WARNINT_MIN) {
        warnVal = WWDT_WARNINT_MIN;
        retval = -1;
    }
    else if (warnVal > WWDT_WARNINT_MAX) {
        warnVal = WWDT_WARNINT_MAX;
        retval = -1;
    }

    LPC_WDT->WARNINT = warnVal;

    return retval;
}


uint8_t WWDT_SetWarning(uint32_t WarnTime)
{
    return WWDT_SetWarningRaw(WDT_GET_FROM_USEC(WarnTime));
}



uint8_t WWDT_SetWindowRaw(uint32_t wndVal)
{
    int8_t retval = 0;

    if(wndVal < WWDT_WINDOW_MIN)  {
        wndVal = WWDT_WINDOW_MIN;
        retval = -1;
    }
    else if (wndVal > WWDT_WINDOW_MAX) {
        wndVal = WWDT_WINDOW_MAX;
        retval = -1;
    }

    LPC_WDT->WINDOW = wndVal;

    return retval;
}


uint8_t WWDT_SetWindow(uint32_t WindowedTime)
{
    return WWDT_SetWindowRaw(WDT_GET_FROM_USEC(WindowedTime));
}

void WDT_UpdateTimeOut(uint32_t TimeOut)
{
    /* check WDPROTECT,
     * if it is enable, wait until the counter is below the value of
     * WDWARNINT and WDWINDOW
     */
    if(LPC_WDT->MOD & (1<<4)) {
        while((LPC_WDT->TV <(LPC_WDT->WARNINT & WWDT_WDWARNINT_MASK))\
                        &&(LPC_WDT->TV <(LPC_WDT->WINDOW & WWDT_WDTC_MASK)));
    }

    WWDT_SetTimeOut(TimeOut);
}

FlagStatus WWDT_GetStatus (uint8_t Status)
{
    if(Status == WWDT_WARNINT_FLAG)  {
        return ((FlagStatus)(LPC_WDT->MOD & (1<<3)));
    }
    else if (Status == WWDT_TIMEOUT_FLAG) {
        return ((FlagStatus)(LPC_WDT->MOD & (1<<2)));
    }
    return RESET;
}


void WWDT_ClearStatusFlag (uint8_t flag)
{
    if(flag == WWDT_WARNINT_FLAG) {
        LPC_WDT->MOD |= WWDT_WDMOD_WDINT;               /* 清报警标志      */
    }
    else if(flag == WWDT_TIMEOUT_FLAG) {
        LPC_WDT->MOD &= ~ WWDT_WDMOD_WDTOF;             /* 清超时标志      */
    }
}


void WWDT_ClrTimeOutFlag (void)
{
    LPC_WDT->MOD &= ~ WWDT_WDMOD_WDTOF;
}


void WWDT_FeedStdSeq (void)
{
    LPC_WDT->FEED = 0xAA;

    LPC_WDT->FEED = 0x55;
}


void WWDT_Feed (void)
{
    __disable_irq();

    WWDT_FeedStdSeq();

    __enable_irq();
}


uint32_t WWDT_GetCurrentCount(void)
{
    return LPC_WDT->TV;
}

void wdt_entry(void)
{
    WWDT_Init(WDT_TIMEOUT);                                             /* 初始化WDT                    */

    WWDT_Enable(ENABLE);

    WWDT_SetMode(WWDT_RESET_MODE, ENABLE);
    
    WWDT_Start(WDT_TIMEOUT);

    WWDT_Feed();                                                        /* 喂狗                         */   
}
/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/
