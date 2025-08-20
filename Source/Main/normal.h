
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

#ifndef __APP_SETTING_H
#define __APP_SETTING_H
#ifdef __cplusplus
extern "C"
{
#endif



volatile static uint16_t   snrLeakSVmin,   snrLeakSVmax, snrLeakDT,   snrLeakDelayCnt;
volatile static uint16_t   snrH2oSV,   snrH2oPT;
volatile static int16_t    snrH2oPwrOffCnt;
volatile static uint16_t   snrBrokenSV, snrBrokenDT, snrBrokenDelayCnt;

#ifdef __cplusplus
}
#endif

#endif   /*__FND_H */

