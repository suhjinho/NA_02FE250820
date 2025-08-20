/***************************************************************************//**
* @file     queue.h
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

#ifndef __QUEUE_H
#define __QUEUE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
void addq(uint8_t value);
uint8_t deleteq(uint8_t *value);
#ifdef __cplusplus
}
#endif

#endif   /*__MODBUD_H */

