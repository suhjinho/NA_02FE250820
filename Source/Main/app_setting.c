#include "hal.h"
#include "app_ctl.h"
#include "xprintf.h"
#include "hal.h"
#include "fnd.h"
#include "key.h"
#include "sound.h"
#include "app_setting.h"
#include "app_data.h"

#define AD_MAX          4096
#define AD_MIN          0  
#define ER_MAX          4096
#define ER_MIN          0  
#define TIME_MAX         999  
#define TIME_MIN         0

void * setting_func[SETTING_FUNC_MAX] = 
{
	setting_normal1			,//1
	setting_normal2			,//2
	setting_snr_min1		,//3
	setting_snr_min2		,//4
	setting_snr_sv_max1		,//5
	setting_snr_sv_max2		,//6
	setting_snr_dt1			,//7
	setting_snr_dt2			,//8
	setting_snr_pt1			,//9
	setting_snr_pt2			,//10
	setting_broken_sv1		,//11
	setting_broken_sv2		,//12
	setting_short_sv1		,//13
	setting_short_sv2		,//14
	setting_h2o_rel			,//15
	setting_h2o_dry 		,//16
	setting_broken_chk		,//17
	setting_short_chk		,//18
	setting_buzzer			,//19
	setting_relay_out_md    ,
	setting_modbus_v		,//20
	setting_modbus_id		,//21
	setting_recovery_md	    ,//22
	//23
	setting_factort_reset	,//24
	setting_exit			 //25
};


static const uint16_t pow10[4] = {1,10,100,1000};

uint8_t setting(void)
{
	static uint8_t menu_cnt = 0;
	uint8_t re = 0;
	uint8_t set_re = 0;
	uint8_t (*xfunc_out)(void);	/* Pointer to the output stream */
	
	xfunc_out = (uint8_t (*)(void))setting_func[menu_cnt];
	set_re = xfunc_out();
	switch(set_re)
	{
		case 1:
			if(menu_cnt == 0)
				menu_cnt = SETTING_FUNC_MAX-1;
			else
				menu_cnt--;
			break;
		
		case 2:
			menu_cnt++;
			menu_cnt = menu_cnt% SETTING_FUNC_MAX;
			break;
			
		case 3:
			menu_cnt = 0;
			re = 1;
			break;
	}
	return re;
}


