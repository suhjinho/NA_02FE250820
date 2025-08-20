#include "app_sensing.h"
#include "app_data.h"
#include "sensor.h"
#include "hal.h"
#include "xprintf.h"
#include "app_normal.h"

static int16_t sensor_zone[SNR_NUM] = {0};
static int16_t diff_dis_impedance[SNR_NUM] = {0};
static int16_t cal_impedance[SNR_NUM] = {0};
static int16_t snr_pof_cnt[SNR_NUM] = {0};

uint16_t get_snr_total_impedance(uint8_t snr_num)
{
	return cal_impedance[snr_num];
}

int16_t get_diff_dis_impedance(uint8_t snr_num)
{
	return diff_dis_impedance[snr_num];
}

int16_t get_sensor_zone(uint8_t snr_num)
{
	return sensor_zone[snr_num];
}

uint8_t get_snr_st(uint8_t *re) // 1s
{
	#define DT_MAX 100
	#define SENDS_C_OFFSET 0
	#define SENDS_H_OFFSET 0
	#define SENDS_A_LOW_OFFSET 270
	#define SENDS_B_LOW_OFFSET 85.0F
	#define ZONE_B_AD_START_OFFSET 69 //ad가 b 영역으로 넘어 갈때 시작 값
	#define SENDS_MAX 200
	#define ZONE_1 1000

	static uint16_t detect_cnt[SNR_NUM] = {0};
	static uint16_t wet_cnt[SNR_NUM] = {0};
	static uint16_t leak_cnt[SNR_NUM] = {0};
	static uint16_t broken_cnt[SNR_NUM] = {0};
	static uint16_t short_cnt[SNR_NUM] = {0};
	static uint16_t pwroff_st[SNR_NUM] = {0};
	static uint8_t is_wet[SNR_NUM] = {0};

	int16_t SENDS_OFFSET = 0;
	float SENDS_LOW_OFFSET = 0;
	uint8_t t_re[SNR_NUM] = {SNR_ST_NORMAL};
	uint8_t sensor_cnt;
	
	for(sensor_cnt=0;sensor_cnt<SNR_NUM;sensor_cnt++)
	{

		uint16_t t_total_impedance;

		float t_sensor_dis_impedance;// 측정한한 물 방전 값
		uint16_t t_ref_dis_impedance;
		
		float t_ref_max;// 측정한한 물 방전 mix		
		
		if(get_app_data(e_sys_endmode_v) == 0)
		{
			t_total_impedance = (get_charging_low_impedance(sensor_cnt)/5)+get_charging_high_impedance(sensor_cnt);
		}
		else if(get_app_data(e_sys_endmode_v) == 0)
		{
			t_total_impedance = get_charging_high_impedance(sensor_cnt);
		}
		else
		{
			t_total_impedance = get_charging_low_impedance(sensor_cnt)+get_charging_high_impedance(sensor_cnt);			
		}
		
		cal_impedance[sensor_cnt] = t_total_impedance;
		
#ifdef DEBUG
		xprintf("%d:%d:%d:%d:%d:%d\n",sensor_cnt,get_charging_low_impedance(sensor_cnt),get_charging_high_impedance(sensor_cnt),(int)(get_discharging_low_impedance(sensor_cnt)),(int)(get_discharging_high_impedance(sensor_cnt)),(int)(t_water_ref));

#endif
		{
			int16_t t_diff = (int16_t)(((float)(t_sensor_dis_impedance-SENDS_LOW_OFFSET)/(float)t_ref_max)*100.0f)+SENDS_OFFSET;
		
			diff_dis_impedance[sensor_cnt] = 0;
			t_ref_dis_impedance = 9999;
		}
		
		if(get_app_data_ch(e_normal_sv,sensor_cnt) <= t_total_impedance && t_total_impedance <= get_app_data_ch(e_short_sv,sensor_cnt))
		{
			detect_cnt[sensor_cnt]++;
			if(get_app_data_ch(e_snr_sv_min,sensor_cnt) <= t_total_impedance && get_app_data_ch(e_snr_sv_max,sensor_cnt) >= t_total_impedance)
			{		
				if(diff_dis_impedance[sensor_cnt] < t_ref_dis_impedance)
				{
					leak_cnt[sensor_cnt] = 0;
					wet_cnt[sensor_cnt]++;
				}
				else
				{
					leak_cnt[sensor_cnt]++;
					wet_cnt[sensor_cnt] = 0;
				}
			}
			else
			{
				leak_cnt[sensor_cnt]++;
				wet_cnt[sensor_cnt] = 0;
			}
		}
		else
		{
			wet_cnt[sensor_cnt] = 0;
			detect_cnt[sensor_cnt] = 0;
			leak_cnt[sensor_cnt] = 0;
			t_re[sensor_cnt] = SNR_ST_NORMAL;
		}
		
		if(t_total_impedance < get_app_data_ch(e_broken_sv,sensor_cnt))
		{
			if(get_sys_config_bit(SYS_BROKEN_USE) == 1)
			{
				broken_cnt[sensor_cnt]++;
			}
			else
			{
				broken_cnt[sensor_cnt] = 0;
			}
		}
		else
		{
			broken_cnt[sensor_cnt] = 0;
		}
		
		if(t_total_impedance > get_app_data_ch(e_short_sv,sensor_cnt))
		{
			if(get_sys_config_bit(SYS_SHORT_USE) == 1)
			{
				short_cnt[sensor_cnt]++;
			}
			else
			{
				short_cnt[sensor_cnt]=0;
			}
		}
		else
		{
			short_cnt[sensor_cnt] = 0;
		}
		if(detect_cnt[sensor_cnt] >= 1)
		{
			if(leak_cnt[sensor_cnt] >= get_app_data_ch(e_snr_dt,sensor_cnt)+1)
			{
				leak_cnt[sensor_cnt] = DT_MAX;
				t_re[sensor_cnt] = SNR_ST_LEAK;
					
			}
			else if(wet_cnt[sensor_cnt] >= get_app_data_ch(e_snr_dt,sensor_cnt)+1)
			{
				wet_cnt[sensor_cnt] = DT_MAX;
				t_re[sensor_cnt] = SNR_ST_H2O;
			}
			else
			{
				t_re[sensor_cnt] = SNR_ST_DETECT;
			}
		}
		
		if(broken_cnt[sensor_cnt] >= get_app_data_ch(e_snr_dt,sensor_cnt))
		{
			broken_cnt[sensor_cnt] = DT_MAX;
			t_re[sensor_cnt] = SNR_ST_BROKEN;
		}
		if(short_cnt[sensor_cnt] >= get_app_data_ch(e_snr_dt,sensor_cnt))
		{
			short_cnt[sensor_cnt] = DT_MAX;
			t_re[sensor_cnt] = SNR_ST_SHORT;
		}
				
		
		if(get_sys_config_bit(SYS_H2O_DRY_USE) == 1)
		{	
			if(t_re[sensor_cnt] == SNR_ST_NORMAL)
			{
				is_wet[sensor_cnt] = 0;
			}
			else if(t_re[sensor_cnt] == SNR_ST_H2O)
			{
				is_wet[sensor_cnt] = 1;
			}
			
			if((is_wet[sensor_cnt] == 1) && (t_total_impedance >= get_app_data_ch(e_normal_sv,sensor_cnt)) &&  (t_total_impedance < get_app_data_ch(e_snr_sv_min,sensor_cnt)) && (t_re[sensor_cnt] == SNR_ST_LEAK))
			{
				t_re[sensor_cnt] = SNR_ST_H2O;			
			}
		}
		if(snr_pof_cnt[sensor_cnt] > 0)
		{	
			snr_pof_cnt[sensor_cnt]--;
			t_re[sensor_cnt] = pwroff_st[sensor_cnt] |	SNR_ST_SNR_OFF ;
			detect_cnt[sensor_cnt] = 0;
			wet_cnt[sensor_cnt]    = 0;
			leak_cnt[sensor_cnt]   = 0;
			broken_cnt[sensor_cnt] = 0;
			short_cnt[sensor_cnt]  = 0;
		}
		else
		{
			set_sensor_on(sensor_cnt,1);
			if(t_re[sensor_cnt] == SNR_ST_H2O /*|| t_re[sensor_cnt] == SNR_ST_LEAK */)
			{
				if(get_app_data_ch(e_snr_pwoff_t,sensor_cnt)>0)
				{
					pwroff_st[sensor_cnt] = t_re[sensor_cnt];
					set_sensor_on(sensor_cnt,0);
					snr_pof_cnt[sensor_cnt] = get_app_data_ch(e_snr_pwoff_t,sensor_cnt);
					
				}
			}
		}
		
		if(get_auto_rcv_off() == 0)
		{
			re[sensor_cnt] = t_re[sensor_cnt];
		}
	}

	return 1;
}
