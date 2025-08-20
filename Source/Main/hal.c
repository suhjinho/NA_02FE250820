#include "hal.h"
#include "xprintf.h"
#include "sound.h"
#include "key.h"
#include "app_data.h"
#include "app_ctl.h"
#include "fnd.h"
#include "queue.h"
#include "sound.h"
#include "flash.h"
#include "model.h"

#define I2C_TIME_OUT 500

static uint32_t delay_time = 0;
uint16_t current_sensor_data[ADC_DATA_MAX];

void WDT_ResetRun(void)
{	
   HAL_WDT_Start(ENABLE);
}

void WDT_ReloadTimeRun(void)
{
   HAL_WDT_ReloadTimeCounter();
}

void WDT_Configure(void)
{
	WDT_CFG_Type wdtCfg;
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	HAL_SCU_WDT_ClockConfig(WDTCLK_WDTRC);	// 31250 hz	

	HAL_SCU_SetResetSrc(RST_WDTRST, ENABLE);
	HAL_SCU_ClearResetStatus(0xff); // clear all reset status 

  /*	WDTDR(0.5s) < WDTWDR(1s), clear in 900ms */
	wdtCfg.wdtResetEn = ENABLE;
	wdtCfg.wdtClkDiv = WDT_DIV_4;  	
	wdtCfg.wdtTmrConst = 7812/2; 		// 0.5s    @31250
	wdtCfg.wdtWTmrConst = 7812; 		// 1s	 
 
	if(HAL_WDT_Init(wdtCfg)!= HAL_OK)
  {
		/* Initialization Error */
    Error_Handler(" WDT");
	}
}

void TIMER10_IRQHandler_IT(void)
{	
    static uint8_t time_cnt = 0;
    static uint8_t pf4_toggle = 0;
    if (HAL_TIMER1n_GetMatchInterrupt(TIMER10) == 1)
    {
        time_cnt++;
        if(time_cnt >= 10)
        {
            task_1ms_interrupt();
            time_cnt = 0;
        }
        task_100us_interrupt();

        if(pf4_toggle)
        {
            PF->BCR = _BIT(4);
        }
        else
        {
            PF->BSR = _BIT(4);
        }
        pf4_toggle ^= 1;

        HAL_TIMER1n_ClearMatchInterrupt(TIMER10);
    }
}

void Error_Handler( char *errcode  )
{
	fnd_set_blink(FND_BLINK_DIG0|FND_BLINK_DIG1|FND_BLINK_DIG2|FND_BLINK_DIG3);
	fnd_put_line((uint8_t *)errcode,4);

   while( 1 )
   {
		 

   }
}
uint8_t hal_get_key(void)
{
	uint8_t key = 0;
	key = get_key();
	if(key && get_sys_config_bit(SYS_BUZZER_ON))
	{
		beep_buzzer();
	}
	return key;
}
	
void send_data(uint8_t c)
{
#ifdef DEBUG
	DE_485;
#endif
	uart_tx(&c,1);
}
uint8_t escape_msdelay1(void)
{
	if(delay_time == 0)
		return 1;
	else 
		return 0;
}

uint32_t get_msdelay1(void)
{
	return delay_time;
}

void set_msdelay1(uint32_t de)
{
	delay_time = de;
}

void decrease_msdelay1(void)
{
	if(delay_time > 0)
		delay_time--;
}

