#include "hal.h"
#include "app_ctl.h"
#include "app_sensing.h"
#include "xprintf.h"
#include "hal.h"
#include "fnd.h"
#include "key.h"
#include "sound.h"
#include "app_normal.h"
#include "app_data.h"
#include "led.h"
#include "sensor.h"
#include "io.h"

volatile static uint8_t   snr_value[SNR_NUM] = {SNR_ST_NORMAL,SNR_ST_NORMAL}; //
volatile static uint16_t  snr_error_time[SNR_NUM] = {0,0}; //
volatile static uint16_t   snr_impedance; //
volatile static int16_t snr_off_time = 0;
volatile static int16_t snr_on_time = 0;
static uint8_t is_setting_mode = 0;
static uint8_t auto_rcv_off = 0;

#define NORMAL_STATE_INIT 0
#define NORMAL_STATE_MAIN 1
#define NORMAL_STATE_SENSEING 2

void snr_ST_2_char(uint8_t st_v, uint8_t re_type, uint8_t *retext );

uint8_t get_auto_rcv_off(void)
{
	return auto_rcv_off;
}
uint8_t get_snr_state_modbud(uint8_t ch)
{
	uint8_t re = 0;
	
	if(ch < get_app_data(e_sys_snr_max_v))
	{
		switch (snr_value[ch])
		{
			case SNR_ST_NORMAL:
			re = 1;
			break;
			case SNR_ST_H2O:
			re = 4;
			break;
			case SNR_ST_LEAK:
			re = 2;
			break;
			case SNR_ST_SHORT:
			case SNR_ST_BROKEN:
			re = 3;
			break;
			default:
			re  = 1;
			break;
		}		
	}	
	return re;
}

void update_st_error_t(void)
{
	uint16_t i;
	for(i=0;i<get_app_data(e_sys_snr_max_v);i++)
	{
		switch(snr_value[i])
		{
			case SNR_ST_NORMAL:
				snr_error_time[i] = 0;
			break;
			case SNR_ST_LEAK:
				snr_error_time[i]++;
			break;
			case SNR_ST_H2O:
			
			if(get_sys_config_bit(SYS_H2O_REL_USE) == 1)
			{
				snr_error_time[i]++;
			}
			
			break;
			case SNR_ST_SHORT:
			if(get_sys_config_bit(SYS_SHORT_USE) == 1)
			{
				snr_error_time[i]++;
			}
			break;
			case SNR_ST_BROKEN:
			if(get_sys_config_bit(SYS_BROKEN_USE) == 1)
			{
				snr_error_time[i]++;
			}
			break;
		}
	}	
}

void update_st_data(void)
{
	get_snr_st((uint8_t *)&snr_value);
}

