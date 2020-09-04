/**
 * @file a_to_d_utils.c
 * @brief Component that handles all analog to digital converter setup
 * and operation.
 *
 * @author Jim Sutton
 * @date August 24 2020
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc.h"
#include <stdio.h>
#include <string.h>

#define tag "a_to_d_utils"

void init_ADC(int adc1_channel){
  ESP_LOGI(tag, "Initialize Analag to Digital Converter 1");

  esp_err_t r;
  gpio_num_t adc_gpio_num;

  r = adc1_pad_get_io_num( adc1_channel, &adc_gpio_num );
  if ( r == ESP_OK ){
    ESP_LOGI(tag, "ADC1 channel %d @ GPIO %d \n", adc1_channel, adc_gpio_num);
  } else {
    ESP_LOGE(tag, "ADC failed to get I/O number code: 0x%.2X",r);
  }

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten( adc1_channel, ADC_ATTEN_DB_11 ); //0 to 3.9VDC
}

int get_adc_measured_count(int adc1_channel){
  int adc_measured_count;
  adc_measured_count = adc1_get_raw( adc1_channel);
  return adc_measured_count;
}

float get_adc_measured_voltage(int adc1_channel){
    float adc_input_vdc;
    int adc_measured_count;
    adc_measured_count = adc1_get_raw( adc1_channel);
    adc_input_vdc = adc_measured_count * 0.000952; // 3.9 divided by 4096
    return adc_input_vdc;
}

int get_span_pct_integer(int adc1_channel){
  int span_percent;
  span_percent = (int) (get_adc_measured_voltage(adc1_channel)*30.3);
	span_percent = (float) span_percent * -2.703 + 191.89 + .5;
  if (span_percent < 0){
    span_percent = 0;
  }
  else {if (span_percent > 100){
    span_percent = 100;
    }
  }
  return span_percent;
}

void get_span_pct_string(int adc1_channel, char* span){
  sprintf(span,"%3d", get_span_pct_integer( adc1_channel ));
  return;
}

void get_voltage_string(int adc1_channel, char* span){
  sprintf(span,"%3.1f", get_adc_measured_voltage( adc1_channel ));
}

void output_table_header(){
  printf("  DAC Count    DAC VDC  ADC Count    ADC VDC   Span PCT"
  "   Digit[0]   Digit[1]   Digit[2]\n");
}

void output_table( uint8_t dac_set_point, float dac_set_voltage,
                   int adc_measured_count, float adc_measured_voltage,
                   int span_pct_integer, char* span){

  printf("%11d %10.2f %10d %10.2f %10d %10c %10c %10c\n",
  dac_set_point, // 0 to 256 is the input range of the DAC
  dac_set_voltage, //Calculated output voltage from the DAC
  adc_measured_count, // input counts measured by ADC range is 0 to 4096
  adc_measured_voltage, //Calculated ADC voltage
  span_pct_integer, //Span (0-100) recorded by ADC integer
  span[0], span[1], span[2]); //Span (0-100) recorded by ADC as string
}
