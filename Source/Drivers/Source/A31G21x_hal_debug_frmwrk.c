#include <stdarg.h>
#include <stdio.h>
#include "A31G21x_hal_debug_frmwrk.h"
#include "A31G21x_hal_pcu.h"

#define ASCII_BACKSPACE          (0x08)
#define ASCII_LINEFEED           (0x0A)
#define ASCII_CARRIAGE_RETURN    (0x0D)

#ifdef _DEBUG_MSG

void (*_db_msg)(const void* s);
void (*_db_msg_)(const void* s);
void (*_db_char)(uint8_t ch);
void (*_db_dec)(uint8_t decn);
void (*_db_dec_16)(uint16_t decn);
void (*_db_dec_32)(uint32_t decn);
void (*_db_hex)(uint8_t hexn);
void (*_db_hex_16)(uint16_t hexn);
void (*_db_hex_32)(uint32_t hexn);
uint8_t (*_db_get_char)(void);
uint8_t (*_db_get_ch)(uint8_t* ch);

#if 0
int fputc(int ch, FILE* f)
{
   while(HAL_UART_CheckBusy((DEBUG_UART_PORT));
   HAL_UART_TransmitByte(DEBUG_UART_PORT, ch);

   return(ch);
}
#endif

void dPutChar(uint8_t ch)
{
#if (USED_UART_DEBUG_PORT == 0) || (USED_UART_DEBUG_PORT == 1)
   HAL_UART_Transmit(DEBUG_UART_PORT, &ch, 1);
#elif (USED_UART_DEBUG_PORT == 10) || (USED_UART_DEBUG_PORT == 11)
   HAL_USART_Transmit(DEBUG_UART_PORT, &ch, 1);
#endif
}

uint8_t dGetChar(void)
{
   uint8_t     tmp = 0;

#if (USED_UART_DEBUG_PORT == 0) || (USED_UART_DEBUG_PORT == 1)
   HAL_UART_Receive(DEBUG_UART_PORT, &tmp, 1);
#elif (USED_UART_DEBUG_PORT == 10) || (USED_UART_DEBUG_PORT == 11)
   HAL_USART_Receive(DEBUG_UART_PORT, &tmp, 1);
#endif
   return(tmp);
}

uint8_t dGetCh(uint8_t* ch)
{
#if (USED_UART_DEBUG_PORT == 0) || (USED_UART_DEBUG_PORT == 1)
   if (!(DEBUG_UART_PORT->LSR & UART0_LSR_DR_Msk))
   {
      *ch = 0;
      return(0);
   }
   else
   {
      *ch = HAL_UART_ReceiveByte((UARTn_Type*)DEBUG_UART_PORT);
      return(1);
   }
#elif (USED_UART_DEBUG_PORT == 10) || (USED_UART_DEBUG_PORT == 11)
   if (!(DEBUG_UART_PORT->ST & USART10_ST_DREn_Msk))
   {
      *ch = 0;
      return(0);
   }
   else
   {
      *ch = HAL_USART_ReceiveByte(DEBUG_UART_PORT);
      return(1);
   }
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a string to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     str
 *                   String to put
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPuts(const void* str)
{
   uint8_t*    s = (uint8_t*)str;

   while(*s)
   {
      dPutChar(*s++);
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a string to UART port and print new line
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     str
 *                   String to put
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPuts_(const void* str)
{
   dPuts(str);
   dPuts("\n\r");
}

void dPutDecn(uint32_t decnum, int n)
{
	int		i;
	char	d[11];
	d[n] = 0;
	for (i = n - 1; i >= 0; i++)
	{
		d[i] = (decnum % 10) + '0';
		decnum /= 10;
	}	
	dPuts(d);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a decimal number to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     decnum
 *                   Decimal number (8-bit long)
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPutDec(uint8_t decnum)
{
	dPutDecn(decnum, 3);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a decimal number to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     decnum
 *                   Decimal number (8-bit long)
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPutDec16(uint16_t decnum)
{
	dPutDecn(decnum, 5);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a decimal number to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     decnum
 *                   Decimal number (8-bit long)
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPutDec32(uint32_t decnum)
{
	dPutDecn(decnum, 10);
}


/*-------------------------------------------------------------------------*//**
 * @brief         Puts a hex number to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     hexnum
 *                   Hex number (8-bit long)
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPutHex(uint8_t hexnum)
{
   uint8_t     nibble, i;

   i = 1;
   do
   {
      nibble = (hexnum >> (4 * i)) & 0x0F;
      dPutChar((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
   } while(i--);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a hex number to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     hexnum
 *                   Hex number (16-bit long)
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPutHex16(uint16_t hexnum)
{
   uint8_t     nibble, i;

   i = 3;
   do
   {
      nibble = (hexnum >> (4 * i)) & 0x0F;
      dPutChar((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
   } while(i--);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Puts a hex number to UART port
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     hexnum
 *                   Hex number (32-bit long)
 * @return        None
 *//*-------------------------------------------------------------------------*/
void dPutHex32(uint32_t hexnum)
{
   uint8_t     nibble, i;

   i = 7;
   do
   {
      nibble = (hexnum >> (4 * i)) & 0x0F;
      dPutChar((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
   } while(i--);
}

/*-------------------------------------------------------------------------*//**
 * @brief         print function that supports format as same as printf() function of <stdio.h> library
 * @param[in]     format
 *                   formatted string to be print
 * @return        None
 *//*-------------------------------------------------------------------------*/
void cprintf(const char* format, ...)
{
   char        buffer[512 + 1];
   va_list     vArgs;

   va_start(vArgs, format);
   vsprintf((char*)buffer, (char const*)format, vArgs);
   va_end(vArgs);

   _DBG(buffer);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initializes Debug Framework through initializing UARTn
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void debug_frmwrk_init(void)
{
   // Initialize UART/USART pin connect
   HAL_GPIO_ConfigOutput(DEBUG_UART_TXD_PORT, DEBUG_UART_TXD_PIN, ALTERN_FUNC);
   HAL_GPIO_ConfigFunction(DEBUG_UART_TXD_PORT, DEBUG_UART_TXD_PIN, DEBUG_UART_TXD_AF);

   HAL_GPIO_ConfigOutput(DEBUG_UART_RXD_PORT, DEBUG_UART_RXD_PIN, ALTERN_FUNC);
   HAL_GPIO_ConfigFunction(DEBUG_UART_RXD_PORT, DEBUG_UART_RXD_PIN, DEBUG_UART_RXD_AF);

   /* Initialize UART Configuration parameter structure to default state:
    * Baudrate = 38400bps
    * 8 data bit
    * no parity
    * 1 stop bit
    */
#if (USED_UART_DEBUG_PORT == 0) || (USED_UART_DEBUG_PORT == 1)
	{
		UART_CFG_Type    UARTn_Config;

		HAL_UART_ConfigStructInit(&UARTn_Config);
		UARTn_Config.Baudrate = 38400;

		// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
		HAL_UART_Init(DEBUG_UART_PORT, &UARTn_Config);
	}
#elif (USED_UART_DEBUG_PORT == 10) || (USED_UART_DEBUG_PORT == 11) || (USED_UART_DEBUG_PORT == 12) || (USED_UART_DEBUG_PORT == 13)
	{
		USART_CFG_Type    USART1n_Config;

		HAL_USART_UART_Mode_Config(&USART1n_Config);
		USART1n_Config.Baud_rate = 38400;

		// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
		HAL_USART_Init(DEBUG_UART_PORT, &USART1n_Config);
		HAL_USART_Enable(DEBUG_UART_PORT, ENABLE);
	}
#endif

   _db_msg  = dPuts;
   _db_msg_ = dPuts_;
   _db_char = dPutChar;
   _db_hex = dPutHex;
   _db_hex_16 = dPutHex16;
   _db_hex_32 = dPutHex32;
   _db_dec = dPutDec;
   _db_dec_16 = dPutDec16;
   _db_dec_32 = dPutDec32;
   _db_get_char = dGetChar;
   _db_get_ch = dGetCh;
}

#endif   /* _DEBUG_MSG */

