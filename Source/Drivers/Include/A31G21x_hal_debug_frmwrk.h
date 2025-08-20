/***************************************************************************//**
* @file     A31G21x_hal_debug_frmwrk.h
* @brief    Contains all macro definitions and function prototypes
*           support for debug_frmwrk firmware library on A31G21x
*
*
* @version  1.01
* @date     2022-01-10
* @author   ABOV MDS Team
*
* Copyright(C) 2022, ABOV Semiconductor
* All rights reserved.
*
********************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements,
*modifications, and improvements to ABOV products and/or to this document at any time without notice.
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders.
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and
*ABOV assumes no liability for application assistance or the design of purchasers' products. No license,
*express or implied, to any intellectual property rights is granted by ABOV herein.
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications.
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners.
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
*//****************************************************************************/

#ifndef _DEBUG_FRMWRK_H_
#define _DEBUG_FRMWRK_H_

#include "A31G21x_hal_uartn.h"
#include "A31G21x_hal_usart1n.h"
#include "A31G21x_hal_libcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG_MSG

//******************************************************************************
// Constant
//******************************************************************************

//#define USED_UART_DEBUG_PORT     1

#if (USED_UART_DEBUG_PORT == 0)
	#define DEBUG_UART_PORT		UART0
	#define DEBUG_UART_TXD_PORT	PB			// PB5   SWDIO    0: SEG36       1: RXD0        2: SWDIO       3: ----        4: ----
	#define DEBUG_UART_TXD_PIN	5
	#define DEBUG_UART_TXD_AF	FUNC1
	#define DEBUG_UART_RXD_PORT	PB			// PB4   SWCLK    0: SEG37       1: TXD0        2: SWCLK       3: ----        4: ----
	#define DEBUG_UART_RXD_PIN	4
	#define DEBUG_UART_RXD_AF	FUNC1
#elif (USED_UART_DEBUG_PORT == 1)
	#define DEBUG_UART_PORT		UART1
	#define DEBUG_UART_TXD_PORT	PB			// PB7            0: SEG34       1: RXD1        2: ----        3: ----        4: ----
	#define DEBUG_UART_TXD_PIN	7
	#define DEBUG_UART_TXD_AF	FUNC1
	#define DEBUG_UART_RXD_PORT	PB			// PB6            0: SEG35       1: TXD1        2: ----        3: ----        4: ----
	#define DEBUG_UART_RXD_PIN	6
	#define DEBUG_UART_RXD_AF	FUNC1
#elif (USED_UART_DEBUG_PORT == 10)
	#define DEBUG_UART_PORT		USART10
	#define DEBUG_UART_TXD_PORT	PB			// PB1            0: SEG40       1: RXD10       2: ----        3: ----        4: ----
	#define DEBUG_UART_TXD_PIN	1
	#define DEBUG_UART_TXD_AF	FUNC1
	#define DEBUG_UART_RXD_PORT	PB			// PB0            0: SEG41       1: TXD10       2: ----        3: ----        4: ----
	#define DEBUG_UART_RXD_PIN	0
	#define DEBUG_UART_RXD_AF	FUNC1
#elif (USED_UART_DEBUG_PORT == 11)
	#define DEBUG_UART_PORT		USART11
	#define DEBUG_UART_TXD_PORT	PD			// PD3            0: SEG9        1: RXD11       2: ----        3: ----        4: ----
	#define DEBUG_UART_TXD_PIN	3
	#define DEBUG_UART_TXD_AF	FUNC1
	#define DEBUG_UART_RXD_PORT	PD			// PD2            0: SEG10       1: TXD11       2: ----        3: ----        4: ----
	#define DEBUG_UART_RXD_PIN	2
	#define DEBUG_UART_RXD_AF	FUNC1
#elif (USED_UART_DEBUG_PORT == 12)
	#define DEBUG_UART_PORT		USART12
	#define DEBUG_UART_TXD_PORT	PE			// PE13          0:             1: RXD12       2: ----        3: ----        4: ----
	#define DEBUG_UART_TXD_PIN	13			
	#define DEBUG_UART_TXD_AF	FUNC1
	#define DEBUG_UART_RXD_PORT	PE			// PE12          0:             1: TXD12       2: ----        3: ----        4: ----
	#define DEBUG_UART_RXD_PIN	12
	#define DEBUG_UART_RXD_AF	FUNC1
#elif (USED_UART_DEBUG_PORT == 13)
	#define DEBUG_UART_PORT		USART13
	#define DEBUG_UART_TXD_PORT	PE			// PE9            0:             1: RXD13       2: ----        3: ----        4: ----
	#define DEBUG_UART_TXD_PIN	9
	#define DEBUG_UART_TXD_AF	FUNC1
	#define DEBUG_UART_RXD_PORT	PE			// PE8            0:             1: TXD13       2: ----        3: ----        4: ----
	#define DEBUG_UART_RXD_PIN	8
	#define DEBUG_UART_RXD_AF	FUNC1
#endif

//******************************************************************************
// Macro
//******************************************************************************

#define _DBG(x)       _db_msg(x)
#define _DBG_(x)      _db_msg_(x)
#define _DBC(x)       _db_char(x)
#define _DBD(x)       _db_dec(x)
#define _DBD16(x)     _db_dec_16(x)
#define _DBD32(x)     _db_dec_32(x)
#define _DBH(x)       _db_hex(x)
#define _DBH16(x)     _db_hex_16(x)
#define _DBH32(x)     _db_hex_32(x)
#define _DG()         _db_get_char()
#define _DG_(x)       _db_get_ch(x)

//******************************************************************************
// Variable
//******************************************************************************

extern void (*_db_msg)(const void* s);
extern void (*_db_msg_)(const void* s);
extern void (*_db_char)(uint8_t ch);
extern void (*_db_dec)(uint8_t decn);
extern void (*_db_dec_16)(uint16_t decn);
extern void (*_db_dec_32)(uint32_t decn);
extern void (*_db_hex)(uint8_t hexn);
extern void (*_db_hex_16)(uint16_t hexn);
extern void (*_db_hex_32)(uint32_t hexn);
extern uint8_t (*_db_get_char)(void);
extern uint8_t (*_db_get_ch)(uint8_t* ch);

//******************************************************************************
// Function
//******************************************************************************

void dPutChar(uint8_t ch);
void dPuts(const void* str);
void dPuts_(const void* str);
void dPutDec(uint8_t decnum);
void dPutDec16(uint16_t decnum);
void dPutDec32(uint32_t decnum);
void dPutHex(uint8_t hexnum);
void dPutHex16(uint16_t hexnum);
void dPutHex32(uint32_t hexnum);
uint8_t dGetChar(void);
uint8_t dGetCh(uint8_t* ch);

void cprintf(const char* format, ...);

void debug_frmwrk_init(void);
uint8_t getstring(void);

#endif   /* _DEBUG_MSG */

#ifdef __cplusplus
}
#endif

#endif   /* _DEBUG_FRMWRK_H_ */

