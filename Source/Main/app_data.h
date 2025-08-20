
/***************************************************************************//**
* @file     app_data.h
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

#ifndef __APP_DATA_H
#define __APP_DATA_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "hal.h"
#include "sensor.h"

typedef enum _eeprom_index 
{    // 열거형 정의
  e_sys_ver           = 0,								//0
  e_sys_ID_v          = e_sys_ver           +1,         //0+1 = 1            
  e_sys_snr_max_v     = e_sys_ID_v          +1,         //0+1 = 2
  e_normal_sv         = e_sys_snr_max_v     +1,   			//0+1 = 3
  e_hsens_sv          = e_normal_sv         +SNR_NUM,   //0+1 = 30         5   SNR_NUM == 2
  e_csens_sv          = e_hsens_sv          +SNR_NUM,   //0+1 = 40		7
  e_snr_sv_min     	  = e_csens_sv          +SNR_NUM,   //0+1 = 50
  e_snr_sv_max        = e_snr_sv_min        +SNR_NUM,   //0+1 =60
  e_snr_dt            = e_snr_sv_max        +SNR_NUM,   //0+1 =70
  e_snr_pwoff_t       = e_snr_dt            +SNR_NUM,   //0+1 =80
  e_broken_sv         = e_snr_pwoff_t       +SNR_NUM,   //0+1 =90
  e_short_sv          = e_broken_sv         +SNR_NUM,   //0+1 =100
  e_sys_config_value  = e_short_sv          +SNR_NUM,   //0+1 =110
  e_sys_modebus_v     = e_sys_config_value  +1,         //0+1 =111
  e_sys_endmode_v     = e_sys_modebus_v     +1,         //0+1 =111
  e_sys_magic_code    = e_sys_endmode_v     +1          //0+1 =112
  
}eeprom_index;

#define APP_DATA_MAX e_sys_magic_code+1

#define  SYS_BUZZER_ON     0x01/* Bit0 : Buzzer On(1) / Offer(0) */ 
#define  SYS_RELAY_MD     0x02/* Bit1 : 릴레이NO(1)/NC(0) */
#define  SYS_H2O_REL_USE   0x04/* Bit2 : 물감지 릴레이 사용(1) / 미사용(0) */
#define  SYS_BROKEN_USE     0x08/* Bit3 : 센서파손감지(1) / 미감지(0) */
#define  SYS_SHORT_USE     0x10/* Bit4 : 센서숏트감지(1) / 미감지(0)*/
#define  SYS_H2O_DRY_USE   0x20/* Bit5 : 물자연건조감지(1) / 미감지(0)*/
#define  SYS_AUTO_CLEAR_USE   0x40/* Bit6 시스템 경고 해제 Auto(1) / Manual(0) */

//#define  SNR_ST_NORMAL     1
#define  SNR_ST_NORMAL     1
#define  SNR_ST_LEAK     2
#define  SNR_ST_H2O       3
#define  SNR_ST_SHORT     4
#define  SNR_ST_BROKEN     5
#define  SNR_ST_DETECT     9
#define  SNR_ST_SNR_OFF 		0x80


uint16_t get_app_data_ch(uint8_t index,uint8_t ch_num);
uint16_t get_app_data(uint8_t index);

uint16_t set_app_data(uint8_t index, uint16_t v);
uint16_t set_app_data_ch(uint8_t index,uint8_t ch_num, uint16_t v);

uint8_t set_sys_config_bit(uint8_t bit , uint8_t v);
uint8_t get_sys_config_bit(uint8_t ck);
uint8_t set_sys_config(uint8_t v);
uint8_t get_sys_config(void);

uint8_t save_data(eeprom_index index, uint32_t data);
uint32_t load_data(eeprom_index index);

uint8_t cheak_data(void);
uint8_t format_eeprom(uint8_t mode);

uint8_t load_all_data(void);
void app_data_reset(uint8_t mode);

uint8_t set_sys_mode(uint8_t v);
uint8_t get_sys_mode(void);

#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */

