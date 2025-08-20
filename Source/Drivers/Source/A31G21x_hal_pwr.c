#include "A31G21x_hal_scu.h"
#include "A31G21x_hal_pwr.h"

void HAL_PWR_EnterSleepMode(void)
{
	/* Sleep Mode*/
	SCB->SCR = 0;
	__WFI();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();	
}

void HAL_PWR_EnterPowerDownMode(void)
{
   /* Deep-Sleep Mode, set SLEEPDEEP bit */
	SCB->SCR = 0x4;
	/* Power Down Mode*/
	__WFI();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}

HAL_Status_Type HAL_LVI_Init(LVI_CFG_Type *LVI_ConfigStruct)
{
	uint32_t	tempreg;
	
	tempreg = SCULV->LVICR&0xFF;
	
	tempreg = SCULV->LVICR;
	tempreg = 0
	| ((LVI_ConfigStruct->EnSel)<<7)
	| ((LVI_ConfigStruct->IntSel)<<5)
	| ((LVI_ConfigStruct->LvlSel)<<0)
	;
	SCULV->LVICR = tempreg;
	return HAL_OK;
}
