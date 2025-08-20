#include "A31G21x_hal_wt.h"

uint32_t _WTCLK;

HAL_Status_Type HAL_WT_Init(WT_CFG_Type wtCfg)
{
	SCU->PER1 &= ~(1UL<<31); // WT 31 bit
	SCU->PCER1 &= ~(1UL<<31);	

	SCU->PER1 |= (1UL<<31);
	SCU->PCER1 |= (1UL<<31);	
	

	WT->CR = 0
		|((wtCfg.wtClkDiv&0x03)<<4) // WTINTV[1:0]
		|WTCR_WTCLR
		;
	WT->DR = (wtCfg.wtTmrConst & 0xFFF);
	return HAL_OK;
}

HAL_Status_Type HAL_WT_DeInit(void)
{
	WT->CR = 0;
	return HAL_OK;
}

HAL_Status_Type HAL_WT_Start(FunctionalState ctrl)
{
	uint32_t tmp_reg;
	
	tmp_reg = WT->CR&0xFF;
	tmp_reg &= ~(0x1<<7); // Disable watch-dog timer counter
	
	if (ctrl == ENABLE){
		tmp_reg |= (0x1<<7); // Enable watch timer counter
	}
	
	tmp_reg |= WTCR_WTCLR; // clear the counter and divider

	WT->CR = tmp_reg; 
	return HAL_OK;
}

uint32_t HAL_WT_GetCurrentCount(void)
{
	return (WT->CNT & 0xFFF);
}

uint32_t HAL_WT_GetStatus(void)
{
	return (WT->CR & WT_STATUS_BITMASK);
}

HAL_Status_Type HAL_WT_ClearStatus(void)
{
	WT->CR |= WTCR_WTIFLAG; //(1<<1) : clear for WTIFLAG
	return HAL_OK;

}

HAL_Status_Type HAL_WT_ConfigInterrupt(FunctionalState NewState)
{
	uint32_t reg_val=0;
	
	reg_val =(WT->CR &0xFF);
	
	if (NewState == ENABLE) {
		reg_val |= WTCR_WTIEN; // WTIEN bit
	}
	else	{
		reg_val &= ~WTCR_WTIEN;
	}
	
	WT->CR = reg_val | WTCR_WTIFLAG; //(1<<1) : clear for WTIFLAG
	return HAL_OK;

}
