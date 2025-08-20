#include "hal.h"
#include "app_ctl.h"
#include "xprintf.h"
#include "hal.h"
#include "fnd.h"
#include "key.h"
#include "sound.h"
#include "app_normal.h"
#include "app_data.h"
#include "model.h"

#define VER 0x0005

typedef struct _data 
{
    uint8_t adress;
    uint8_t size;
} eeprom_data; 

const uint16_t reset[APP_DATA_MAX]__attribute__((at(0x00007000)))=
{
	VER,										//1			e_sys_ver       
	1,											//2     e_sys_ID_v      
	SNR_NUM,									//3     e_sys_snr_max_v 
	690,690,									//4     e_normal_sv         //Leak Low 
	0,0,										//5     e_hsens_sv      
	0,0,										//6     e_csens_sv      
	200,200,									    //7     e_snr_sv_min        //Water Low
	690,690,									//8     e_snr_sv_max        //Water High
	3,3,               				            //9     e_snr_dt            //Detect Time  
	300,300,               				        //10    e_snr_pwoff_t   water detect after alram off
	10,10,          					        //11    e_broken_sv     
	3700,3700,								    //12    e_short_sv          //Leak High
	0x1B,          	    			            //13	e_sys_config_val
	2 ,                 			            //14    e_sys_modebus_v 
	0,											//15    e_sys_endmode_v 
	MAGIC_CODE          			            //16    e_sys_magic_code	
};


volatile static uint16_t   app_data[APP_DATA_MAX];

uint8_t cheak_data(void)
{
	uint8_t re = 0;
	unsigned long * magic_code = (unsigned long *)FLASH_ADD;//magic_code = (uint16_t)load_data(e_sys_magic_code);
	
    if(magic_code[0] == MAGIC_CODE)
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	
	return re;
}

uint8_t format_eeprom(uint8_t mode)
{
	uint8_t i;
	app_data_reset(mode);
	
	for(i = 0;i < APP_DATA_MAX ; i++)
	{
		save_data((eeprom_index)i		,app_data[i]	);
		delay(10000);
	}
	
	return 1;
}


uint8_t load_all_data(void)
{
	uint8_t i;
	
	for(i = 0;i < APP_DATA_MAX ; i++)
	{
		app_data[i] = (uint16_t)load_data((eeprom_index)i);
	}
	beep_on(get_sys_config_bit(SYS_BUZZER_ON));
	return 1;
}

uint8_t save_data(eeprom_index index, uint32_t data)
{
	uint8_t loop = 0;
	uint8_t re =0;

	uint8_t save[4]={0,0,0,0};
	for(int i = 0 ;i < 2 ;i++ )
	{
		save[i] = (uint8_t)((0xff)&data>>(8*i));
	}
	
	
	for(loop = 0; loop < 3 ; loop ++)
	{
		I2C_Configure();
		re = I2C_write(index*2,&save[0],2);
		if(re == 0)
		{
			re =1;
			break;
		}
		else
		{
			I2C_Configure();
		}
	}
	if(re != 1)
	{
		Error_Handler("0000");
	}
	return re;
}


uint32_t load_data(eeprom_index index)
{	
	uint32_t t_data = 0;
	uint8_t load[4]={0,0,0,0};
	uint8_t loop = 0;
	uint8_t re =0;

	for(loop = 0; loop < 3 ; loop ++)
	{
		I2C_Configure();
		re = I2C_read(index*2,&load[0],2);
		if(re == 0)
		{
			re =1;
			break;
		}
		else
		{
			I2C_Configure();
		}
	}
	if(re != 1)
	{
		Error_Handler("0001");
	}
	
	for(int i = 0 ;i < 2;i++ )
	{
		t_data |= (uint8_t)load[i]<<(8*i);
	}
		
	return t_data;
}


void app_data_reset(uint8_t mode)
{
	
	uint8_t i;
	
	for(i = 0;i < APP_DATA_MAX ; i++)
	{
		app_data[i] = reset[i];
	}
	beep_on(get_sys_config_bit(SYS_BUZZER_ON));
		
}


uint16_t set_app_data(uint8_t index, uint16_t v)
{
	app_data[index] = v;
	save_data((eeprom_index)index	,v);
	return v;
}

uint16_t set_app_data_ch(uint8_t index,uint8_t ch_num, uint16_t v)
{
	app_data[index+ch_num] = v;
	save_data((eeprom_index)(index+ch_num),v);
	return v;
}

uint16_t get_app_data_ch(uint8_t index,uint8_t ch_num)
{
	return app_data[index+ch_num];
}

uint16_t get_app_data(uint8_t index)
{
	return app_data[index];
}

uint8_t set_sys_config_bit(uint8_t bit , uint8_t v)
{
	//app_data[e_sys_config_value]
	if(v == 1)
	{
		app_data[e_sys_config_value] = (app_data[e_sys_config_value] | bit);
	}
	else
	{
		app_data[e_sys_config_value] = (app_data[e_sys_config_value] & (~bit));
	}
	save_data(e_sys_config_value	,app_data[e_sys_config_value]);
	return app_data[e_sys_config_value];
}

uint8_t set_sys_config(uint8_t v)
{
	app_data[e_sys_config_value] = v;
	save_data(e_sys_config_value	,app_data[e_sys_config_value]);
	return app_data[e_sys_config_value];
}

uint8_t get_sys_config(void)
{
	return app_data[e_sys_config_value];
}

uint8_t get_sys_config_bit(uint8_t ck)
{
	uint8_t re=0;
	if(app_data[e_sys_config_value] & ck)
	{
		re = 1;
	}

	return re;
}
