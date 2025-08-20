#include "sensor.h"
#include "hal.h"
#include "app_data.h"

void crrent_sensor_onoff(uint8_t onoff);
void set_sensor_sw_enable(uint8_t en);
uint32_t sensing(void);

static uint16_t sensor_on=0;

static uint8_t sensor_index_cnt = 0;
uint16_t t_sensing;
static uint16_t sensor_impedance[2][SNR_NUM] = {0};
uint16_t water_ref = 0;
static float sensor_dis_impedance[2][SNR_NUM] = {0};

static uint32_t snr_keep_cnt[SNR_NUM] = {0};
static uint32_t snr_pwoff_cnt[SNR_NUM] = {0};

static uint8_t sensor_ready = 0;


uint32_t get_snr_pwoff_t(uint8_t ch)
{
	if(ch >= SNR_NUM)
		return 0;
	return snr_pwoff_cnt[ch];	
}
uint32_t get_snr_keep_t(uint8_t ch)
{
	if(ch >= SNR_NUM)
		return 0;
	return snr_keep_cnt[ch];
}

uint8_t is_sensor_ready(void)
{
	return sensor_ready;
}

void crrent_sensor_onoff(uint8_t onoff)
{
	if(sensor_on & (0x01<<sensor_index_cnt))
	{		
		if(onoff == 1)
		{
			switch(sensor_index_cnt)
			{
				case 0:
					SNR_EN1_HIGH;
				break;
				case 1:
					SNR_EN2_HIGH;
				break;
			}
		}
		else
		{
			switch(sensor_index_cnt)
			{
				case 0:
					SNR_EN1_LOW;
				break;
				case 1:
					SNR_EN2_LOW;
				break;
			}
		}
	}
	else
	{
			SNR_EN2_LOW;
			SNR_EN1_LOW;
	}
}

	
void set_sensor_on(uint16_t ch,uint16_t onoff)
{
	if(onoff)
	{
		sensor_on = sensor_on | (0x01<<ch);
	}
	else
	{
		sensor_on = sensor_on & ~(0x01<<ch);
	}
}
uint16_t get_sensor_on(uint16_t ch)
{
	return sensor_on | (0x01<<ch);
}


void reset_sensor_data()
{
	uint16_t i;
	for(i=0;i<ADC_DATA_MAX;i++)
	{
		current_sensor_data[i]= 0u;
	}
	//e_sys_snr_max_vsensor_impedance[sel_sensor] = 0u;	
}

uint16_t get_avg(uint16_t *data, uint8_t len)
{
	uint32_t sum=0;
	int i;
	for(i=0;i<len;i++)
	{
		sum = sum + data[i];
	}
		
	return (uint16_t)(sum/len);
}

float get_avg_f(uint16_t *data, uint8_t len)
{
	uint32_t sum=0;
	int i;
	for(i=0;i<len;i++)
	{
		sum = sum + data[i];
	}
		
	return ((float)sum/(float)len);
}

uint16_t get_avg_minmax(uint16_t *data, uint8_t len)
{
	uint16_t min=0xffff;
	uint16_t max=0;
	uint32_t sum=0;
	int i;
	for(i=0;i<len;i++)
	{
		sum = sum + data[i];
		
		if(max <= data[i])
		{
			max = data[i];
		}
		
		if(min >= data[i])
		{
			min = data[i];
		}
	}
	
	sum = sum-(min+max);
	
	
	return (uint16_t)(sum/(len-2));
}

/*
#define SNR_M_SIG_HIGH	PB->BSR = _BIT(3);
#define SNR_M_S0_HIGH	PA->BSR = _BIT(3);
#define SNR_M_S1_HIGH	PA->BSR = _BIT(4);
#define SNR_M_S2_HIGH	PA->BSR = _BIT(0);
#define SNR_M_S3_HIGH	PA->BSR = _BIT(1);
#define SENS_SW_HIGH	PB->BSR = _BIT(6);
*/
uint8_t get_sel_sensor(void)
{
	return sensor_index_cnt;
}

void set_sensor_sw_enable(uint8_t en)
{
#ifdef M_CH1
	if(en)
	{
		SNR_SENS_SW_HIGH
	}
	else
	{
		SNR_SENS_SW_LOW
	}
#else 
	if(en)
	{
		SNR_SENS_SW1_HIGH;
		SNR_SENS_SW2_HIGH;
	}
	else
	{
		SNR_SENS_SW1_LOW;
		SNR_SENS_SW2_LOW;
	}
#endif 

}



void set_sensor_enable(uint8_t snr_ch)
{
#ifdef M_CH1
	SNR_LOAD_EN_HIGH
#else 

	switch(snr_ch)
	{
		case 0 :

			SNR_EN2_LOW;
      SNR_EN1_HIGH;
		break;
		case 1 :
			SNR_EN1_LOW;			
			SNR_EN2_HIGH;
		break;
		case 2 :
		break;
		case 3 :
		break;
		case 4 :
		break;
		default :
			SNR_EN1_LOW;
			SNR_EN1_LOW;
		break;
	}
#endif
}



uint16_t get_charging_low_impedance(uint8_t snr_num)
{
	if(snr_num < SNR_NUM)
	{
		return sensor_impedance[0][snr_num];
	}
	return 0;	
}

uint16_t get_charging_high_impedance(uint8_t snr_num)
{
	if(snr_num < SNR_NUM)
	{
		return sensor_impedance[1][snr_num];
	}
	return 0;	
}

float get_discharging_low_impedance(uint8_t snr_num)
{
	if(snr_num < SNR_NUM)
	{
		return sensor_dis_impedance[0][snr_num];
	}
	return 0;	
}
float get_discharging_high_impedance(uint8_t snr_num)
{
	if(snr_num < SNR_NUM)
	{
		return sensor_dis_impedance[1][snr_num];
	}
	return 0;	
}
//1500 /5 200*5
void sensing_loop(void)//0.1ms
{
	#define NOSENSING_MAX 			1000  	// 45
	#define CHARGING_MAX 	  		500 		// 5
	#define DISCHARGING_SENSING 70		// 10 
	#define DISCHARGING_MAX 		1000 	// 100
  #define SENSOR_PWOFF	 1000		// 1800
  #define AVG_CNT_MAX 	2 //1


	
	static uint8_t is_sener_chraging = 0;//0 dont sening 1 charging 2 discharging
	static uint8_t is_sener_low_impedance = 1;

	static uint32_t charging_low_impedance;
	static uint32_t charging_high_impedance;
	static uint32_t discharging_low_impedance;
	static uint32_t discharging_high_impedance;

	static uint16_t charging_avg_low_impedance[AVG_CNT_MAX];
	static uint16_t discharging_avg_low_impedance[AVG_CNT_MAX];
	
	static uint16_t charging_avg_high_impedance[AVG_CNT_MAX];
	static uint16_t discharging_avg_high_impedance[AVG_CNT_MAX];
	
	static uint16_t senser_cnt = 0;
	
	static uint16_t senser_avg_cnt = 0;
	static uint16_t t_snr_detail_cnt = 0;

	if(sensor_on)
	{
		if(is_sener_low_impedance == 1)
		{
			#if 0
			if(is_sener_chraging == 0)
			{
				senser_cnt++;
				set_sensor_sw_enable(1);
				crrent_sensor_onoff(1);
				
				if(senser_cnt >= NOSENSING_MAX)          //1000
				{
					senser_cnt = 0;
					is_sener_chraging = 1;
					charging_low_impedance = 0;
				}
			}
			else if(is_sener_chraging == 1)
			{				
				charging_low_impedance = charging_low_impedance + sensing();
				senser_cnt++;

				
				if(senser_cnt >= CHARGING_MAX)        // CHARGING_MAX 500
				{
					senser_cnt = 0;
					is_sener_chraging = 2;
				}
			}
			else if(is_sener_chraging == 2)//discharging
			{
				
				senser_cnt++;
				set_sensor_sw_enable(0);	
				
//				if(t_sensing > 20)
//				{
	//				discharging_high_impedance = senser_cnt;
	//			}
				
				if(senser_cnt == DISCHARGING_SENSING)
				{
					discharging_low_impedance = t_sensing;
				}
				if(senser_cnt >= DISCHARGING_MAX)       // DISCHARGING_MAX 1000
				{	
					is_sener_chraging = 3;
					senser_cnt = 0;
#if 0					

					
#endif
				}	
			}
			else if(is_sener_chraging == 3)
			{
				senser_cnt++;
				if(senser_cnt >= 500)       // DISCHARGING_MAX 1000
				{	
					senser_cnt = 0;
					is_sener_chraging = 4;
				}
			}
			else if(is_sener_chraging == 4)
			{
				senser_cnt++;
				crrent_sensor_onoff(0);
				if(senser_cnt >= 1000)       // DISCHARGING_MAX 1000
				{					
					senser_cnt = 0;
					is_sener_chraging = 0;
					is_sener_low_impedance = 0;
				}
			}			
			else
			{
				//nothing
			}
			#endif
			is_sener_low_impedance = 0;
		}
		else
		{
			if(is_sener_chraging == 0)
			{
				senser_cnt++;
				set_sensor_sw_enable(1);
				crrent_sensor_onoff(1);
				
				if(senser_cnt >= NOSENSING_MAX)
				{
					senser_cnt = 0;
					is_sener_chraging = 1;
					charging_low_impedance = 0;
				}
			}
			else if(is_sener_chraging == 1)
			{
				charging_low_impedance = charging_low_impedance + sensing();
				senser_cnt++;
//				set_sensor_sw_enable(1);
//				crrent_sensor_onoff(1);
				t_sensing = sensing();
				if(senser_cnt >= CHARGING_MAX)
				{
					senser_cnt = 0;
					
					is_sener_chraging = 2;
					
//					set_sensor_sw_enable(0);
//					crrent_sensor_onoff(0);
				}
			}
			else if(is_sener_chraging == 2)//discharging
			{
//				uint16_t t_sensing;
				senser_cnt++;
				set_sensor_sw_enable(0);
//				crrent_sensor_onoff(0);
				if(senser_cnt >= NOSENSING_MAX)
				{
					senser_cnt = 0;
					is_sener_chraging = 3;
					charging_high_impedance = 0;
				}
			}
			else if(is_sener_chraging == 3)//discharging
			{
				charging_high_impedance = charging_high_impedance + sensing();
				senser_cnt++;
				
				if(senser_cnt >= CHARGING_MAX)
				{
					senser_cnt = 0;
					is_sener_chraging = 4;
				}
			}			
			else if(is_sener_chraging == 4)
			{
				senser_cnt++;
				crrent_sensor_onoff(0);
				if(senser_cnt >= SENSOR_PWOFF)
				{					
//				set_sensor_sw_enable(0);
					
					senser_cnt = 0;
					is_sener_chraging = 0;
					is_sener_low_impedance = 1;

					charging_avg_high_impedance[senser_avg_cnt] = charging_high_impedance/CHARGING_MAX;
					discharging_avg_high_impedance[senser_avg_cnt] = discharging_high_impedance;

					charging_avg_low_impedance[senser_avg_cnt] = charging_low_impedance/CHARGING_MAX;
					discharging_avg_low_impedance[senser_avg_cnt] = discharging_low_impedance;
					
					senser_avg_cnt++;
					if(senser_avg_cnt == AVG_CNT_MAX )
					{
						senser_avg_cnt = 0;						
						
//						sensor_impedance[0][sensor_index_cnt] = get_avg(charging_avg_low_impedance,AVG_CNT_MAX);
						sensor_impedance[1][sensor_index_cnt] = get_avg(charging_avg_low_impedance,AVG_CNT_MAX);
						water_ref = sensor_impedance[1][sensor_index_cnt];
						sensor_dis_impedance[0][sensor_index_cnt] = get_avg_f(discharging_avg_low_impedance,AVG_CNT_MAX);
						sensor_dis_impedance[1][sensor_index_cnt] = get_avg_f(charging_avg_low_impedance,AVG_CNT_MAX);
						
						//next sensor
						sensor_index_cnt++;
						if(sensor_index_cnt == SNR_NUM)  // SNR_NUM 2
						{
							sensor_ready = 1;
						}
						sensor_index_cnt = sensor_index_cnt % SNR_NUM;
						set_sensor_enable(sensor_index_cnt);
					}
				}
			}
		}		
	}
	else
	{
		is_sener_low_impedance = 1;
		is_sener_chraging = 0;
//		set_sensor_sw_enable(0);
//		crrent_sensor_onoff(0);
		senser_cnt = 0;
		senser_avg_cnt = 0;
	}
}

uint32_t sensing()
{
	uint16_t t_current;
	
	switch(sensor_index_cnt)
	{
		case 0:
			t_current = get_ADC_data(9);//2;
		break;
		case 1:
			t_current = get_ADC_data(8);//2;
		break;
	}
	return (uint32_t)t_current;
}
