/***************************************************************************//**
* @file     hal.h
*
*
* @version  1.00
* @date     2022-05-02
* @author   RND
*
* Copyright(C) 2022, amers
* All rights reserved.
*
*//****************************************************************************/

#ifndef __HAL_H
#define __HAL_H

/* Includes ----------------------------------------------------------------- */
/* Uncomment the line below to enable peripheral header file inclusion */

#include "A31G21x.h"
#include "A31G21x_hal_aa_types.h"
#include "A31G21x_hal_debug_frmwrk.h"
#include "A31G21x_hal_libcfg.h"
#include "A31G21x_hal_adc.h"
#include "A31G21x_hal_crc.h"
#include "A31G21x_hal_i2c.h"
#include "A31G21x_hal_pcu.h"
#include "A31G21x_hal_pwr.h"
#include "A31G21x_hal_scu.h"
#include "A31G21x_hal_timer1n.h"
#include "A31G21x_hal_timer2n.h"
#include "A31G21x_hal_timer30.h"
#include "A31G21x_hal_uartn.h"
#include "A31G21x_hal_usart1n.h"
#include "A31G21x_hal_wdt.h"
#include "A31G21x_hal_wt.h"

//#include "A31G21x_it.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */

//extern uint16_t volt_sensor_data[];
extern uint16_t current_sensor_data[];
/* Private define ----------------------------------------------------------- */
#define PWM_HZ			6000  /* pwm */
#define I2C_ADDRESS_W  0xA0
#define I2C_ADDRESS_R  0xA1

//#define ADC_DATA_MAX 100

#define ADC_DATA_MAX 10

#define DE_485		PB->BSR = _BIT(6); // 485
#define RE_485		PB->BCR = _BIT(6); // 485	

#define KEY_D	1
#define KEY_R	(PC->INDR & (_BIT(1))) //rotation
#define KEY_E	(PC->INDR & (_BIT(2))) // enter 

#define KEY_U	1
#define KEY_L	1


#define COM_ALL_HIGH COM0_HIGH;\
	COM1_HIGH;\
	COM2_HIGH;\
	COM3_HIGH;

#define COM_ALL_LOW COM0_LOW;\
 COM1_LOW;\
 COM2_LOW;\
 COM3_LOW;

#define COM0_LOW	PD->BCR = _BIT(1); // com0 
#define COM1_LOW	PD->BCR = _BIT(0);// com1
#define COM2_LOW	PC->BCR = _BIT(4);// com2 
#define COM3_LOW	PC->BCR = _BIT(3);// com3 


#define COM0_HIGH	PD->BSR = _BIT(1); // com0 
#define COM1_HIGH	PD->BSR = _BIT(0);// com1
#define COM2_HIGH	PC->BSR = _BIT(4);// com2 
#define COM3_HIGH	PC->BSR = _BIT(3);// com3 

#define SEG_ALL_LOW SEG0_LOW;\
SEG0_LOW;\
SEG1_LOW;\
SEG2_LOW;\
SEG3_LOW;\
SEG4_LOW;\
SEG5_LOW;\
SEG6_LOW;\
SEG7_LOW;

#define SEG_ALL_HIGH SEG0_HIGH;\
SEG0_HIGH;\
SEG1_HIGH;\
SEG2_HIGH;\
SEG3_HIGH;\
SEG4_HIGH;\
SEG5_HIGH;\
SEG6_HIGH;\
SEG7_HIGH;

#define SEG0_LOW	PE->BCR = _BIT(2);//seg0
#define SEG1_LOW	PE->BCR = _BIT(3);//seg1
#define SEG2_LOW	PE->BCR = _BIT(4);//seg2
#define SEG3_LOW	PE->BCR = _BIT(5);//seg3
#define SEG4_LOW	PE->BCR = _BIT(6);//seg4
#define SEG5_LOW	PE->BCR = _BIT(7);//seg5
#define SEG6_LOW	PD->BCR = _BIT(5);//seg6
#define SEG7_LOW	PD->BCR = _BIT(4);//seg7

