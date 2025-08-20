
/***************************************************************************//**
* @file     app_normal.h
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

#ifndef __APP_NORMAL_H
#define __APP_NORMAL_H
#ifdef __cplusplus
extern "C"
{
#endif
void update_st_error_t(void);
uint8_t get_snr_state_modbud(uint8_t ch);
void normal_timer(void);//1s
void update_st_relay(void);
uint8_t normal(void);
void update_st_data(void);
uint8_t get_auto_rcv_off(void);
#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */

