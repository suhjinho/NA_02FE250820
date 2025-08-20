#include "A31G21x_hal_timer2n.h"

HAL_Status_Type HAL_TIMER2n_Init(TIMER2n_Type *TIMER2n, TIMER2n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct)
{
	TIMER2n_PERIODICCFG_Type *pTimeCfg;
	TIMER2n_PWMCFG_Type *pPwmOneshotCfg;
	TIMER2n_CAPTURECFG_Type *pCaptureCfg;
	uint16_t reg_val16;
	
	if (TIMER2n== TIMER20)
	{
		SCU->PER1 &= ~(1<<25);
		SCU->PCER1 &= ~(1<<25);
		
		SCU->PER1 |= (1<<25);
		SCU->PCER1 |= (1<<25);		
	}
	else if (TIMER2n== TIMER21)
	{
		SCU->PER1 &= ~(1<<26);
		SCU->PCER1 &= ~(1<<26);
		
		SCU->PER1 |= (1<<26);
		SCU->PCER1 |= (1<<26);		
	}
	
	if (TimerCounterMode == TIMER2n_PERIODIC_MODE)
	{
		pTimeCfg = (TIMER2n_PERIODICCFG_Type *)TIMER_ConfigStruct;
		
		reg_val16 = 0
			|TIMER2n_CR_CLK_SET(pTimeCfg->CkSel)
			|TIMER2n_CR_MODE_SET(TimerCounterMode)		
			|TIMER2n_CR_OPOL_SET(pTimeCfg->StartLevel)
			;
		if (pTimeCfg->CkSel==1) {
			reg_val16 = reg_val16 | TIMER2n_CR_ECE_SET(pTimeCfg->ECE);
		}		
		TIMER2n->CR = reg_val16;
		
		TIMER2n->PREDR = ((pTimeCfg->Prescaler-1) & TIMER2n_PREDR_MASK);
		TIMER2n->ADR = pTimeCfg->ADR;
	}
	else if ((TimerCounterMode == TIMER2n_PWM_MODE) || (TimerCounterMode == TIMER2n_ONESHOT_MODE))
	{
		pPwmOneshotCfg = (TIMER2n_PWMCFG_Type *)TIMER_ConfigStruct;

		reg_val16 = 0
			|TIMER2n_CR_CLK_SET(pPwmOneshotCfg->CkSel)
			|TIMER2n_CR_MODE_SET(TimerCounterMode)			
			|TIMER2n_CR_OPOL_SET(pPwmOneshotCfg->StartLevel)
			;
		if (pTimeCfg->CkSel==1) {
			reg_val16 = reg_val16 | TIMER2n_CR_ECE_SET(pPwmOneshotCfg->ECE);
		}			
		TIMER2n->CR = reg_val16;
		
		TIMER2n->PREDR = ((pPwmOneshotCfg->Prescaler-1) & TIMER2n_PREDR_MASK);
		TIMER2n->ADR = pPwmOneshotCfg->ADR;
		TIMER2n->BDR = pPwmOneshotCfg->BDR;
	}
	else if (TimerCounterMode == TIMER2n_CAPTURE_MODE)
	{
		pCaptureCfg = (TIMER2n_CAPTURECFG_Type *)TIMER_ConfigStruct;
		
		reg_val16 = 0
			|TIMER2n_CR_CLK_SET(pCaptureCfg->CkSel)
			|TIMER2n_CR_MODE_SET(TimerCounterMode)			
			|TIMER2n_CR_CPOL_SET(pCaptureCfg->ClrMode)
			|TIMER2n_CR_CAPSEL_SET(pCaptureCfg->CAPCkSel)
			;
		if (pTimeCfg->CkSel==1) {
			reg_val16 = reg_val16 | TIMER2n_CR_ECE_SET(pCaptureCfg->ECE);
		}			
		TIMER2n->CR = reg_val16;
		
		TIMER2n->PREDR = ((pCaptureCfg->Prescaler-1) & TIMER2n_PREDR_MASK);
		TIMER2n->ADR = pCaptureCfg->ADR;
	}
	
	TIMER2n->CR |= TIMER2n_CLEAR;
	return HAL_OK;
}

