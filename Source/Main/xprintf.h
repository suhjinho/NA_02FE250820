/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */
/* # 파일명 : xprintf.h                                                       */
/*                                                                          */
/* # 내역: 시리얼 통신관련 드라이브용 헤더파일                              */
/* 		1.                                                                  */
/*		2.                                                                  */
/* # Chip type : PIC24FJ64GA008 (Device ID Revision = 0x3044)               */
/* # Ext. X-tal frequency : 32MHz                                           */
/* # Environment : MPLAB X IDE v4.05,  Microchip XC16 v1.33                 */
/* # Text Encoding : x-windows-949                                          */
/* # Modification History                                                   */
/*   -------------------------------------------------------------------    */
/*   No. Ver     Date        by who      Trouble No. (Change Request No.)   */
/*   -------------------------------------------------------------------    */
/*   01  Universal string handler for user console interface  (C)ChaN, 2011 */   
/*   02  0.0.1   2018/01/08  IC.Park Modified                                */
/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */

#ifndef _xprintf_h 
#define _xprintf_h 

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */
#define	_LINE_ECHO		0	/* 1: Echo back input chars in xgets function */

#if _USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)
extern void (*xfunc_out)(unsigned char);
void xputc (char c);
void xputs (const char* str);
void xfputs (void (*func)(unsigned char), const char* str);
void xprintf (const char* fmt, ...);
void xsprintf (char* buff, const char* fmt, ...);
void xfprintf (void (*func)(unsigned char), const char*	fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#endif