void GPIO_Configure( void )
{
	//TO-DO	
	HAL_GPIO_ConfigOutput(PD, 1, PUSH_PULL_OUTPUT); // COM0
	HAL_GPIO_ConfigPullup(PD, 1, DISPUPD);
	HAL_GPIO_ClearPin(PD, _BIT(1));
	
	HAL_GPIO_ConfigOutput(PD, 0, PUSH_PULL_OUTPUT); // COM1
	HAL_GPIO_ConfigPullup(PD, 0, DISPUPD);
	HAL_GPIO_ClearPin(PD, _BIT(0));
	
	HAL_GPIO_ConfigOutput(PC, 4, PUSH_PULL_OUTPUT); // COM2
	HAL_GPIO_ConfigPullup(PC, 4, DISPUPD);
	HAL_GPIO_ClearPin(PC, _BIT(4));
	
	HAL_GPIO_ConfigOutput(PC, 3, PUSH_PULL_OUTPUT); // COM3
	HAL_GPIO_ConfigPullup(PC, 3, DISPUPD);
	HAL_GPIO_ClearPin(PC, _BIT(3));
			
	HAL_GPIO_ConfigOutput(PE, 2, PUSH_PULL_OUTPUT); // SEG0
	HAL_GPIO_ConfigPullup(PE, 2, DISPUPD);
	HAL_GPIO_ClearPin(PE, _BIT(2));

	HAL_GPIO_ConfigOutput(PE, 3, PUSH_PULL_OUTPUT); // SEG1
	HAL_GPIO_ConfigPullup(PE, 3, DISPUPD);
	HAL_GPIO_ClearPin(PE, _BIT(3));
	
	HAL_GPIO_ConfigOutput(PE, 4, PUSH_PULL_OUTPUT); // SEG2
	HAL_GPIO_ConfigPullup(PE, 4, DISPUPD);
	HAL_GPIO_ClearPin(PE, _BIT(4));
	
	HAL_GPIO_ConfigOutput(PE, 5, PUSH_PULL_OUTPUT); // SEG3
	HAL_GPIO_ConfigPullup(PE, 5, DISPUPD);
	HAL_GPIO_ClearPin(PE, _BIT(5));
	
	HAL_GPIO_ConfigOutput(PE, 6, PUSH_PULL_OUTPUT); // SEG4
	HAL_GPIO_ConfigPullup(PE, 6, DISPUPD);
	HAL_GPIO_ClearPin(PE, _BIT(6));
	
	HAL_GPIO_ConfigOutput(PE, 7, PUSH_PULL_OUTPUT); // SEG5
	HAL_GPIO_ConfigPullup(PE, 7, DISPUPD);
	HAL_GPIO_ClearPin(PE, _BIT(7));
	
	HAL_GPIO_ConfigOutput(PD, 5, PUSH_PULL_OUTPUT); // SEG6
	HAL_GPIO_ConfigPullup(PD, 5, DISPUPD);
	HAL_GPIO_ClearPin(PD, _BIT(5));
	
	HAL_GPIO_ConfigOutput(PD, 4, PUSH_PULL_OUTPUT); // SEG7
	HAL_GPIO_ConfigPullup(PD, 4, DISPUPD);
	HAL_GPIO_ClearPin(PD, _BIT(4));
	
	HAL_GPIO_ConfigOutput(PE, 0, ALTERN_FUNC); //BUZZER
	HAL_GPIO_ConfigFunction(PE, 0, FUNC1);
	
	// I2C0 PF6:SCL0, PF7:SDA0
	HAL_GPIO_ConfigOutput((PORT_Type*)PF, 6, ALTERN_FUNC);
	HAL_GPIO_ConfigFunction((PORT_Type*)PF, 6, FUNC2);
	HAL_GPIO_ConfigOutput((PORT_Type*)PF, 7, ALTERN_FUNC);
	HAL_GPIO_ConfigFunction((PORT_Type*)PF, 7, FUNC2);
		
	
	HAL_GPIO_ConfigOutput(PB, 2, PUSH_PULL_OUTPUT); //LOAD_EN1			
	HAL_GPIO_ConfigPullup(PB, 2, DISPUPD);
	HAL_GPIO_ClearPin(PB, _BIT(2));
	
	HAL_GPIO_ConfigOutput(PB, 3, PUSH_PULL_OUTPUT); //LOAD_EN2
	HAL_GPIO_ConfigPullup(PB, 3, DISPUPD);
	HAL_GPIO_ClearPin(PB, _BIT(3));

	HAL_GPIO_ConfigOutput(PB, 7, PUSH_PULL_OUTPUT); // SENS_SW1				// mosfet?
	HAL_GPIO_ConfigPullup(PB, 7, DISPUPD);
	HAL_GPIO_ClearPin(PB, _BIT(7));
	
	HAL_GPIO_ConfigOutput(PC, 0, PUSH_PULL_OUTPUT); // SENS_SW2
	HAL_GPIO_ConfigPullup(PC, 0, DISPUPD);
	HAL_GPIO_ClearPin(PC, _BIT(0));
    HAL_GPIO_ConfigOutput(PF, 4, ALTERN_FUNC);
    HAL_GPIO_ConfigFunction(PF, 4, FUNC1);  // CLKO 기능
	
    HAL_GPIO_ConfigOutput(PF, 3, PUSH_PULL_OUTPUT);   // ★ 출력 모드
    HAL_GPIO_ClearPin(PF, _BIT(3));                   // ★ 초기 Low
	
	HAL_GPIO_ConfigOutput(PA, 3, PUSH_PULL_OUTPUT); // LEAK_OUT2
	HAL_GPIO_ConfigPullup(PA, 3, DISPUPD);
	HAL_GPIO_ClearPin(PA, _BIT(3));
	
	HAL_GPIO_ConfigOutput(PA, 4, PUSH_PULL_OUTPUT); // LEAK_OUT1
	HAL_GPIO_ConfigPullup(PA, 4, DISPUPD);
	HAL_GPIO_ClearPin(PA, _BIT(4));
	HAL_GPIO_ConfigOutput(PA, 0, PUSH_PULL_OUTPUT); // BR_OUT1
	HAL_GPIO_ConfigPullup(PA, 0, DISPUPD);
	HAL_GPIO_ClearPin(PA, _BIT(0));
	
	HAL_GPIO_ConfigOutput(PA, 1, PUSH_PULL_OUTPUT); // BR_OUT2
	HAL_GPIO_ConfigPullup(PA, 1, DISPUPD);
	HAL_GPIO_ClearPin(PA, _BIT(1));

	HAL_GPIO_ConfigOutput(PB, 1, ALTERN_FUNC);//I_ADC1 an8
	HAL_GPIO_ConfigFunction(PB, 1, FUNC3);
	HAL_GPIO_ConfigPullup(PB, 1, DISPUPD);
	

	HAL_GPIO_ConfigOutput(PB, 0, ALTERN_FUNC);//I_ADC2//an8
	HAL_GPIO_ConfigFunction(PB, 0, FUNC3);
	HAL_GPIO_ConfigPullup(PB, 0, DISPUPD);

	// KEYS
	HAL_GPIO_ConfigOutput(PC, 1, INPUT); // KEY_R
	HAL_GPIO_ConfigPullup(PC, 1, ENPU);
	
	HAL_GPIO_ConfigOutput(PC, 2, INPUT); // KEY_E
	HAL_GPIO_ConfigPullup(PC, 2, ENPU);
	
	//Initialize USART11 pin connect
	
	HAL_GPIO_ConfigOutput(PD, 3, ALTERN_FUNC);
	HAL_GPIO_ConfigFunction(PD, 3, FUNC1);

	HAL_GPIO_ConfigOutput(PD, 2, ALTERN_FUNC);
	HAL_GPIO_ConfigFunction(PD, 2, FUNC1);
	
	HAL_GPIO_ConfigOutput(PB, 6, PUSH_PULL_OUTPUT); // 485RE/DE
	HAL_GPIO_ConfigPullup(PB, 6, DISPUPD);
	HAL_GPIO_ClearPin(PB, _BIT(6));
	
    HAL_GPIO_ConfigOutput((PORT_Type*)PF, 4, PUSH_PULL_OUTPUT);
    HAL_GPIO_ClearPin(PF, _BIT(4));
}


