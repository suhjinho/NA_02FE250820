#include "hal.h"
#include "sound.h"
#include "led.h"

static uint16_t tone_freq[20][2];
static uint16_t tone_freq_MAX = 0;
static uint8_t tone_start_flag = 0;
static uint8_t tone_on_flag = 0;
static uint8_t is_tone_on = 0;

/*
도 523.25  16Mhz / 523.25  =  30578
레 587.33  16Mhz / 587.33  =  27241
미 659.26  16Mhz / 659.26  =  24269
파 698.46  16Mhz / 698.46  =  22907
솔 783.99  16Mhz / 783.99  =  20408
라 880.00  16Mhz / 880.00  =  18181
시 987.77  16Mhz / 987.77  =  16198
도 1046.50 16Mhz / 1046.50 =  15289

도미솔 도미솔 라라라솔
*/

void beep_alram(void)
{
	if(tone_on_flag == 1)
	{
		if(tone_start_flag == 0)
		{
			tone_freq[0][0] = 0xffff;
			tone_freq[0][1] = 50;
			tone_start_flag = 1;
			tone_freq_MAX = 1;
		}
	}
}

uint8_t is_beep(void)
{
	return is_tone_on;
}
void beep_stop(void)
{
	tone_start_flag = 0;
	tone_freq_MAX = 0;
}

void beep_on(uint8_t sw)
{
	tone_on_flag = sw;
	if(tone_on_flag == 0)
	{
		beep_stop();
	}
}
void beep_buzzer(void)
{
	if(tone_on_flag == 1)
	{
		tone_freq[0][0] = 30578;
		tone_freq[0][1] = 25;
		tone_freq[1][0] = 15289;
		tone_freq[1][1] = 25;	
		tone_start_flag = 1;
		tone_freq_MAX = 2;
	}
}
void opening_buzzer(void)
{
	if(tone_on_flag == 1)
	{
		tone_freq[0][0] = 30578;
		tone_freq[0][1] = 250;
		
		tone_freq[1][0] = 24269;
		tone_freq[1][1] = 250;

		tone_freq[2][0] = 20408;
		tone_freq[2][1] = 250;

		
		tone_start_flag = 1;
		tone_freq_MAX = 3;
	}
}

void sound_loop(void)
{

	static uint16_t tone_time_cnt = 0;
	static uint16_t tone_freq_cnt = 0;
	
	if(tone_start_flag == 1)
	{
		if(tone_time_cnt == 0)
		{
			if(tone_freq_cnt < tone_freq_MAX)
			{
				//tone_freq[tone_freq_cnt][0];
				tone_time_cnt = tone_freq[tone_freq_cnt][1];

				if(tone_freq[tone_freq_cnt][0] != 0u)
				{
					is_tone_on = 1;

					HAL_TIMER3n_SetPeriod(TIMER30,tone_freq[tone_freq_cnt][0]);
					HAL_TIMER3n_SetADuty(TIMER30,(TIMER30->PDR>>1)); 
					HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
				}
				else
				{
					is_tone_on = 0;
					HAL_TIMER3n_Start(TIMER30,TIMER30_DISABLE);
				}
				
			}
			else
			{
				tone_start_flag = 0;
				tone_time_cnt = 0;
				tone_freq_cnt = 0;
				is_tone_on = 0;
				return ;
			}
			tone_freq_cnt++;			
		}		
		if(tone_time_cnt != 0xFFFF)
		{
			tone_time_cnt--;
		}
	}
	else
	{
		//PWM STOP
		is_tone_on = 0;
		HAL_TIMER3n_Start(TIMER30,TIMER30_DISABLE);
	}
}
