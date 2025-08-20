#include "A31G21x_hal_crc.h"

HAL_Status_Type HAL_CRC_Init(void)
{
    SCU->PER2 &=~(1UL<<31);
	  SCU->PCER2&=~(1UL<<31);
    /* Set up peripheral clock for CRC module */
    SCU->PER2 |= (1UL<<31);
    SCU->PCER2|= (1UL<<31);
	
    return HAL_OK;
}

HAL_Status_Type HAL_CRC_DeInit(void)
{
	// Disable CRC peripheral & clock
	SCU->PER2 &= ~(1UL<<31);
	SCU->PCER2 &= (1UL<<31);
	
	return HAL_OK;
}

uint32_t HAL_CRC_ConfigAutoMode(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit)
{
//	if(SystemCoreClock>20000000uL) SCUCG->SCDIVR0_b.HDIV = 3;               // HCLK should be less than or equal to 20MHz during CRC/Checksum auto mode

	CRC->CR = (0x1uL<<CRC_CR_RLTCLR_Pos)                              // Initialize RLT Register with INIT value
	             |CRC_AUTO_M                                                // Set Auto Mode
	             |u32SEL
	             |u32POLY
	             |CRC_NOINC                                                 // Don't care at auto mode
	             |u32FirstBit;
//	DI();
//	CRCRun();
//	EI();
//	while(ChkCRCFinish()) {}                                                // Check if CRC/Checksum finishes or not
//	SCUCG->PPCLKEN1_b.CRCLKE = 0;                                           // CRC/Checksum Clock Disable
//	if(SystemCoreClock>20000000uL) SCUCG->SCDIVR0_b.HDIV = 4;               // HCLK should be set with original frequency
	return (CRC->RLT & 0xffff);
}

HAL_Status_Type HAL_CRC_ConfigUserMode(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit)
{
	CRC->CR = (0x1uL<<CRC_CR_RLTCLR_Pos)                              // Initialize RLT Register with INIT value
	             |CRC_USER_M                                                // Set User Mode
	             |u32SEL
	             |u32POLY
	             |u32FirstBit;
	return HAL_OK;
//	CRCRun();
}

uint32_t HAL_CRC_UserInput(uint32_t u32Input)
{
	uint32_t u32Result = 0x8a290000uL;
	
	CRC_InData(u32Input);
//	if(!ChkCRCFinish() || (CRC->CRCSADR > CRC->CRCEADR)) {                  // "Auto"/"User" Increment of Start Address
//	  CRCStop();                                                            // Stop forcingly on User Increment of Start Address
//	  SCUCG->PPCLKEN1_b.CRCLKE = 0;                                         // CRC/Checksum Clock Disable
//	  u32Result = (CRC->RLT & 0xffff);
//	}
	return u32Result;
}


uint32_t HAL_CRC_GetData(void)
{
	uint32_t	u32Result;
	
	u32Result = CRC->RLT;
	
	return u32Result;
}


HAL_Status_Type HAL_CRC_SetUserInput(uint32_t InitVal)
{
	CRC->INIT = InitVal;
	return HAL_OK;
}


HAL_Status_Type HAL_CRC_ConfigInterrupt(uint32_t IntSel)
{
	uint32_t	tempreg;
	
	tempreg = CRC->CR;
	
	tempreg &= ~(1<<9);
	
	if (IntSel)
	{
		tempreg |= (1<<9);
	}
	
	CRC->CR = tempreg;
	return HAL_OK;
}

uint32_t HAL_CRC_GetState_IT(void)
{
	return (CRC->CR & (1<<8));
}

HAL_Status_Type HAL_CRC_ClearState_IT(void)
{
	CRC->CR |= (1<<8);
	return HAL_OK;
}
