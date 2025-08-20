#include "main_conf.h"
#include "Init.h"
#include "xprintf.h"
#include "hal.h"
#include "fnd.h"
#include "key.h"
#include "sound.h"
#include "app_ctl.h"
#include "app_data.h"

void DEBUG_Init( void );
void DEBUG_MenuPrint( void );

volatile static uint16_t    msec_1ms;
volatile static uint16_t    msec_10ms;
volatile static uint16_t    msec_100ms;
volatile static uint16_t    msec_1000ms;

void SysTick_ExceptionHandler_IT( void )
{
	if( msec_1ms > 0 )
	{
		msec_1ms--;
	}
	if( msec_10ms > 0 )
	{
		msec_10ms--;
	}
	if( msec_100ms > 0 )
	{
		msec_100ms--;
	}
	if( msec_1000ms > 0 )
	{
		msec_1000ms--;
	}
	decrease_msdelay1();
}

int main( void )
{
	Init();

   SysTick_Config( SystemCoreClock / 1000 );

   __enable_irq();

  msec_1ms = 1;
	msec_10ms = 10;
	msec_100ms = 100;
	msec_1000ms = 1000;
	task_start();
	while (1)
	{
		if(msec_1ms == 0)
		{
			task_1ms();
			msec_1ms = 1;
		}
		if(msec_10ms == 0)
		{
			task_10ms();
			msec_10ms = 10;			
		}
		if(msec_100ms == 0)
		{
			task_100ms();
			msec_100ms = 100;			
		}
		if(msec_1000ms == 0)
		{
			task_1000ms();
			msec_1000ms = 1000;			
		}
		task_idle();		
	}
}


#ifdef  USE_FULL_ASSERT
void check_failed( uint8_t* file, uint32_t line )
{
   while( 1 )
   {
   }
}
#endif
