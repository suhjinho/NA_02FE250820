#include "modbus.h"
#include "hal.h"
#include "queue.h"
#include "app_data.h"
#include "sensor.h"
#include "app_sensing.h"
#include "app_normal.h"

#define REG_MAX 10
#define MODBUS_TIME_OUT 5
#define MODBUS_MAP REG_MAX*2
#define RCV_DATA_MAX 20
static uint8_t function_code = 0;
static uint16_t address = 0;
static uint16_t n_of_data = 0;
static uint8_t rcv_data[RCV_DATA_MAX] = {0};
static uint8_t rcv_data_byte = 0;
static uint16_t packet_crc = 0;
static uint8_t rx_data[3+(REG_MAX*8)+5];
static uint16_t modbus_lock_cnt = 0;

static uint16_t modbusmap[MODBUS_MAP] = {0};

uint8_t modbus_save_data(uint16_t address,uint8_t *rcv_data,uint8_t rcv_data_byte);


 static const uint16_t crc_table[] = 
 {
	 0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280,
	 0XC241, 0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1,
	 0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00,
	 0XCFC1, 0XCE81, 0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40,
	 0XC901, 0X09C0, 0X0880, 0XC841, 0XD801, 0X18C0, 0X1980,
	 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, 0X1E00, 0XDEC1,
	 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400,
	 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	 0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081,
	 0X1040, 0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1,
	 0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501,
	 0X35C0, 0X3480, 0XF441, 0X3C00, 0XFCC1, 0XFD81, 0X3D40,
	 0XFF01, 0X3FC0, 0X3E80, 0XFE41, 0XFA01, 0X3AC0, 0X3B80,
	 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840, 0X2800, 0XE8C1,
	 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41, 0XEE01, 
	 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40, 
	 0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 
	 0X2640, 0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 
	 0X2080, 0XE041, 0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 
	 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740, 
	 0XA501, 0X65C0, 0X6480, 0XA441, 0X6C00, 0XACC1, 0XAD81, 
	 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41, 0XAA01, 0X6AC0, 
	 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840, 0X7800, 
	 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41, 
	 0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 
	 0X7C40, 0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 
	 0XB681, 0X7640, 0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 
	 0X71C0, 0X7080, 0XB041, 0X5000, 0X90C1, 0X9181, 0X5140, 
	 0X9301, 0X53C0, 0X5280, 0X9241, 0X9601, 0X56C0, 0X5780, 
	 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01, 0X5CC0, 
	 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 
	 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841, 
	 0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 
	 0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 
	 0X4C80, 0X8C41, 0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 
	 0X47C0, 0X4680, 0X8641, 0X8201, 0X42C0, 0X4380, 0X8341, 
	 0X4100, 0X81C1, 0X8081, 0X4040 };
	 
static uint16_t crc_word = 0xFFFF; 



static uint8_t packet_status = 0;
static uint8_t data_cnt = 0;
static uint8_t packet_time_out = 0;

static uint8_t modbud_err_code;

uint16_t send_packet(uint16_t address, uint16_t len);
uint16_t send_errorcode (uint8_t error);
uint16_t send_respone_single_reg(uint16_t address);
uint16_t send_reg(uint16_t address, uint16_t len);
uint16_t send_single_data(uint16_t address, uint16_t data);

void modbus_time_refresh(void)
{
	packet_time_out = 0;
}	
	 
void modbus_time_out(void)
{
	packet_time_out ++;
	
	if(modbus_lock_cnt > 0) // 100ms 
	{
		modbus_lock_cnt--;
	}
	
	if(packet_time_out > MODBUS_TIME_OUT)
	{
		packet_status = 0;
		data_cnt = 0;
		packet_time_out=0;
	}
	
}	
uint16_t crc16_modbus(const uint8_t data) 
{
	uint8_t nTemp; 
	
	nTemp = data ^ crc_word;
	crc_word >>= 8; 

	crc_word ^= crc_table[nTemp]; 

	return crc_word; 
}

