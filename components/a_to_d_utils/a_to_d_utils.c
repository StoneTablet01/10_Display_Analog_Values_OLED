#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc.h"

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
