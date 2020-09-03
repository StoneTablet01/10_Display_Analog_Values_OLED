/**
 * @file main.c
 * @brief Example program to measure a voltage on an ESP32 Analog to Digital
 * channel and write it to a SSD106 OLED display over an I2c serial connection.
 *
 * @author Jim Sutton
 * @date August 24 2020
 *
 * Copyright 2020 Jim Sutton
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"

#include "esp_system.h"
#include "sdkconfig.h" // generated by "make menuconfig"

#include "oled_1306.h" // driver and functions for OLED Display
#include "a_to_d_utils.h" // driver and functions for Analog to Digital Converter
#include "d_to_a_utils.h" // driver and functions for Analog to Digital Converter

#define SDA_PIN GPIO_NUM_21 // serial data line old value 18
#define SCL_PIN GPIO_NUM_22 // serial clock line old value 19
#define DAC_EXAMPLE_CHANNEL     CONFIG_EXAMPLE_DAC_CHANNEL
#define ADC1_EXAMPLE_CHANNEL    CONFIG_EXAMPLE_ADC1_CHANNEL

#define tag "SSD1306"

float adc_measured_voltage1(int adc_measured_counts){
    float adc_input_vdc;
    adc_input_vdc = adc_measured_counts * 0.000952; // 3.9 divided by 4096
    return adc_input_vdc;
}

int span_pct_integer(int adc_measured_counts){
  int span_percent;
  span_percent = (int) (adc_measured_voltage1(adc_measured_counts)*30.3);
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

void span_pct_string(int adc_measured_counts, char* span){
    sprintf(span,"%3d", span_pct_integer(adc_measured_counts));
    return;
}

void app_main(void) {
	uint8_t dac_set_point=0;
	int adc_measured_counts=0;

	char span[4] = "";
	char oled_message [32] ="";

	init_ADC(ADC1_EXAMPLE_CHANNEL);
	init_DAC(DAC_EXAMPLE_CHANNEL);

	vTaskDelay(2 * portTICK_PERIOD_MS); //2 port ticks == 1/50 sec

	//ESP_LOGI(tag, "start conversion.\n");
	//printf("  DAC Count    DAC VDC  ADC Count    ADC VDC   Span PCT"
	//"   Digit[0]   Digit[1]   Digit[2]\n");

	i2c_master_init(SDA_PIN, SCL_PIN);
	ssd1306_init();
	oled_display_clear();


	char str[] = "\n\n\n\n\n\n\n\nX";
	oled_display_text(str);
	//oled_display_scroll();

	while(1) {
		// test external components
		 	//myPrintf1( 2 ); //this function shows activity on the monitor..remove programming complete

			dac_output_voltage( DAC_EXAMPLE_CHANNEL, dac_set_point );
			adc_measured_counts = adc1_get_raw( ADC1_EXAMPLE_CHANNEL);

			span_pct_string(adc_measured_counts,span);
			/*
			printf("%11d %10.2f %10d %10.2f %10d %10c %10c %10c\n",
			dac_set_point, // 0 to 256 is the input range of the DAC
			dac_set_voltage(dac_set_point), //Calculated output voltage from the DAC
			adc_measured_counts, // input counts measured by ADC range is 0 to 4096
			adc_measured_voltage1(adc_measured_counts), //Calculated ADC voltage
			span_pct_integer(adc_measured_counts), //Span (0-100) recorded by ADC integer
			span[0], span[1], span[2]); //Span (0-100) recorded by ADC as string
			*/

			dac_set_point++;

			strcpy(oled_message, "  Stone Tablet  \n\nMoisture ");
			strcat(oled_message,span);
			strcat(oled_message," %");

			oled_display_text(oled_message);

			vTaskDelay( 2 * portTICK_PERIOD_MS );
	}
}
