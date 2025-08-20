
#include "led.h"
#include "hal.h"


static uint8_t led_color=0;
static uint8_t blink_on=0;

/*
#define RED_LOW		PF->BCR = _BIT(4);//RED
#define RED_HIGH	PF->BSR = _BIT(4);//RED

#define GREEN_LOW	PE->BCR = _BIT(1);//GREEN
#define GREEN_HIGH	PE->BSR = _BIT(1);//GREEN
*/


void set_led(uint8_t led)
{
	led_color = led;
}

void set_led_blink(uint8_t blink)
{
	blink_on = blink;
}


uint8_t get_led(void)
{
	return led_color;
}

void led_loop(void)
{
	static uint16_t blink_cnt=0;
	static uint8_t blink_st=0;
	
	if(blink_cnt)
		blink_cnt--;
	
	if(blink_on)
	{
		
		if(blink_cnt == 0)
		{
			blink_cnt = 50;
			if(blink_st)
			{
				blink_st = 0;
				RED_LOW;
				GREEN_LOW;
			}
			else
			{
				blink_st = 1;
				if(led_color & 0x01)
				{
					RED_HIGH;
				}
				else
				{
					RED_LOW;
				}
				if(led_color & 0x02)
				{
					GREEN_HIGH;
				}
				else
				{
					GREEN_LOW;
				}
				
			}
		}
	}
	else
	{
		if(led_color & 0x01)
		{
			RED_HIGH;
		}
		else
		{
			RED_LOW;
		}
		if(led_color & 0x02)
		{
			GREEN_HIGH;
		}
		else
		{
			GREEN_LOW;
		}
	}
}
