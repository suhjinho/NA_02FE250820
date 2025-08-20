#include "hal.h"
#include "key.h"

static uint8_t key_event = 0;
uint8_t get_key(void)
{
	uint8_t t_key;
	t_key = key_event;
	
	key_event = 0;
	
	return t_key;
}

void key_loop(void)
{
	uint8_t key_press = 0;
	static uint8_t key_press_type = 0;
	static uint16_t press_cnt = 0;
	static uint8_t last_key_press_type = 0;


	static uint8_t double_press = 0;
	static uint16_t double_press_cnt = 0;
	
	if(KEY_D == 0)
	{
		key_press = KEY_D_SHORT;
	}
	else if(KEY_R == 0)
	{
		key_press = KEY_R_SHORT;
	}
	else if(KEY_E == 0)
	{
		key_press = KEY_E_SHORT;
	}
	else if(KEY_U == 0)
	{
		key_press = KEY_U_SHORT;
	}
	else if(KEY_L == 0)
	{
		key_press = KEY_L_SHORT;
	}
	
	if(key_press > 0)
	{
		press_cnt++;
	}
	else
	{
		if(last_key_press_type > 0)
		{
			double_press_cnt++;
			if(double_press_cnt >= DOUBLE_PRESS_TIME)
			{
				double_press_cnt = 0;
				key_event = last_key_press_type;
				last_key_press_type = 0;
			}
			else
			{
				if(double_press == 1)
				{
					double_press_cnt = 0;
					key_event = 0x40 | last_key_press_type;
					last_key_press_type = 0;
					double_press = 0;
				}				
			}
		}
		else
		{
			double_press_cnt = 0;
		}
		press_cnt = 0;
		key_press_type = KEY_TY_SHORT;
	}
	if(key_press_type == KEY_TY_SHORT)
	{
		if(press_cnt >= SHORT_PRESS_TIME)
		{
			if(last_key_press_type == 0)
			{
				last_key_press_type = key_press;
				key_press_type = KEY_TY_LONG;
				press_cnt = 0;
			}
			else
			{
				if(last_key_press_type == key_press)
				{
					double_press = 1;
					press_cnt = 0;
				}
			}
			double_press_cnt = 0;
		}		
	}
	else if(key_press_type == KEY_TY_LONG)
	{
		if(press_cnt == SHORT_LONG_TIME)
		{
			last_key_press_type = 0;
			key_event = 0x40 | key_press;
			key_press_type = KEY_TY_LONGOUT;
			press_cnt = 0;
		}		
	}
	else if(key_press_type == KEY_TY_LONGOUT)
	{
		if(press_cnt == SHORT_LONGOUT_TIME)
		{
			last_key_press_type = 0;
			key_event = 0x80 | key_press;
			key_press_type = KEY_TY_COUNTINE;
			press_cnt = 0;
		}
	}
	else if(key_press_type == KEY_TY_COUNTINE)
	{
		if(press_cnt == SHORT_COUNTINE_TIME)
		{
			last_key_press_type = 0;
			key_event = 0x80 | key_press;
			key_press_type = KEY_TY_COUNTINE;
			press_cnt = 0;
		}
	}
}