uint8_t setting_normal1			(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_normal_sv,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1LEL",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_normal_sv,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}


uint8_t setting_normal2			(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_normal_sv,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2LEL",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_normal_sv,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}
uint8_t setting_hsens1		(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_hsens_sv,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1ErA",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_hsens_sv,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > ER_MAX)
			ad = ER_MIN;
		if(ad < ER_MIN)
			ad = ER_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_hsens2		(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_hsens_sv,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2ErA",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_hsens_sv,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > ER_MAX)
			ad = ER_MIN;
		if(ad < ER_MIN)
			ad = ER_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_csens1			(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_csens_sv,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1Erb",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_csens_sv,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > ER_MAX)        // ER_MAX 2047
			ad = ER_MIN;
		if(ad < ER_MIN)
			ad = ER_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_csens2			(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_csens_sv,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2Erb",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_csens_sv,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > ER_MAX)
			ad = ER_MIN;
		if(ad < ER_MIN)
			ad = ER_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_min1(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_sv_min,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1H2L",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_sv_min,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_min2(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_sv_min,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2H2L",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_sv_min,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_sv_max1(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_sv_max,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1H2H",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_sv_max,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_sv_max2(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_sv_max,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2H2H",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_sv_max,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}


uint8_t setting_broken_sv1(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_broken_sv,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1Brk",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_broken_sv,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}


uint8_t setting_broken_sv2(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_broken_sv,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2Brk",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_broken_sv,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}


uint8_t setting_short_sv1(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_short_sv,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1LEH",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_short_sv,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}


uint8_t setting_short_sv2(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_short_sv,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2LEH",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_short_sv,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > AD_MAX)
			ad = AD_MIN;
		if(ad < AD_MIN)
			ad = AD_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_dt1(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_dt,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1LDT",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_dt,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > TIME_MAX)
			ad = TIME_MIN;
		if(ad < TIME_MIN)
			ad = TIME_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_dt2(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_dt,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2LDT",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_dt,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > TIME_MAX)
			ad = TIME_MIN;
		if(ad < TIME_MIN)
			ad = TIME_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_snr_pt1(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_pwoff_t,0);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"1LPT",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_pwoff_t,0,ad);
			enter = 0;
			break;			
		}
		
		if(ad > TIME_MAX)
			ad = TIME_MIN;
		if(ad < TIME_MIN)
			ad = TIME_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}
uint8_t setting_snr_pt2(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data_ch(e_snr_pwoff_t,1);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"2LPT",4);
		fnd_put_point(0);
	}
	else
	{
		
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data_ch(e_snr_pwoff_t,1,ad);
			enter = 0;
			break;			
		}
		
		if(ad > TIME_MAX)
			ad = TIME_MIN;
		if(ad < TIME_MIN)
			ad = TIME_MAX;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}


uint8_t setting_h2o_rel(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	key = hal_get_key();
	
	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_H2O_REL_USE);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"H2or",4);
	}
	else
	{
		switch(key)
		{
			
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_H2O_REL_USE,chk);

			enter = 0;
			break;
			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"On  ",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"OFF ",4);
		}
	}
	return re;
}

uint8_t setting_broken_chk(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	
	key = hal_get_key();

	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_BROKEN_USE);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"broC",4);
	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG7|FND_BLINK_DIG6|FND_BLINK_DIG5|FND_BLINK_DIG4);
		switch(key)
		{			
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_BROKEN_USE,chk);

			enter = 0;
			break;
			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"On  ",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"OFF ",4);
		}
	}
	
	
	return re;
}
uint8_t setting_short_chk(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	
	key = hal_get_key();

	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_SHORT_USE);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"ShtC",4);
	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG7|FND_BLINK_DIG6|FND_BLINK_DIG5|FND_BLINK_DIG4);
		switch(key)
		{			
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_SHORT_USE,chk);

			enter = 0;
			break;			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"On  ",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"OFF ",4);
		}
	}
	
	
	return re;
}

uint8_t setting_h2o_dry			(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	
	key = hal_get_key();
	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_H2O_DRY_USE);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"Hdry",4);
	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG7|FND_BLINK_DIG6|FND_BLINK_DIG5|FND_BLINK_DIG4);
		switch(key)
		{
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_H2O_DRY_USE,chk);
			enter = 0;
			break;
			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"On  ",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"OFF ",4);
		}
	}
	
	
	return re;	
}

uint8_t setting_buzzer(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	
	key = hal_get_key();
	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_BUZZER_ON);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"Sond",4);
	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG7|FND_BLINK_DIG6|FND_BLINK_DIG5|FND_BLINK_DIG4);
		switch(key)
		{
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_BUZZER_ON,chk);
			beep_on(get_sys_config_bit(SYS_BUZZER_ON));

			enter = 0;
			break;
			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"On  ",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"OFF ",4);
		}
	}
	
	
	return re;
}

uint8_t setting_factort_reset(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static int32_t ad = 0;
	uint8_t adtxt[4];	
	static uint8_t enter = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t i;

	
	key = hal_get_key();
	if(enter == 0 )
	{
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			dignum = 0;
			enter = 1;
			break;
		}
		fnd_put_line((uint8_t *)"FECt",4);

	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:
			if(ad == 510u)
			{
				format_eeprom(0);			
				WDT_Configure();
				WDT_ResetRun();
			}
			else	
			{
				ad = 0;
				enter = 0;
			}
			break;			
		}
		itoa(ad,adtxt,4,10);
		for(i = 0; i < 4; i++ )
		{
			if(i != dignum)
			{
				adtxt[3-i] = '-';
			}
		}
		fnd_put_line((uint8_t *)adtxt,4);		
	}
	
	return re;
}
uint8_t setting_recovery_md(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	
	key = hal_get_key();
	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_AUTO_CLEAR_USE);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;
			
		}
		fnd_put_line((uint8_t *)"rECS",4);	

	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG7|FND_BLINK_DIG6|FND_BLINK_DIG5|FND_BLINK_DIG4);
		switch(key)
		{
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_AUTO_CLEAR_USE,chk);

			enter = 0;
			break;			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"Auto",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"Hold",4);
		}
	}
	
	return re;
}
uint8_t setting_exit(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	fnd_put_line((uint8_t *)"EXIT",4);
	key = hal_get_key();
	switch(key)
	{
		case KEY_R_SHORT:
		re = 2;
		break;
		case KEY_E_SHORT:
		WDT_Configure();
		WDT_ResetRun();
		re = 3;
		break;
		
	}
	return re;
}

