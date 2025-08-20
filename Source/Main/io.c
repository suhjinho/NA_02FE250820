#include "io.h"
#include "hal.h"
#include "sensor.h"
#include "app_data.h"

void relay_out(uint8_t onoff)
{
	if(onoff == 1)
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1)
		{
			//T_LAMP_HIGH; // NO
		}
		else
		{
			//T_LAMP_LOW;//NC
		}
	}
	else
	{
		if(get_sys_config_bit(SYS_RELAY_MD) == 1)
		{
			
			//T_LAMP_LOW;//NC
		}
		else
		{
			//T_LAMP_HIGH; // NO			
		}		
	}	
}
