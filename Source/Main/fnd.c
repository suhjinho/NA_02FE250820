#include "hal.h"
#include "fnd.h"
#include "xprintf.h"

#define FND_MAX 4

static uint8_t brightness_max  = FND_MAX;
static uint8_t fnd_data[FND_MAX]  = {0};
static uint8_t fnd_blink  = 0;

static const uint8_t fnd_number_font[10]  = 
{
	0xFC,//	0
	0x60,// 1
	0xDA,// 2
	0xF2,//3
	0x66,// 4
	0xB6,//5
	0xBE,// 6
	0xE0,// 7
	0xFE,// 8
	0xF6// 9
};
static const uint8_t fnd_lower_font[26]  = 
{
	0xEE,//    A
	0x3E,//    B
	0x9C,//    C
	0x7A,//    D
	0x9E,//    E
	0x8E,//    F
	0xBC,//    G
	0x2E,//    H
	0x60,//    I
	0x78,//    J
	0xAE,//    K
	0x1C,//    L
	0x6A,//    M
	0x2A,//    N
	0x3A,//    O
	0xCE,//    P
	0xE6,//    Q
	0x0A,//    R
	0xB6,//    S
	0x1E,//    T
	0x38,//    U
	0x7C,//    V
	0x7E,//    W
	0x92,//    X
	0x76,//    Y
	0xCA,//    Z	
};

static const uint8_t fnd_upper_font[26]  = 
{
	0xEE,//		A
	0x3E,//    B
	0x9C,//    C
	0x7A,//    D
	0x9E,//    E
	0x8E,//    F
	0xBC,//    G
	0x6E,//    H
	0x60,//    I
	0x78,//    J
	0xAE,//    K
	0x1C,//    L
	0x6A,//    M
	0xEC,//    N
	0xFC,//    O
	0xCE,//    P
	0xE6,//    Q
	0x0A,//    R
	0xB6,//    S
	0x1E,//    T
	0x7C,//    U
	0x7C,//    V
	0x7E,//    W
	0x92,//    X
	0x76,//    Y
	0xCA,//    Z	
};


void fnd_set_blink (uint8_t dig)
{
	fnd_blink = dig;
}

void fnd_set_brightness(uint8_t bright)
{
	brightness_max = bright;
}


void fnd_put_point(uint8_t row)
{

	if(row > 4)
		return;
	
	fnd_data[row] = fnd_data[row]|0x01;
}
void fnd_put_line(uint8_t * data,uint8_t size)
{
	int i=0;
	int st=0;
	
	if(size > 4)
		return;
	
	for(i = 0;i < 4;i++)
		fnd_data[st+i] = 0;

	for(i = 0;i<size;i++)
	{
		if(data[i] >= 65 && data[i] <= 90)
		{
			fnd_data[st+i] = fnd_upper_font[data[i]-65];
		}
		else if(data[i] >= 97 && data[i] <= 122)
		{
			fnd_data[st+i] = fnd_lower_font[data[i]-97];
		}
		else if(data[i] >= 48 && data[i] <= 57)
		{
			fnd_data[st+i] = fnd_number_font[data[i]-48];			
		}
		else if(data[i] == '-')
		{
			fnd_data[st+i] = 0x02;
		}
		
		
		else
		{
			fnd_data[st+i] = 0;
		}
	}
}

void fnd_put(uint8_t * data,uint8_t size)
{
	int i=0;

	if(size > FND_MAX)
		return;
	
/*	for(i = 0;i < FND_MAX;i++)
		fnd_data[i] = 0;
	*/

	for(i = 0;i<size;i++)
	{
		if((data[i] & 0x80) == 0x80 )
		{
			uint8_t t_temp = data[i] & 0x7F ;
			if(t_temp >= 65 && t_temp <= 90)
			{
				fnd_data[i] = fnd_upper_font[t_temp-65];
			}
			else if(t_temp >= 97 && t_temp <= 122)
			{
				fnd_data[i] = fnd_lower_font[t_temp-97];
			}
			else if(t_temp >= 48 && t_temp <= 57)
			{
				fnd_data[i] = fnd_number_font[t_temp-48];			
			}
			else
			{
				fnd_data[i] = 0;
			}
			fnd_data[i] = fnd_data[i] | 0x01;
		}
		else
		{
			uint8_t t_temp = data[i];
			if(t_temp >= 65 && t_temp <= 90)
			{
				fnd_data[i] = fnd_upper_font[t_temp-65];
			}
			else if(t_temp >= 97 && t_temp <= 122)
			{
				fnd_data[i] = fnd_lower_font[t_temp-97];
			}
			else if(t_temp >= 48 && t_temp <= 57)
			{
				fnd_data[i] = fnd_number_font[t_temp-48];			
			}
			else
			{
				fnd_data[i] = 0;
			}
		}
		
		
	}
}


void FND_memcpy(uint8_t * dest, const uint8_t * source, uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		dest[i] = source[i];
	}	
}


void FND_Seg(uint8_t data)
{
	SEG_ALL_HIGH;
	if(data & 0x01)
	{
		SEG7_LOW;
	}
	
	if(data & 0x02)
	{
		SEG6_LOW;
	}
	
	if(data & 0x04)
	{
		SEG5_LOW;
	}
	
	if(data & 0x08)
	{
		SEG4_LOW;
	}
	
	if(data & 0x10)
	{
		SEG3_LOW;
	}
	
	if(data & 0x20)
	{
		SEG2_LOW;
	}
	
	if(data & 0x40)
	{
		SEG1_LOW;
	}
	
	if(data & 0x80)
	{
		SEG0_LOW;
	}
}

void FND_loop(void)
{
	static uint8_t com_loop = 0;
	static uint16_t blink_cnt = 0;
	static uint8_t tblink = 0;
	if(fnd_blink)
	{
		if(blink_cnt-- == 0)
		{
			tblink = !tblink;
			blink_cnt = 500;
		}
	}
	else
	{
		tblink = 0;
		blink_cnt = 500;
	}
	COM_ALL_HIGH;

	FND_Seg(fnd_data[com_loop]);
		
	if(tblink && fnd_blink & _BIT(com_loop))
	{
		COM_ALL_HIGH;
	}
	else
	{
		COM_ALL_HIGH;
		switch(com_loop)
		{
			case 0:
			COM3_LOW;
			break;
			case 1:
			COM2_LOW;
			break;
			case 2:
			COM1_LOW;
			break;
			case 3:
			COM0_LOW;
			break;
			default:
			COM_ALL_LOW;
			break;
		}
	}
	
	com_loop++;
	com_loop = com_loop % brightness_max;
	
}


int itoa(int num, unsigned char* str, int len, int base)
{
	int sum = num;
	int i = 0;
	int digit;
	int pos = 0;
	for(i=len;i;i--)
	{
		digit = sum % base;
		if(digit >= 0 && digit <= 9)
		{
			str[i-1] = digit + 48;
			pos++;
		}
		else
		{
			str[i-1] = 0;
		}
		sum /= base;
	}
	
	return pos;
}

void zero_to_blank(unsigned char* str, int len)
{
	uint8_t i=0;
	
	for(i=0;i<len;i++)
	{
		if(str[i] == '0')
		{
			str[i] = ' ';
		}
		else
		{
			break;
		}		
	}
}
