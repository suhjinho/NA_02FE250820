#include "hal.h"
#include "app_ctl.h"
#include "xprintf.h"
#include "hal.h"
#include "fnd.h"
#include "key.h"
#include "led.h"
#include "sound.h"
#include "app_setting.h"
#include "app_normal.h"
#include "app_data.h"
#include "sensor.h"
#include "modbus.h"

uint8_t setting(void);
uint8_t init(void);
void app_task(void);

const char build_date[] =__DATE__;
const char build_time[] =__TIME__;
static e_app_state app_state = app_init;

void task_start(void)
{
	GPIO_Configure();
	ADC_Configure();
	TIMER3n_Configure();
	I2C_Configure();
	UART_USART11Configure();
	TIMER1n_Configure();
#ifdef DEBUG
	xdev_out(send_data);
#endif
}

void task_100us_interrupt(void)
{
	sensing_loop();
}
void task_1ms_interrupt(void)//0.5ms 로 수정
{
	FND_loop();		
}

void task_1ms(void)
{
	sound_loop();
}
void task_10ms(void)
{
	key_loop();
}

void task_1000ms(void)
{
	if(is_sensor_ready())
	{
		update_st_data();
		update_st_relay();
		update_st_error_t();
	}
}
void task_100ms(void)
{
	app_task();
#ifndef DEBUG
	memory_map_update();
	modbus_time_out();
#endif
}
void task_idle(void)
{
#ifndef DEBUG
	modbus_loop();
#endif
}

void app_task(void)
{
	switch(app_state)
	{
		case app_init:
		if(init() == 1)
		{
			app_state = app_normal;
		}
		break;
		case app_normal:
		if(normal() == 1)
		{
			app_state = app_setting;
		}
		break;
		case app_setting:
		{
			if(setting() == 1)
			{
				app_state = app_init;
				
			}			
		}
		break;
		default:
		break;
	}
}

void FMC_WriteEraseRun(void)
{			
		tt_flash_iap_pgm();
		__NOP();	
}

uint8_t init(void)
{
	static uint8_t init_sub_state = 0;
	uint8_t re = 0;
	switch(init_sub_state)
	{
	case 0:
		if(cheak_data())
		{
			load_all_data();
		}
		else
		{
			format_eeprom(0);
		}	
		set_msdelay1(10);
		init_sub_state = 1;
		break;
	case 1:
		if(escape_msdelay1())
		{
			opening_buzzer();
			init_sub_state = 2;
			set_msdelay1(10);
		}
                
        if(! cheak_data())
        {
            FMC_WriteEraseRun();
        }
		break;
	case 2:
		if(escape_msdelay1())
		{

			set_msdelay1(10);
			init_sub_state = 3;
		}
	case 3:
		if(escape_msdelay1())
		{
			uint8_t i;
			fnd_put("v005",4);
			
			for(i=0;i<SNR_NUM;i++)
			{
				set_sensor_on(i,1);
			}
			init_sub_state = 4;
		}
		break;
	case 4:

		if(is_sensor_ready() == 1)
		{
			re = 1;
			init_sub_state = 0;
		}
		break;
	}
	return re;
}
