/***************************************************************************//**
* @file     led.h
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

#ifndef __LED_H
#define __LED_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#define RED_LED 0x01
#define GREEN_LED 0x02
#define ORANGE_LED 0x03

void set_led(uint8_t led);
void set_led_blink(uint8_t blink);
uint8_t get_led(void);
void led_loop(void);

#ifdef __cplusplus
}
#endif

#endif   /*__LED_H */
