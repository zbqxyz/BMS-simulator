/******************************************************************************
 * @file:    core_cm3.h
 * @purpose: CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 * @version: V1.20
 * @date:    22. May 2009
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-Mx 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef __CM3_CORE_H__
#define __CM3_CORE_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#define __CM3_CMSIS_VERSION_MAIN  (0x01)                                                       /*!< [31:16] CMSIS HAL main version */
#define __CM3_CMSIS_VERSION_SUB   (0x20)                                                       /*!< [15:0]  CMSIS HAL sub version  */
#define __CM3_CMSIS_VERSION       ((__CM3_CMSIS_VERSION_MAIN << 16) | __CM3_CMSIS_VERSION_SUB) /*!< CMSIS HAL version number       */

#define __CORTEX_M                (0x03)                                                       /*!< Cortex core                    */

/**
 *  Lint configuration \n
 *  ----------------------- \n
 *
 *  The following Lint messages will be suppressed and not shown: \n
 *  \n
 *    --- Error 10: --- \n
 *    register uint32_t __regBasePri         __asm("basepri"); \n
 *    Error 10: Expecting ';' \n
 *     \n
 *    --- Error 530: --- \n
 *    return(__regBasePri); \n
 *    Warning 530: Symbol '__regBasePri' (line 264) not initialized \n
 *     \n
 *    --- Error 550: --- \n
 *      __regBasePri = (basePri & 0x1ff); \n
 *    } \n
 *    Warning 550: Symbol '__regBasePri' (line 271) not accessed \n
 *     \n
 *    --- Error 754: --- \n
 *    uint32_t RESERVED0[24]; \n
 *    Info 754: local structure member '<some, not used in the HAL>' (line 109, file ./cm3_core.h) not referenced \n
 *     \n
 *    --- Error 750: --- \n
 *    #define __CM3_CORE_H__ \n
 *    Info 750: local macro '__CM3_CORE_H__' (line 43, file./cm3_core.h) not referenced \n
 *     \n
 *    --- Error 528: --- \n
 *    static __INLINE void NVIC_DisableIRQ(uint32_t IRQn) \n
 *    Warning 528: Symbol 'NVIC_DisableIRQ(unsigned int)' (line 419, file ./cm3_core.h) not referenced \n
 *     \n
 *    --- Error 751: --- \n
 *    } InterruptType_Type; \n
 *    Info 751: local typedef 'InterruptType_Type' (line 170, file ./cm3_core.h) not referenced \n
 * \n
 * \n
 *    Note:  To re-enable a Message, insert a space before 'lint' * \n
 *
 */

/*lint -save */
/*lint -e10  */
/*lint -e530 */
/*lint -e550 */
/*lint -e754 */
/*lint -e750 */
/*lint -e528 */
/*lint -e751 */


#include <stdint.h>                           /* Include standard types */

#if defined (__ICCARM__)
  #include <intrinsics.h>                     /* IAR Intrinsics   */
#endif


#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    4               /*!< standard definition for NVIC Priority Bits */
#endif




/**
 * IO definitions
 *
 * define access restrictions to peripheral registers
 */

#ifdef __cplusplus
#define     __I     volatile                  /*!< defines 'read only' permissions      */
#else
#define     __I     volatile const            /*!< defines 'read only' permissions      */
#endif
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */




#endif /* __CM3_CORE_H__ */

/*lint -restore */
