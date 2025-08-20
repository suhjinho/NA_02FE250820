#include "main_conf.h"
#include "hal.h"

void HardFault_Handler( void )
{
   /* Go to infinite loop when Hard Fault exception occurs */
   while( 1 )
   {
   }
}

void SysTick_Handler( void )
{
   SysTick_ExceptionHandler_IT();
}

void USART11_IRQHandler(void)
{
	UART11_IRQHandler_IT();
}

void TIMER10_IRQHandler(void)
{
	TIMER10_IRQHandler_IT();
}