#define SEG0_HIGH	PE->BSR = _BIT(2);//seg0
#define SEG1_HIGH	PE->BSR = _BIT(3);//seg1
#define SEG2_HIGH	PE->BSR = _BIT(4);//seg2
#define SEG3_HIGH	PE->BSR = _BIT(5);//seg3
#define SEG4_HIGH	PE->BSR = _BIT(6);//seg4
#define SEG5_HIGH	PE->BSR = _BIT(7);//seg5
#define SEG6_HIGH	PD->BSR = _BIT(5);//seg6
#define SEG7_HIGH	PD->BSR = _BIT(4);//seg7


#define SNR_EN1_HIGH   PB->BSR = _BIT(2) //LOAD_EN1
#define SNR_EN1_LOW   PB->BCR = _BIT(2) //LOAD_EN1
								   
#define SNR_EN2_HIGH   PB->BSR = _BIT(3) //LOAD_EN2
#define SNR_EN2_LOW   PB->BCR = _BIT(3) //LOAD_EN2

#define SNR_SENS_SW1_HIGH   PB->BSR = _BIT(7) //SENS_SW1
#define SNR_SENS_SW1_LOW   PB->BCR = _BIT(7) //SENS_SW1
								   
#define SNR_SENS_SW2_HIGH   PC->BSR = _BIT(0) //SENS_SW2
#define SNR_SENS_SW2_LOW   PC->BCR = _BIT(0) //SENS_SW2
        
#define LEAK_OUT1_HIGH  PA->BSR = _BIT(4) //LEAK_OUT1
#define LEAK_OUT1_LOW  PA->BCR = _BIT(4) //LEAK_OUT1
								   
#define LEAK_OUT2_HIGH  PA->BSR = _BIT(3) //LEAK_OUT2
#define LEAK_OUT2_LOW  PA->BCR = _BIT(3) //LEAK_OUT2


#define BR_OUT1_HIGH  PA->BSR = _BIT(0) //LEAK_OUT1
#define BR_OUT1_LOW  PA->BCR = _BIT(0) //LEAK_OUT1
								   
#define BR_OUT2_HIGH  PA->BSR = _BIT(1) //LEAK_OUT2
#define BR_OUT2_LOW  PA->BCR = _BIT(1) //LEAK_OUT2
	
/* Private function prototypes ---------------------------------------------- */
void GPIO_Configure( void );
void ADC_Configure(void);
void Error_Handler( char *errcode  );
uint16_t get_adc_ch(uint8_t ch);
void TIMER3n_Configure(void);
void I2C_Configure(void);
void UART_USART11Configure(void);
uint8_t uart_tx(uint8_t *data, uint8_t len);
uint8_t uart_rx(uint8_t *data);
uint8_t I2C_write(uint8_t address, uint8_t *data, uint8_t len);
uint8_t I2C_read(uint8_t address, uint8_t *data, uint8_t len);
void send_data(uint8_t c);
void delay(uint32_t de);


uint8_t escape_msdelay1(void);
uint32_t get_msdelay1(void);
void set_msdelay1(uint32_t de);
void decrease_msdelay1(void);
uint8_t hal_get_key(void);
void TIMER10_IRQHandler_IT(void);
void UART11_IRQHandler_IT(void);
void TIMER1n_Configure(void);
void adc_start(void);
uint16_t get_ADC_data(uint8_t ad_num);

void WDT_ResetRun(void);
void WDT_ReloadTimeRun(void);
void WDT_Configure(void);
void adc_restart(void);
uint8_t is_sensor_ready(void);

void tt_flash_iap_pgm(void);
void tt_flash_iap_erase(void);

#ifdef __cplusplus
}
#endif

#endif   /* __A31G21x_CONF_H */

