#ifndef __SENSOR_H
#define __SENSOR_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "model.h"

//#define ADC_DATA_MAX 20

//#define VOLT_ADC_OFFSET 2.11
//#define VOLT_ADC_OFFSET 7
#define VOLT_ADC_OFFSET 0

#define DETECTION_TIME      5 //*100ms 500ms

#define SNR_NUM 2



void get_sensors_data(void);
uint16_t get_avg(uint16_t *data, uint8_t len);
float get_avg_f(uint16_t *data, uint8_t len);
uint16_t get_avg_minmax(uint16_t *data, uint8_t len);

uint16_t get_charging_low_impedance(uint8_t snr_num);
uint16_t get_charging_high_impedance(uint8_t snr_num);
float get_discharging_low_impedance(uint8_t snr_num);
float get_discharging_high_impedance(uint8_t snr_num);
uint16_t get_dis_impedance(uint8_t snr_num,uint8_t snr_zone);

void set_sensor_on(uint16_t ch,uint16_t onoff);
uint16_t get_sensor_on(uint16_t ch);

void sensing_loop(void);
void sensor_sw(uint16_t impedance);
uint32_t sensing(void);

uint8_t get_sel_sensor(void);
uint8_t is_sensor_ready(void);

uint32_t get_snr_pwoff_t(uint8_t ch);
uint32_t get_snr_keep_t(uint8_t ch);
#ifdef __cplusplus
}
#endif

#endif   /*__LED_H */
