#ifndef __APP_SENSING_H
#define __APP_SENSING_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

uint16_t get_snr_total_impedance(uint8_t snr_num);
int16_t get_diff_dis_impedance(uint8_t snr_num);
int16_t get_sensor_zone(uint8_t snr_num);
uint8_t get_snr_st(uint8_t *re);


uint16_t get_ref_error(uint16_t impedance,uint8_t snr_zone);


#ifdef __cplusplus
}
#endif

#endif   /*__APP_SENSING_H */