uint16_t crc16_modbus_with_size(const uint8_t * data, uint16_t len) 
{
//	uint8_t nTemp;
	crc_word = 0xFFFF;
	uint8_t i=0;
	
	for(i=0;i<len;i++)
	{
		crc16_modbus(data[i]);
	}

	return crc_word; 

}

uint8_t get_last_err_code()
{
	uint8_t t;
	t = modbud_err_code;
	modbud_err_code = 0;
	
	return t;
}
 
uint8_t analysis_packet(uint8_t data)
{
	uint8_t re = 0;

	switch(packet_status)
	{
		case 0://slave_id
		{
			if(data == (uint8_t)get_app_data(e_sys_ID_v))
			{
				packet_status = 1;
				address = 0;
				crc_word = 0xFFFF;
				crc16_modbus(data);
				packet_crc = 0;
			}
		}
		break;
		 
		case 1://Function Code
		{
			function_code = data;
			if(function_code == 0x04)
			{
				packet_status = 2;
				crc16_modbus(data);
			}
			else if(function_code == 0x06)
			{
				packet_status = 2;
				crc16_modbus(data);
			}
			else if(function_code == 0x03)
			{
				packet_status = 2;
				crc16_modbus(data);
			}
			else 
			{
				modbud_err_code = 1;
			}
		}
		break;
		
		case 2://Address
		{
			//address |= (uint16_t)data<<(data_cnt*8);
			address = address << (data_cnt*8) | (uint16_t)data;
			if(get_app_data(e_sys_modebus_v)  < 2) //old ver 
			{
				address = 0;
			}
			data_cnt++;
			if(data_cnt == 2)
			{
				data_cnt = 0;
				if(function_code == 0x04)
				{
					packet_status = 4;
					n_of_data = 0;
				}
				else if(function_code == 0x06)
				{
					packet_status = 6;
					rcv_data_byte = 2;
					n_of_data = 0;
				}
				else
				{
					modbud_err_code = 1;
				}
			}
			crc16_modbus(data);
		}
		break;
		case 4://n_of_data
		{
			//n_of_data |= (uint16_t)data<<(data_cnt*8);
			n_of_data = n_of_data << (data_cnt*8) | (uint16_t)data;
			data_cnt++;
			if(data_cnt == 2)
			{
				if(function_code == 0x04)
				{
					data_cnt = 0;
					packet_status = 5;
				}
				else
				{
					modbud_err_code = 1;
				}
					

			}
			crc16_modbus(data);
		}
		break;

		case 5://CRC
		{
			packet_crc |= (uint16_t)data<<(data_cnt*8);
			data_cnt++;
			if(data_cnt == 2)
			{
				//crc 체크
				packet_status = 0;
				data_cnt = 0;
				if( packet_crc == crc_word)
				{
					re = 1;
				}
				else
				{
					//error
					function_code = 8;
					address = 0;
					n_of_data = 0;
					packet_crc = 0;
				}
			}
		}
		break;
		case 6://rcv_data
		{
			//01 06 00 00 00 FF C9 8A
			 //n_of_data |= (uint16_t)data<<(data_cnt*8);
			rcv_data[data_cnt] = data;
			data_cnt++;
			if(data_cnt >= rcv_data_byte)
			{
				data_cnt = 0;
				packet_status = 5;
				rcv_data_byte = 2;
			}
			 crc16_modbus(data);
		}
		break;
		
		case 7://rcv_byte_data_size
		{
			rcv_data_byte = data;
			data_cnt = 0;
			if(rcv_data_byte <= 20)
			{
				packet_status = 8;				
			}
			else
			{
				modbud_err_code = 2;
			}
			
			 crc16_modbus(data);
		}
		break;
		case 8://rcv_byte_data_size
		{
			//01 10 00 00 00 05 0A 00 00 00 00 00 00 00 00 00 00 01 F8
			rcv_data[data_cnt] = data;
			data_cnt++;
			if(data_cnt >= rcv_data_byte)
			{
				data_cnt = 0;
				packet_status = 5;
			}
			
			crc16_modbus(data);
		}
		break;
			
	}
	if(modbud_err_code)
	{
		packet_status = 0;
		function_code = 0;
		address = 0;
		n_of_data = 0;
		packet_crc = 0;	
        modbud_err_code = 0;
	}

	 return re;
	 
 }