void TIMER1n_Configure(void)
{
	TIMER1n_PERIODICCFG_Type TIMER1n_Config;
	/*Timer1n clock source from PCLK*/
	HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK); 
	TIMER1n_Config.CkSel = TIMER1n_MCCR1PCLK;    
	TIMER1n_Config.Prescaler =32;    
  
	/*TIMER1n 1msec	*/
	TIMER1n_Config.ADR = (100); 	
	TIMER1n_Config.StartLevel = TIMER1n_START_LOW;
	
	if(HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Config) != HAL_OK)
	{	/* Initialization Error */
		Error_Handler("0003");
	}
	/*TIMER10 ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, ENABLE);
	
	/* Enable Interrupt for TIMERn channel */
	NVIC_SetPriority(TIMER10_IRQn, 3);
	NVIC_EnableIRQ(TIMER10_IRQn);	
	HAL_TIMER1n_Cmd(TIMER10, ENABLE); 
}

void UART11_IRQHandler_IT(void)
{
	// Receive Data Available or Character time-out
	if ((USART11->ST & 0x20)== 0x20)
	{
			uint8_t temp = USART11->DR;
			addq(temp);
	}
}

uint16_t get_ADC_data(uint8_t ad_num)
{
	HAL_ADC_ClearStatus(ADC);	
	HAL_ADC_ChannelSel(ADC, ad_num); // select ch
	HAL_ADC_Start(ADC); // start
	
	while((HAL_ADC_GetStatus(ADC)&ADC_STAT_END) != ADC_STAT_END )
	{
		__nop();
	}
	return HAL_ADC_GetData(ADC);
}