uint8_t setting_modbus_v(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint16_t ver=0;
	static uint8_t enter = 0;
	
	key = hal_get_key();
	if(enter == 0 )
	{
		ver = get_app_data_ch(e_sys_modebus_v,0);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;
			
		}
		fnd_put_line((uint8_t *)"M- V",4);	

	}
	else
	{
		fnd_set_blink(FND_BLINK_DIG7|FND_BLINK_DIG6|FND_BLINK_DIG5|FND_BLINK_DIG4);
		switch(key)
		{
			case KEY_R_SHORT:
			ver ++;
			ver = ver % 3;
			break;
			case KEY_E_LONG:    
            set_app_data_ch(e_sys_modebus_v,0,ver);
			enter = 0;
			break;			
		}
		switch(ver)
		{
			case 0:
			fnd_put_line((uint8_t *)"  v1",4);
			break;
			case 1:
			fnd_put_line((uint8_t *)"  v2",4);
			break;
			case 2:
			fnd_put_line((uint8_t *)"  v3",4);
			break;

		}
	}
	
	return re;
}

uint8_t setting_modbus_id(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t enter = 0;
	static int32_t ad = 0;
	static uint8_t digit = 0;
	static int8_t dignum = 0;
	uint8_t adtxt[4];	
	key = hal_get_key();
	if(enter == 0)
	{
		dignum = 0;
		ad = get_app_data(e_sys_ID_v);
		digit = ad % 10;
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"M-ID",4);
	}
	else
	{	
		fnd_set_blink(FND_BLINK_DIG3>>dignum);
		switch(key)
		{
			case KEY_E_SHORT:
			digit = (ad/pow10[dignum])%10;
			ad = ad - (pow10[dignum]*digit);
			digit = (++digit)%10;
			ad = ad + (pow10[dignum]*digit);
			break;
			case KEY_R_SHORT:
			dignum++;
			dignum = dignum % 4;

			break;
			
			case KEY_E_LONG:

			set_app_data(e_sys_ID_v,ad);
			enter = 0;
			break;			
		}
		
		if(ad > 255)
			ad = 1;
		if(ad < 1)
			ad = 255;

		itoa(ad,adtxt,4,10);
		fnd_put_line(adtxt,4);
	}
	return re;
}

uint8_t setting_relay_out_md(void)
{
	uint8_t re=0;
	uint8_t key = 0;
	static uint8_t chk=0;
	static uint8_t enter = 0;
	key = hal_get_key();
	
	if(enter == 0 )
	{
		chk = get_sys_config_bit(SYS_RELAY_MD);
		fnd_set_blink(0);
		switch(key)
		{
			case KEY_R_SHORT:
			re = 2;
			break;
			case KEY_E_SHORT:
			enter = 1;
			break;			
		}
		fnd_put_line((uint8_t *)"ReLy",4);
	}
	else
	{
		switch(key)
		{
			
			case KEY_R_SHORT:
			chk ++;
			chk = chk % 2;
			break;
			case KEY_E_LONG:
			set_sys_config_bit(SYS_RELAY_MD,chk);

			enter = 0;
			break;
			
		}
		if(chk)
		{
			fnd_put_line((uint8_t *)"No  ",4);
		}
		else
		{
			fnd_put_line((uint8_t *)"Nc  ",4);
		}
	}
	
	return re;
}