uint8_t modbus_save_data(uint16_t address,uint8_t *rcv_data,uint8_t rcv_data_byte)
{
	uint8_t re = 0;
	uint16_t t_addess;
	uint8_t i;
	uint16_t t_data = 0;
	t_addess = address-1000;
	if(t_addess < APP_DATA_MAX)
	{
		for(i=0;i<rcv_data_byte;)
		{

			t_data = rcv_data[i++];
			t_data = (t_data<<8) | rcv_data[i++];
		}
		//save_data((eeprom_index)t_addess, (uint32_t)(rcv_data_byte/2));//시작 인덱스 부터 size 만큼 eeprom에 저장 
		set_app_data(t_addess,t_data);
	}
	else
	{
		re = 1;
	}
		
	return re;
}


 void modbus_loop(void)
{
	uint8_t data;
	static uint8_t status = 0;
	static uint16_t len = 0;
	
	switch(status)
	{
		case 0:
				
			if(deleteq(&data))
			{
				modbus_time_refresh();
				if(analysis_packet(data))
				{
					if(function_code == 0x04)
					{
						if(n_of_data <= 20)
						{
							//if(address >= 0 && address < 1000 )
							if(address < 1000 )
							{
								len = send_packet(address,n_of_data);		
								status = 1;
							}
							else if(address >= 1000 && address < 1100 )
							{
								if(modbus_lock_cnt)
								{
									len = send_reg(address ,n_of_data);	
								}
								else
								{
									len = send_errorcode(7);
								}
								status = 1;
							}
							else
							{
								len = send_errorcode(2);
								status = 1;							
							}
						}
						else
						{
							len = send_errorcode(2);
							status = 1;							
						}
					}
					if(function_code == 0x06 )
					{
						
						if(address >= 1000 && address < 1100 )
						{
							if(modbus_lock_cnt)
							{
								uint8_t t = modbus_save_data(address,rcv_data,rcv_data_byte);							
								len = send_respone_single_reg(address);
								status = 1;	
							}
							else
							{
								len = send_errorcode(7);
								status = 1;	
							}
						}else if (address == 5000)
						{
							uint16_t t_data;
							t_data = (rcv_data[0]<<8) | rcv_data[1];
							if(t_data == 510)
							{
								modbus_lock_cnt = 600;//100ms*600 = 60 000ms 60s
								len = send_single_data(address, t_data);
								status = 1;
							}
							else
							{
								len = send_errorcode(7);
								status = 1;	
							}	
						}
						else
						{
							len = send_errorcode(2);
							status = 1;						
						}
					}
				}
				else
				{
					uint8_t t = get_last_err_code();
					if(t)
					{
						len = send_errorcode(t);
						status = 1;
					}

				}
			}
			break;
			
		case 1: 
			DE_485;
			set_msdelay1(5);		
			status = 2;
		break;
		
		case 2:
		if(escape_msdelay1())
		{
			uart_tx(rx_data,len);
			set_msdelay1(5);
			status = 3;
		}
		break;
		
		case 3:
		if(escape_msdelay1())
		{
			RE_485;
			len = 0;
			status = 0;
		}
		break;
	}
	
}

uint16_t get_modbud_address(void)
{
	return address;
	
}
uint16_t get_modbud_data_size(void)
{
	return n_of_data;
}

uint8_t get_modbud_function_code(void)
{
	return function_code;
}

void memory_map_update(void)
{
	uint8_t i;
	uint8_t map_index = 0;
	
	if(get_app_data(e_sys_modebus_v)  == 0) //old ver 
	{
		for(i=0;i<SNR_NUM;i++)
		{
			modbusmap[map_index++] = get_snr_state_modbud(i);
		}		
	}
	else
	{
		for(i=0;i<SNR_NUM;i++)
		{
			modbusmap[map_index++] = get_snr_state_modbud(i);
			modbusmap[map_index++] = get_snr_total_impedance(i);
			modbusmap[map_index++] = get_diff_dis_impedance(i);   /* ER값 */
		}
	}
}

uint16_t send_errorcode (uint8_t error)
{
	uint16_t crc;
	uint16_t i = 0;
	rx_data[i++] = (uint8_t)get_app_data(e_sys_ID_v);
	rx_data[i++] = function_code | 0x80;
	rx_data[i++] = error;
	crc = crc16_modbus_with_size(rx_data,i);
	rx_data[i++] = (uint8_t)(crc);
	rx_data[i++] = (uint8_t)(crc>>8);
	
	return i;
}

uint16_t send_single_data(uint16_t address, uint16_t data)
{
	uint16_t crc;
	uint16_t i = 0;

	rx_data[i++] = (uint8_t)get_app_data(e_sys_ID_v);
	rx_data[i++] = function_code;
	rx_data[i++] = address>>8;
	rx_data[i++] = (uint8_t)(address&0x00FF);
	rx_data[i++] = (uint8_t)(data>>8);
	rx_data[i++] = (uint8_t)(data&0x00FF);
	crc = crc16_modbus_with_size(rx_data,i);
	rx_data[i++] = (uint8_t)(crc);
	rx_data[i++] = (uint8_t)(crc>>8);
	
	return i;
}

uint16_t send_reg(uint16_t address, uint16_t len)
{
	uint16_t crc;
	uint16_t i = 0;
	uint16_t j = 0;
//	uint16_t data_index = 0;
//	data_index = address;
	rx_data[i++] = (uint8_t)get_app_data(e_sys_ID_v);
	rx_data[i++] = function_code;
	rx_data[i++] = len*2;
	for(j = 0 ; j < len;j++)
	{
		uint16_t temp = get_app_data((address-1000)+j);
		rx_data[i++] = (uint8_t)(temp>>8);
		rx_data[i++] = (uint8_t)(temp&0x00FF);
	}
	crc = crc16_modbus_with_size(rx_data,i);
	rx_data[i++] = (uint8_t)(crc);
	rx_data[i++] = (uint8_t)(crc>>8);
	
	return i;
}


uint16_t send_packet(uint16_t address, uint16_t len)
{
	uint16_t crc;
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t data_index = 0;
	data_index = address;
	rx_data[i++] = (uint8_t)get_app_data(e_sys_ID_v);
	rx_data[i++] = function_code;
	rx_data[i++] = len*2;
	for(j = 0 ; j < len;j++)
	{
		uint16_t temp;
		if(data_index+j >= MODBUS_MAP)
		{
			temp = 0;
		}
		else
		{
			temp = modbusmap[data_index+j];
		}
		rx_data[i++] = (uint8_t)(temp>>8);
		rx_data[i++] = (uint8_t)(temp&0x00FF);
	}
	crc = crc16_modbus_with_size(rx_data,i);
	rx_data[i++] = (uint8_t)(crc);
	rx_data[i++] = (uint8_t)(crc>>8);
	
	return i;
}

uint16_t send_respone_single_reg(uint16_t address)
{
	uint16_t crc;
	uint16_t i = 0;
	uint16_t temp = get_app_data(address-1000);
	rx_data[i++] = (uint8_t)get_app_data(e_sys_ID_v);
	rx_data[i++] = function_code;
	rx_data[i++] = address>>8;
	rx_data[i++] = (uint8_t)(address&0x00FF);
	rx_data[i++] = (uint8_t)(temp>>8);
	rx_data[i++] = (uint8_t)(temp&0x00FF);
	crc = crc16_modbus_with_size(rx_data,i);
	rx_data[i++] = (uint8_t)(crc);
	rx_data[i++] = (uint8_t)(crc>>8);
	
	return i;
}