void UART_USART11Configure(void)
{
	USART_CFG_Type USARTConfigStruct;
	
	HAL_USART_UART_Mode_Config(&USARTConfigStruct);	
	USARTConfigStruct.Baud_rate = 9600;
	USARTConfigStruct.Databits = USART_DATABIT_8;
	USARTConfigStruct.Parity = USART_PARITY_NONE;
	USARTConfigStruct.Stopbits = USART_STOPBIT_1;
	
	/* USART11 initialize*/
	if(HAL_USART_Init(USART11, &USARTConfigStruct)!= HAL_OK)
	{
		/* Initialization Error */
		Error_Handler("0004");
	}
	HAL_USART_ConfigInterrupt(USART11, USART_INTCFG_RXC, ENABLE);

	NVIC_SetPriority(USART11_IRQn, 3);
	NVIC_EnableIRQ(USART11_IRQn);
	
	HAL_USART_Enable(USART11, ENABLE);
}

void I2C_Configure(void)
{
	HAL_I2C_DeInit(I2C0);
	 /*Initialize Slave I2C peripheral*/
	if(HAL_I2C_Init(I2C0,50000) != HAL_OK)
	{
		 /* Initialization Error */
     Error_Handler("0005");
	}
	
}


void TIMER3n_Configure(void)
{
	
	/*Timer30 clock source from PCLK*/
	HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK); 
	
	if(HAL_TIMER3n_Init(TIMER30) != HAL_OK)
	{
		Error_Handler("0006");
	}
	
	HAL_TIMER3n_SetPeriod(TIMER30,30578);

	HAL_TIMER3n_SetADuty(TIMER30,(TIMER30->PDR>>1)); 
		
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);
}

void ADC_Configure(void)
{
	ADC_CFG_Type AD_config;
	AD_config.RefSel = ADC_REF_VDD;   //0:ADC_REF_VDD ,1:ADC_REF_AVREF
	AD_config.TrgSel = ADC_TRIGGER_DISABLE;	
	AD_config.InClkDiv = 14;
	if(HAL_ADC_Init(ADC, &AD_config) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler(" ADC");
	}
//	HAL_ADC_ConfigInterrupt(ADC, ENABLE);//인터럽트 방
	HAL_ADC_ConfigInterrupt(ADC, DISABLE);//ADC 센싱을 폴링 방식으로 변경
	/* Enable Interrupt for ADC channel */
	NVIC_SetPriority(ADC_IRQn, 3);
	NVIC_EnableIRQ(ADC_IRQn);
}


void adc_start(void)
{
	HAL_ADC_ClearStatus(ADC);
	HAL_ADC_Start(ADC); // start
	HAL_ADC_ChannelSel(ADC, 11); // select ch

}
void adc_restart(void)
{
	//HAL_ADC_ClearStatus(ADC);
	HAL_ADC_Start(ADC); // start
}
void delay(uint32_t de)
{

	for(; de ; de--)
	{
		__nop();
	}		
}
uint8_t I2C_read(uint8_t address, uint8_t *data, uint8_t len)
{
	uint8_t i;
	uint32_t loop=0;
//	I2C0->CR |= (1<<07);			// I2C Block Enable

	loop=0;
	while(I2C0->ST&0x04)// busy check //
	{
		if(loop ++ > I2C_TIME_OUT)
			return 1;
	}		

	I2C0->CR |=(1<<0);	// START
	
	I2C0->DR = I2C_ADDRESS_W;	
	
	I2C0->ST=0xFF;
	loop=0;
	while(1)
	{
		if((I2C0->CR & 0x10) != 0) break;
		if(loop ++ > I2C_TIME_OUT)
			return 2;
	}
	if(I2C0->ST != 0x87 )
	{
		I2C0->CR|=(1<<1);	// STOP
		I2C0->ST=0xFF; //?
		I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
		return 3;
	}
	I2C0->DR = address;	
	I2C0->ST=0xFF;
	loop=0;
	while(1)
	{
		if((I2C0->CR & 0x10) != 0) break;
		if(loop ++ > I2C_TIME_OUT)
			return 4;
	}
	if(I2C0->ST != 0x47 )
	{
		I2C0->CR|=(1<<1);	// STOP
		I2C0->ST=0xFF; //?
		I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
		return 5;
	}
	
	I2C0->CR = 0
						|(1<<0)	// START
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
	I2C0->ST=0xFF;
	I2C0->DR = I2C_ADDRESS_R;	
	
	loop=0;
	while(1)
	{
		if((I2C0->CR & 0x10) != 0) break;
		if(loop ++ > I2C_TIME_OUT)
			return 6;
	}
	if(I2C0->ST != 0x85 )
	{
		I2C0->CR|=(1<<1);	// STOP
		I2C0->ST=0xFF; //?
		I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
		return 7;
	}
	
	I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
	
	for(i=0; i< len;i++)
	{	
		if(i == len-1)
		{
			I2C0->CR = 0
						|(1<<7)	;
		}

		I2C0->DR = 0x00;
		I2C0->ST=0xFF;
		loop=0;
		while(1)
		{
			if((I2C0->CR & 0x10) != 0) break;
			if(loop ++ > I2C_TIME_OUT)
				return 8;
		}
		if(I2C0->ST != 0x44 && I2C0->ST != 0x45 )
		{
			I2C0->CR|=(1<<1);	// STOP
			I2C0->ST=0xFF; //?
			I2C0->CR = 0
							|(1<<7)			// I2C Block Enable
							|(1<<3) ; // ACKEN
			return 9;
		}
		data[i] = I2C0->DR;
	}
	
	I2C0->CR|=(1<<1);	// STOP
	I2C0->ST=0xFF; //?
	I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
	//I2C0->CR = 0;
	return 0;
}

