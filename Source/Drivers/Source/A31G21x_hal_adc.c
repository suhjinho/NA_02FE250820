#include "A31G21x_hal_adc.h"
#include "A31G21x_hal_scu.h"

HAL_Status_Type HAL_ADC_Init(ADC_Type *ADCx, ADC_CFG_Type *ADC_ConfigStruct)
{
	uint32_t tempreg;
  
	
	/* Check ADC handle */
	if(ADCx == NULL)
	{
		return HAL_ERROR;
	}
	 
	SCU->PER2&=~(1<<20);
	SCU->PCER2&=~(1<<20);
	/* Set up peripheral clock for ADC module */
	SCU->PER2|=(1<<20);
	SCU->PCER2|=(1<<20);

	ADCx->CR = (1<<15);  // ADCEN;

	tempreg = 0
		| (1<<15)  // ADCEN
		| (((ADC_ConfigStruct->TrgSel) & 7)<<11)  // TRGSRC
		| (((ADC_ConfigStruct->RefSel) & 1)<<10) 
		| (1<<4)  // clear flag
		;				
	ADCx->CR = tempreg;

	ADCx->PREDR = (ADC_ConfigStruct->InClkDiv&0x1f);
	return HAL_OK;
}

HAL_Status_Type HAL_ADC_DeInit(ADC_Type *ADCx)
{
	ADCx->CR = 0;
	
	SCU->PER2&=~(1<<20);
	SCU->PCER2&=~(1<<20);
	return HAL_OK;
}

HAL_Status_Type HAL_ADC_Start(ADC_Type *ADCx)
{
	/* Check ADC handle */
	if(ADCx == NULL)
	{
	  return HAL_ERROR;
	}

	ADCx->CR |=(1<<8);

	return HAL_OK;
}

HAL_Status_Type HAL_ADC_Stop(ADC_Type *ADCx)
{
	/* Check ADC handle */
	if(ADCx == NULL)
	{
	  return HAL_ERROR;
	}

	ADCx->CR&=~(1<<8);
	return HAL_OK;
}


uint16_t HAL_ADC_GetData(ADC_Type *ADCx)
{
	uint16_t adc_value;

	adc_value = ADCx->DR;
	
	return ADC_DR_RESULT(adc_value);
}

HAL_Status_Type HAL_ADC_ConfigInterrupt(ADC_Type *ADCx, FunctionalState NewState)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg &= ~(1<<5) ;

	if (NewState){
		tempreg |= (1<<5) ;
	}
	ADCx->CR = tempreg; 	
	return HAL_OK;
}

HAL_Status_Type HAL_ADC_ChannelSel(ADC_Type *ADCx, uint32_t Channel)
{
	uint32_t temp_reg;
	
	temp_reg = ADCx->CR & (uint32_t)(~0x0f);
	temp_reg |= (uint32_t)(Channel&0x0f);	
	ADCx->CR = temp_reg;
	return HAL_OK;
}

uint32_t HAL_ADC_GetStatus(ADC_Type *ADCx)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg &= (1<<4) ;
	
	return tempreg;
}

HAL_Status_Type HAL_ADC_ClearStatus(ADC_Type *ADCx)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg |= (1<<4) ;
			
	ADCx->CR = tempreg;  // clear flag;	
	return HAL_OK;
}
