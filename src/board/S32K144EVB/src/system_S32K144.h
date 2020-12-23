/*
** ###################################################################
**     Processor:           S32K144_100
**     Compilers:           GNU C Compiler
**                          GreenHills ARM Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    S32K144RM Rev. 1, Draft D, 10/2015
**     Version:             rev. 1.7, 2015-10-21
**     Build:               b151027
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     Copyright 2016 NXP
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2015-04-09) - Iulian Talpiga
**         Initial version.
**     - rev. 1.1 (2015-05-19) - Bogdan Nitu
**         Updated interrupts table
**         Removed SIM_CHIPCTL_DAC2CMP
**         Compacted PORT_PCR registers
**     - rev. 1.2 (2015-09-16) - Iulian Talpiga
**         Implemented SystemCoreClockUpdate
**
** ###################################################################
*/


/*! @addtogroup soc_support_S32K144*/
/*! @{*/

/*!
 * @file S32K144
 * @version 1.2
 * @date 2015-09-16
 * @brief Device specific configuration file for S32K144 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef SYSTEM_S32K144_H_
#define SYSTEM_S32K144_H_                        /**< Symbol preventing repeated inclusion */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * CPU Settings.
 *****************************************************************************/

/* Watchdog disable */
#ifndef DISABLE_WDOG
  #define DISABLE_WDOG                 1
#endif

/* Value of the external crystal or oscillator clock frequency in Hz */
#ifndef CPU_XTAL_CLK_HZ
  #define CPU_XTAL_CLK_HZ                8000000u
#endif

/* Value of the fast internal oscillator clock frequency in Hz  */
#ifndef CPU_INT_FAST_CLK_HZ
  #define CPU_INT_FAST_CLK_HZ            50000000u
#endif

/* Default System clock value */
#ifndef DEFAULT_SYSTEM_CLOCK
 #define DEFAULT_SYSTEM_CLOCK            50000000u
#endif

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit(void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 * This function must be called when user does not want to use clock manager component.
 * If clock manager is used, the CLOCK_SYS_GetFreq function must be used with CORE_CLOCK
 * parameter.
 *
 */
void SystemCoreClockUpdate(void);

/**
 * @brief Software Resets the microcontroller system.
 *
 * This function is used to software reset the microcontroller
 */
void SystemSoftwareReset(void);

#ifdef __cplusplus
}
#endif

/*! @}*/
#endif  /* #if !defined(SYSTEM_S32K144_H_) */
