#include "esp_err.h"
#include "esp_log.h"
#include "driver/dac.h"

#define tag "d_to_a_utils"

void init_DAC(int dac_channel){

  ESP_LOGI(tag, "Initialize Digital to Analog Converter");

  esp_err_t r;
  gpio_num_t dac_gpio_num;

  r = dac_pad_get_io_num(dac_channel, &dac_gpio_num );
  if ( r == ESP_OK ){
    ESP_LOGI(tag, "DAC channel %d @ GPIO %d.\n",dac_channel + 1, dac_gpio_num );
  } else {
    ESP_LOGE(tag, "DAC failed to get I/O number code: 0x%.2X",r);
  }

  dac_output_enable(dac_channel );
}
