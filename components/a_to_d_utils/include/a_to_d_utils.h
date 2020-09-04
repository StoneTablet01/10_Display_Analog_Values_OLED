#ifndef A_TO_D_UTILS_H
#define  A_TO_D_UTILS_H

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////



void init_ADC(int adc1_channel);

int get_adc_measured_count(int adc1_channel);

float get_adc_measured_voltage(int adc1_channel);

int get_span_pct_integer(int adc1_channel);

void get_span_pct_string(int adc1_channel, char* span);

void get_voltage_string(int adc1_channel, char* span);




///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

#endif //  A_TO_D_UTILS_H
