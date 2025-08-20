/***************************************************************************//**
* @file     sound.h
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

#ifndef __SOUND_H
#define __SOUND_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"	
	
void beep_buzzer(void);
void sound_loop(void);
void beep_alram(void);
void beep_stop(void);
void beep_on(uint8_t sw);
void opening_buzzer(void);
uint8_t is_beep(void);
#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */
