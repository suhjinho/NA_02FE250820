/***************************************************************************//**
* @file     fnd.h
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

#ifndef __FND_H
#define __FND_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
#define FND_BLINK_DIG0 		0x01
#define FND_BLINK_DIG1 		0x02
#define FND_BLINK_DIG2 		0x04
#define FND_BLINK_DIG3 		0x08
#define FND_BLINK_DIG4 		0x10
#define FND_BLINK_DIG5 		0x20
#define FND_BLINK_DIG6 		0x40
#define FND_BLINK_DIG7 		0x80
#define FND_BLINK_DIGALL 	0xFF

#define FND_FONT_DP 			0x80

/* Private function prototypes ---------------------------------------------- */


void fnd_put(uint8_t * data,uint8_t size);
void fnd_put_line(uint8_t * data,uint8_t size);
void FND_loop(void);
int itoa(int num, unsigned char* str, int len, int base);
void fnd_set_blink (uint8_t dig);
void fnd_set_brightness(uint8_t bright);
void zero_to_blank(unsigned char* str, int len);
void fnd_put_point(uint8_t row);
void FND_memcpy(uint8_t * dest, const uint8_t * source, uint8_t num);
#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */

