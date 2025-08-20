#include "main_conf.h"
void	SystemClock_Config(void)
{
	__IO uint32_t cnt = 0, status;
	
	SCU->CMR &= ~(1<<7);	// disable MCLK monitoring
	HAL_SCU_HSI_ClockConfig(HSIOSC_EN);	// enable HSI
	
	SCU->CSCR = 0xA5070000 | (SCU->CSCR & (~(0x7 << 4))) | (0x0 << 4);
	
	SCU->SCCR = 0x570A0000 | (SCU->SCCR & (~(0x1 << 2))) | (0x0 << 2);
	SCU->PLLCON = 0x00C00000;	// enable PLL
	do {	// wait PLL locking
		status = (SCU->PLLCON & (1UL<<31));
		cnt++;
	} while((cnt < PLL_STARTUP_TIMEOUT) && (status != (1UL<<31)));

	
	if (cnt < PLL_STARTUP_TIMEOUT) {
		SCU->PLLCON = 0x00E00000;	// PLL bypass disable
	}
	
	SCU->SCCR = 0x570A0000 | (SCU->SCCR & (~0x3)) | 0x2;
	
	SystemCoreClock = 32000000uL;
	SystemPeriClock = 32000000uL;
	
	SCU->MCCR1 = 0x02010301;
	SCU->MCCR2 = 0x02010201;
	SCU->MCCR3 = 0x02010201;
	SCU->MCCR4 = 0x02010201;
	SCU->MCCR5 = 0x00010201;
}
