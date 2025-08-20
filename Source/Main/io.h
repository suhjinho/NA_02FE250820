/***************************************************************************//**
* @file     io.h
*
*
* @version  1.00
* @date     2022-08-19
* @author   RND
*
* Copyright(C) 2022, amers
* All rights reserved.
*
*//****************************************************************************/

#ifndef __IO_H
#define __IO_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void relay_out(uint8_t onoff);
uint8_t set_io_output(uint8_t ch,uint8_t z);
uint8_t get_io_input(uint16_t * data);
void set_io_config(void);
uint8_t get_io_ch(uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif   /*__IO_H */

