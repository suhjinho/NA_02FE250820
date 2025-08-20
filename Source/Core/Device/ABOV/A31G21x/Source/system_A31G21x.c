#include "A31G21x.h"

uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  HLCK */
uint32_t SystemPeriClock;     /*!< System Clock Frequency (Peri Clock)  PCLK */

void SystemCoreClockUpdate(void)     /* Get Core Clock Frequency      */
{
	SystemCoreClock=32000000; 
	SystemPeriClock=32000000; 
}

void SystemInit (void)
{
///* ToDo: add code to initialize the system
//         do not use global variables because this function is called before
//         reaching pre-main. RW section maybe overwritten afterwards.          */
//
	__disable_irq();      //Disable  Interrupt
	WDT->CR = 0 // disable WDT ;default ON so you must turn off
		|(0x5A69<<16)
		|(0x25<<10)
		|(0x1A<<4)
		;

	SystemCoreClock=500000; //500khz
	SystemPeriClock=500000; //500khz
//
// flash memory controller
	FMC->MR = 0x81;       // after changing 0x81 -> 0x28 in MR reg, flash access timing will be able to be set.
	FMC->MR = 0x28;       // enter flash access timing changing mode
	FMC->CFG = (0x7858<<16) | (3<<8);  //flash access cycles 	
	                            // flash access time cannot overflow ??Mhz.
	                            // ex) if MCLK=40Mhz, 
	                            //       40/1 = 40 (can't set no wait)
	                            //       40/2 = 20 (1 wait is ok)
	                            // so, 1 wait is possible.
	FMC->MR = 0;	      // exit flash access timing --> normal mode		
}
