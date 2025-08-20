/***************************************************************************//**
* @file     app_setting.h
*
*
* @version  1.00
* @date     2022-05-02
* @author   RND
*
* Copyright(C) 2022, amers
* All rights reserved.
*
*//****************************************************************************/

#ifndef __APP_SETTING_H
#define __APP_SETTING_H
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct __setting_menu {   // 구조체 이름은 _Person
    char menu_name[8];            // 구조체 멤버 1
    void * setting_func;
} s_setting_menu;                  // typedef를 사용하여 구조체 별칭을 Person으로 정의


#define SETTING_FUNC_MAX 	25

extern void * setting_func[];
uint8_t setting(void);
uint8_t setting_hsens1				(void);
uint8_t setting_csens1				(void);
uint8_t setting_normal1				(void);
uint8_t setting_snr_min1			(void);
uint8_t setting_snr_sv_max1		(void);
uint8_t setting_snr_dt1				(void);
uint8_t setting_snr_pt1				(void);

uint8_t setting_hsens2				(void);
uint8_t setting_csens2				(void);
uint8_t setting_normal2				(void);
uint8_t setting_snr_min2			(void);
uint8_t setting_snr_sv_max2		(void);
uint8_t setting_snr_dt2				(void);
uint8_t setting_snr_pt2				(void);

uint8_t setting_h2o_rel				(void);
uint8_t setting_broken_chk		(void);
uint8_t setting_broken_sv1		(void);
uint8_t setting_broken_sv2		(void);
uint8_t setting_short_chk			(void);
uint8_t setting_short_sv1			(void);
uint8_t setting_short_sv2			(void);
uint8_t setting_buzzer				(void);
uint8_t setting_recovery_md		(void);
uint8_t setting_relay_out_md		(void);
uint8_t setting_factort_reset	(void);
uint8_t setting_exit					(void);
uint8_t setting_h2o_dry				(void); 
uint8_t setting_debug_mode		(void);
uint8_t setting_modbus_v			(void);
uint8_t setting_modbus_id			(void);
#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */

