/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include "cortexm/ExceptionHandlers.h"

// ----------------------------------------------------------------------------

void __attribute__((weak))
Default_Handler(void);

// Forward declaration of the specific IRQ handlers. These are aliased
// to the Default_Handler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
// TODO: Rename this and add the actual routines here.

void __attribute__ ((weak, alias ("Default_Handler")))
LVI_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
WUT_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
WDT_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
EXTINT0_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
EXTINT1_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
EXTINT2_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
EXTINT3_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER10_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER11_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER12_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
I2C0_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
USART10_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
WT_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER30_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
I2C1_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER20_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER21_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
USART11_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
ADC_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
UART0_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
UART1_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER13_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER14_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER15_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
TIMER16_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
I2C2_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler")))
USART12_13_Handler(void);

// ----------------------------------------------------------------------------

extern unsigned int _estack;

typedef void
(* const pHandler)(void);

// ----------------------------------------------------------------------------

// The vector table.
// This relies on the linker script to place at correct location in memory.

__attribute__ ((section(".isr_vector"),used))
pHandler __isr_vectors[] =
  { //
    (pHandler) &_estack,                          // The initial stack pointer
        Reset_Handler,                            // The reset handler

        NMI_Handler,                              // The NMI handler
        HardFault_Handler,                        // The hard fault handler

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
        MemManage_Handler,                        // The MPU fault handler
        BusFault_Handler,// The bus fault handler
        UsageFault_Handler,// The usage fault handler
#else
        0, 0, 0,				  // Reserved
#endif
        0,                                        // Reserved
        0,                                        // Reserved
        0,                                        // Reserved
        0,                                        // Reserved
        SVC_Handler,                              // SVCall handler
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
        DebugMon_Handler,                         // Debug monitor handler
#else
        0,					  // Reserved
#endif
        0,                                        // Reserved
        PendSV_Handler,                           // The PendSV handler
        SysTick_Handler,                          // The SysTick handler

        // ----------------------------------------------------------------------
        // A31G31x vectors
		LVI_Handler,     //         /*!<  0: LVDDETECT                            */
		WUT_Handler,   //         /*!<  1: SYSCLKFAIL                            */
		WDT_Handler,   //         /*!<  2: XOSCFAIL                            */
		EXTINT0_Handler,         //         /*!<  3: WDT                            */
		EXTINT1_Handler,         //         /*!<  4: Default                            */
		EXTINT2_Handler,      //         /*!<  5: TIMER0                            */
		EXTINT3_Handler,      //         /*!<  6: TIMER1                            */
		TIMER10_Handler,      //         /*!<  7: TIMER2                            */
		TIMER11_Handler,      //         /*!<  8: TIMER3                            */
		TIMER12_Handler,      //         /*!<  9: TIMER4                            */
		I2C0_Handler,      //         /*!< 10: TIMER5                            */
		USART10_Handler,      //         /*!< 11: TIMER6                            */
		WT_Handler,      //         /*!< 12: TIMER7                            */
		TIMER30_Handler,      //         /*!< 13: TIMER8                            */
		I2C1_Handler,      //         /*!< 14: TIMER9                            */
		TIMER20_Handler,     //         /*!< 15: MCKFAIL                            */
		TIMER21_Handler,       //         /*!< 16: GPIOA                            */
		USART11_Handler,       //         /*!< 17: GPIOB                            */
		ADC_Handler,       //         /*!< 18: GPIOC                            */
		UART0_Handler,       //         /*!< 19: GPIOD                            */
		UART1_Handler,       //         /*!< 20: GPIOE                            */
		TIMER13_Handler,       //         /*!< 21: GPIOF                            */
		TIMER14_Handler,     //            /*!< 22: Default                            */
		TIMER15_Handler,     //            /*!< 23: Default                            */
		TIMER16_Handler,        //         /*!< 24: PWM0                            */
		I2C2_Handler,        //         /*!< 25: PWM1                            */
		USART12_13_Handler,        //         /*!< 26: PWM2                            */
		Default_Handler,        //         /*!< 27: PWM3                            */
		Default_Handler,        //         /*!< 28: PWM4                            */
		Default_Handler,        //         /*!< 29: PWM5                            */
		Default_Handler,        //         /*!< 30: PWM6                            */
		Default_Handler,        //         /*!< 31: PWM7                            */
    // TODO: rename and add more vectors here
    };

// ----------------------------------------------------------------------------

// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.

void __attribute__ ((section(".after_vectors")))
Default_Handler(void)
{
  while (1)
    {
    }
}

// ----------------------------------------------------------------------------