uint8_t I2C_write(uint8_t address, uint8_t *data, uint8_t len)
{
	uint8_t i;
	uint32_t loop=0;
	//I2C0->CR |= (1<<07);			// I2C Block Enable

	loop=0;
	while(I2C0->ST&0x04)// busy check //
	{
		if(loop ++ > I2C_TIME_OUT)
			return 1;
	}		
	

	I2C0->CR |=(1<<0);	// START
	
	I2C0->DR = I2C_ADDRESS_W;	
	
	I2C0->ST=0xFF;
	loop=0;
	while(1)
	{
		if((I2C0->CR & 0x10) != 0) break;
		if(loop ++ > I2C_TIME_OUT)
			return 0;
	}
	if(I2C0->ST != 0x87 )
	{
		I2C0->CR|=(1<<1);	// STOP
		I2C0->ST=0xFF; //?
		I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
		return 2;
	}
	I2C0->DR = address;	
	I2C0->ST=0xFF;
	loop=0;
	while(1)
	{
		if((I2C0->CR & 0x10) != 0) break;
		if(loop ++ > I2C_TIME_OUT)
			return 3;
	}
	if(I2C0->ST != 0x47 )
	{
		I2C0->CR|=(1<<1);	// STOP
		I2C0->ST=0xFF; //?
		I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
		return 4;
	}
	
	
	
	for(i=0; i< len;i++)
	{	
		I2C0->DR = data[i];
		I2C0->ST=0xFF;
		loop=0;
		while(1)
		{
			if((I2C0->CR & 0x10) != 0) break;
			if(loop ++ > I2C_TIME_OUT)
				return 5;
		}
		if(I2C0->ST != 0x47 )
		{
			I2C0->CR|=(1<<1);	// STOP
			I2C0->ST=0xFF; //?
			I2C0->CR = 0
							|(1<<7)			// I2C Block Enable
							|(1<<3) ; // ACKEN
			return 6;
		}	
	}

	I2C0->CR|=(1<<1);	// STOP
	I2C0->ST=0xFF; //?
	I2C0->CR = 0
						|(1<<7)			// I2C Block Enable
						|(1<<3) ; // ACKEN
	return 0;
}
	
uint8_t uart_tx(uint8_t *data, uint8_t len)
{
	uint8_t i;
	
	for(i=0;i<len;i++)
	{
		while(!(USART11->ST & 0x80)){ }
		
		USART11->DR = data[i];	
	}
	return i;
}

uint8_t uart_rx(uint8_t *data)
{
	uint8_t i =0;

	if(USART11->ST & USART_SR_RXC)		
	{
		data[0] = USART11->DR;
		i++;		
	}
	return i;
}

void	tt_flash_iap_pgm(void)
{
    unsigned long fbuffer[4];
    fbuffer[0] = MAGIC_CODE;
    
	__disable_irq();
	ProgramPage(FLASH_ADD, 4, fbuffer);	
	__enable_irq();
}

void	tt_flash_iap_erase(void)
{
	__disable_irq();	
	EraseSector(FLASH_ADD);	
	__enable_irq();
}