void update_st_relay(void)
{
	static uint16_t leak_relay_out = 0;
	static uint16_t broken_relay_out = 0;
	static uint16_t ar_out = 0;
	uint8_t i;
	
	for(i=0;i<get_app_data(e_sys_snr_max_v);i++)
	{
		switch(snr_value[i])
		{
			case SNR_ST_NORMAL:		
				leak_relay_out 		= leak_relay_out & (~(0x01<<i));
				broken_relay_out 	= broken_relay_out & (~(0x01<<i));
				ar_out 			= ar_out & (~(0x01<<i));
			break;
			case SNR_ST_LEAK:
			{
				leak_relay_out 		= leak_relay_out | (0x01<<i);
                broken_relay_out 	= broken_relay_out & (~(0x01<<i));

				if(get_sys_config_bit(SYS_BUZZER_ON) == 1)
				{
					ar_out 		= ar_out | (0x01<<i);
				}
			}
			break;
			case SNR_ST_H2O:
			{
				if(get_sys_config_bit(SYS_BUZZER_ON) == 1 )
				{
					ar_out = ar_out | (0x01<<i);
				}

				if(get_sys_config_bit(SYS_H2O_REL_USE) == 1)
				{
					broken_relay_out = broken_relay_out | (0x01<<i);
				}
				else
				{
					leak_relay_out 		= leak_relay_out & (~(0x01<<i));
					broken_relay_out 	= broken_relay_out & (~(0x01<<i));
				}
			}
			break;
			case SNR_ST_SHORT:
			if(get_sys_config_bit(SYS_SHORT_USE) == 1)
			{			
				broken_relay_out 		= broken_relay_out | (0x01<<i);
                leak_relay_out 		= leak_relay_out & (~(0x01<<i));

			}
			ar_out = ar_out & (~(0x01<<i));
			break;
			case SNR_ST_BROKEN:
			if(get_sys_config_bit(SYS_BROKEN_USE) == 1)
			{
				broken_relay_out 		= broken_relay_out | (0x01<<i);
                leak_relay_out 		= leak_relay_out & (~(0x01<<i));
			}
			ar_out = ar_out & (~(0x01<<i));
			break;
		}
	}
	
	if(leak_relay_out || broken_relay_out)
	{
		relay_out(1);
	}
	else
	{
		relay_out(0);
	}
	if(ar_out && is_setting_mode == 0)
	{		
		beep_alram();
	}

    if(leak_relay_out & (0x0001<<0))
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			LEAK_OUT1_HIGH;//LC
		}
		else
		{
			LEAK_OUT1_LOW;//LO
		}
	}
	else
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			LEAK_OUT1_LOW;//LC
		}
		else
		{
			LEAK_OUT1_HIGH;//LO
		}
	}
	
	if(leak_relay_out & (0x0001<<1))
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			LEAK_OUT2_HIGH;//LC
		}
		else
		{
			LEAK_OUT2_LOW;//LO
		}
	}
	else
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			LEAK_OUT2_LOW;//LC
		}
		else
		{
			LEAK_OUT2_HIGH;//LO
		}
	}
	
	if(broken_relay_out & (0x0001<<0))
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			BR_OUT1_HIGH;
		}
		else
		{
			BR_OUT1_LOW;				
		}
	}
	else
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			BR_OUT1_LOW;
		}
		else
		{
			BR_OUT1_HIGH;				
		}
	}
	
	if(broken_relay_out & (0x0001<<1))
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			BR_OUT2_HIGH;
		}
		else
		{
			BR_OUT2_LOW;				
		}
	}
	else
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1) //NO
		{
			BR_OUT2_LOW;
		}
		else
		{
			BR_OUT2_HIGH;				
		}
	}
}

uint8_t normal(void) //100ms
{
	static uint8_t normal_state = NORMAL_STATE_INIT;
	static uint8_t display_st = 0;
	
	uint8_t re = 0;
	uint8_t key = 0;
	char t_string[4]={0};

	static int8_t snr_cnt = 0;
	
	key = hal_get_key() ;
		
	switch (normal_state)
	{	
		case NORMAL_STATE_INIT: //init
		{
			is_setting_mode = 0;
			
			normal_state = NORMAL_STATE_MAIN;
			display_st = 0;

		}
		break;
		case NORMAL_STATE_MAIN: // sensing
		{
			if(snr_cnt < get_app_data(e_sys_snr_max_v))
			{
				switch(display_st)
				{
					case 0:
						switch(snr_value[snr_cnt])
						{
							case SNR_ST_NORMAL:
								fnd_put(" nor",4);
							break;
							case SNR_ST_LEAK:
								fnd_put(" lak",4);
							break;
							case SNR_ST_H2O:
								fnd_put(" h2o",4);
							break;
							case SNR_ST_SHORT:
								fnd_put(" sht",4);
							break;
							case SNR_ST_BROKEN:
								fnd_put(" brk",4);
							break;
							case SNR_ST_DETECT:
								fnd_put(" Anl",4);
							break;
						}
						if(snr_cnt == 0)
						{
							t_string[0] = '1';
							t_string[0] = t_string[0] | FND_FONT_DP;
							fnd_put((uint8_t*)&t_string[0], 1);
						}
						else
						{
							t_string[0] = '2';
							t_string[0] = t_string[0] | FND_FONT_DP;
							fnd_put((uint8_t*)&t_string[0], 1);
						}
						break;
					case 1:
					{
						uint16_t ad = get_snr_total_impedance(snr_cnt);
						itoa(ad, (unsigned char*)t_string, 4, 10); 
						fnd_put((uint8_t*)t_string, 4);  
					}
						break;
				}
			}
			if(key == KEY_R_SHORT)
			{
				snr_cnt++;

				snr_cnt = snr_cnt % (get_app_data(e_sys_snr_max_v));
			}
			else if(key == KEY_E_SHORT)
			{
				display_st++;
				display_st = display_st % 2;
			}		
		}
		break;
	}
	
	if(key == KEY_E_LONG)
	{
		is_setting_mode = 1;
		re = 1;
	}
	return re;
}
