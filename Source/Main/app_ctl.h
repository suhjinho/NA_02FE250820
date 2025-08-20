/***************************************************************************//**
* @file     app_ctl.h
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

#ifndef __APP_CTL_H
#define __APP_CTL_H
#ifdef __cplusplus
extern "C"
{
#endif




typedef enum APP_STATE
{
	app_init,
	app_normal,
	app_leak,
	app_leak_alarm,
	app_broken,
	app_broken_alarm,
	app_setting
}e_app_state;

void task_start(void);

void task_1ms(void);
void task_10ms(void);
void task_100ms(void);
void task_1000ms(void);
void task_idle(void);
void task_100us_interrupt(void);
void task_1ms_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */
