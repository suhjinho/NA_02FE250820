/***************************************************************************//**
* @file     key.h
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

#ifndef __KEY_H
#define __KEY_H
#ifdef __cplusplus
extern "C"
{
#endif


#define KEY_D_SHORT	0x01
#define KEY_R_SHORT	0x02
#define KEY_E_SHORT	0x03
#define KEY_U_SHORT	0x04
#define KEY_L_SHORT	0x05

#define KEY_D_DOUBLE	0x21
#define KEY_R_DOUBLE	0x22
#define KEY_E_DOUBLE	0x23
#define KEY_U_DOUBLE	0x24
#define KEY_L_DOUBLE	0x25

#define KEY_D_LONG	0x41
#define KEY_R_LONG 	0x42
#define KEY_E_LONG 	0x43
#define KEY_U_LONG 	0x44
#define KEY_L_LONG 	0x45


#define KEY_D_COUNTINUE	0x81
#define KEY_R_COUNTINUE	0x82
#define KEY_E_COUNTINUE	0x83
#define KEY_U_COUNTINUE	0x84
#define KEY_L_COUNTINUE	0x85

#define KEY_TY_SHORT 	1
#define KEY_TY_LONG 	2
#define KEY_TY_LONGOUT 	3
#define KEY_TY_COUNTINE 4

#define SHORT_PRESS_TIME 5   //50ms
#define SHORT_LONG_TIME 200	//10ms *200 = 2s
#define SHORT_LONGOUT_TIME 100	//10ms *100 = 1s
#define SHORT_COUNTINE_TIME 100	//10ms *10 = 100ms

#define DOUBLE_PRESS_TIME 0
	

uint8_t get_key(void);
void key_loop(void);

#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */
