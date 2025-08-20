/***************************************************************************//**
* @file     modbus.h
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

#ifndef __MODBUD_H
#define __MODBUD_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

void memory_map_update(void);
void modbus_loop(void);
uint16_t crc16_modbus_wuth_size(const uint8_t data, uint16_t len);
uint8_t analysis_packet(uint8_t data);
uint16_t crc16_modbus(const uint8_t data) ;
uint16_t get_modbud_address(void);
uint16_t get_modbud_data_size(void);
uint8_t get_modbud_function_code(void);

void modbus_time_refresh(void);	 
void modbus_time_out(void);
void modbus_callback(void);
#ifdef __cplusplus
}
#endif

#endif   /*__MODBUD_H */