/**********************************************************************//**
 * @brief 		Close Timer/Counter device
 * @param[in]	TIMx  Pointer to timer device, should be:
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER2n_DeInit (TIMER2n_Type *TIMER2n)
{
	// Disable timer/counter
	TIMER2n->CR = 0;
	
	if (TIMER2n== TIMER20)
	{
		SCU->PER1 &= ~(1<<25);
		SCU->PCER1 &= ~(1<<25);
	}
	else if (TIMER2n== TIMER21)
	{
		SCU->PER1 &= ~(1<<26);
		SCU->PCER1 &= ~(1<<26);
	}	
	return HAL_OK;
}

/**********************************************************************//**
 * @brief	 	Start/Stop Timer/Counter device
 * @param[in]	TIMER2n Pointer to timer device, should be:
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @param[in]	NewState
 * 					- ENABLE  	:Set timer enable
 * 					- DISABLE 	:Disable timer
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER2n_Cmd(TIMER2n_Type *TIMER2n, FunctionalState NewState)
{
	if (NewState == ENABLE) {
		TIMER2n->CR |= (1<<15);
	}
	else {
		TIMER2n->CR &= ~(1<<15);
	}
	return HAL_OK;
}

/********************************************************************//**
 * @brief 		Enable or disable TIMER interrupt.
 * @param[in]	TIMER2n Pointer to timer device, should be:
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @param[in]	UARTIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 *						- TIMER_CR_MATCH_INTR : Match interrupt enable
 *						- TIMER_CR_CAPTURE_INTR : Capture interrupt enable 
 * @param[in]	NewState New state of specified UART interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this interrupt type.
 * 					- DISALBE	:Disable this interrupt type.
 * @return 		 HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_TIMER2n_ConfigInterrupt(TIMER2n_Type *TIMER2n, TIMER2n_INT_Type TIMERIntCfg, FunctionalState NewState)
{
	switch(TIMERIntCfg){
		case TIMER2n_CR_MATCH_INTR:
			if (NewState == ENABLE)
				TIMER2n->CR |= (1<<5);
			else if (NewState == DISABLE)
				TIMER2n->CR &= ~(1<<5);
			break;
		case TIMER2n_CR_CAPTURE_INTR:
			if (NewState == ENABLE)
				TIMER2n->CR |= (1<<4);
			else if (NewState == DISABLE)
				TIMER2n->CR &= ~(1<<4);
			break;
	}
	return HAL_OK;
}

/**********************************************************************//**
 * @brief 		Clear Timer/Counter device,
 * @param[in]	TIMER2n Pointer to timer device, should be:
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER2n_ClearCounter(TIMER2n_Type *TIMER2n)
{
	TIMER2n->CR |= (1<<0); //CLEAR;
	return HAL_OK;
}

/**********************************************************************//**

 * @brief        Get flags of Timer 2n Match Interrupt
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @explain      This macro gets interrupt flag of Timer 2n Match Interrupt
 **********************************************************************/
uint32_t HAL_TIMER2n_GetMatchInterrupt(TIMER2n_Type *TIMER2n)
{
	if (TIMER2n->CR & (1<<3)){
		return (1);
	}
	else {
		return (0);
	}
	
}

/**********************************************************************//**

 * @brief        Clear flags of Timer 2n Match Interrupt
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @explain      This macro clears interrupt flag of Timer 2n Match Interrupt
 * return 		 HAL Satus
**********************************************************************/
HAL_Status_Type HAL_TIMER2n_ClearMatchInterrupt(TIMER2n_Type *TIMER2n)
{
	TIMER2n->CR |= (1<<3);
	return HAL_OK;
}

/**********************************************************************//**

 * @brief         Get Timer 2n Capture Data Register
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
 * @explain      This macro gets Timer 2n Capture Data Register
 * return 		 HAL Satus
**********************************************************************/
uint32_t HAL_TIMER2n_GetCaptureData(TIMER2n_Type *TIMER2n)
{
	return (TIMER2n->CAPDR);
}

/**********************************************************************//**

   @brief        Get flags of Timer 2n Capture Interrupt
 * 					- T20	:TIMER20 peripheral
 * 					- T21	:TIMER21 peripheral
   @explain      This macro gets interrupt flag of Timer 2n Capture Interrupt

**********************************************************************/
uint32_t HAL_TIMER2n_GetCaptureInterrupt(TIMER2n_Type *TIMER2n)
{
	if (TIMER2n->CR & (1<<2)){
		return (1);
	}
	else 
	{
		return (0);
	}
}

HAL_Status_Type HAL_TIMER2n_ClearCaptureInterrupt(TIMER2n_Type *TIMER2n)
{
	TIMER2n->CR |= (1<<2);
	return HAL_OK;
}

/* --------------------------------- End Of File ------------------------------ */
