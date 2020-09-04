/**
 * @file oled_1306_utilities.c
 * @brief Component that handles all setup and operation of
 * the .96 inch OLED display using I2c serial communication
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

#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "oled_ssd1306.h"
#include "font8x8_basic.h"

#define tag "SSD1306     "

void i2c_master_init(int sda_pin, int scl_pin){
// This configures an i2c driver. Reference i2c driver API from Espressif
// ESP32 supports two i2c connections, this will be number 0.
	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = sda_pin,
		.scl_io_num = scl_pin,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 1000000
	};
	i2c_param_config(I2C_NUM_0, &i2c_config);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

void ssd1306_init() {
	// initialiaze the OLED 1306 display chip
	esp_err_t espRc; // for error reporting

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	// send a single byte with the i2c bus address and an embedded write request
	// set by the LSB = 0 to initiate a transaction with the slave
	// true indicates you want acknowledgement
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
	// after requesting connection send 1 byte to specify if data or commands
	// will be transmitted
	i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_CMD, true);

	// Configure Memory addressing mode as Page mode
	// See Table 9 Solomon Systech Technical Data sheet
	i2c_master_write_byte(cmd, OLED_CMD_SET_MEMORY_ADDR_MODE, true); //request to choose addr mode
	i2c_master_write_byte(cmd, OLED_CMD_SET_PAGE_ADDR_MODE, true); // set page addr mode
	i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_COL_L, true); // set lower nibble of col start addr
	i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_COL_H, true); // set lower nibble of col start addr
	i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_PAGE, true); // set start page in GDDRAM

	i2c_master_write_byte(cmd, OLED_CMD_SET_CHARGE_PUMP, true);
	i2c_master_write_byte(cmd, 0x14, true);

	i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP, true); // reverse left-right mapping
	i2c_master_write_byte(cmd, OLED_CMD_SET_COM_SCAN_MODE, true); // reverse up-bottom mapping

	i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_ON, true);
	i2c_master_stop(cmd);

	// now send the commands to the SSD1306. Report result in espRc
	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGI(tag, "OLED configured successfully\n");
	} else {
		ESP_LOGE(tag, "OLED configuration failed. code: 0x%.2X", espRc);
	}
	i2c_cmd_link_delete(cmd);
}

void oled_display_clear() {

	i2c_cmd_handle_t cmd;

	// create array of blanks to load onto device to clear the memory
	uint8_t zero[128];
	memset(&zero,0,128);

	// loop through the 8 pages (each page has 8 rows)
	for (uint8_t i = 0; i < 8; i++) {
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
		i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_CMD, true); // use command mode
		i2c_master_write_byte(cmd, 0xB0 | i, true); // to set the pointer to the row to clear. 0xB0 clears row 0
		i2c_master_stop(cmd);

		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);

		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
		i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_DATA, true); // use data XFER mode to load GDDRAM
		i2c_master_write(cmd, zero, 128, true); // load row (page) with 128 blank columns
		i2c_master_stop(cmd);

		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);
	}
}

void oled_display_scroll() {
	/* This function sets up the OLED display so that
		 Row 0 is static
		 Row 1 Scrolls Horrizontally
		 Row 2 Scrolls Horrizontally
		 Row 3 Scrolls Horrizontally
		 Rows 4-7 Scroll Horrizontally and Vertically
	*/
	esp_err_t espRc;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);

	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_CMD, true);

	// Setup vertical and Horrizontal scroll function for rows 1 to 7
	// 0x29 is a right scroll
	i2c_master_write_byte(cmd, 0x29, true); // vertical and horizontal scroll (p29)
	i2c_master_write_byte(cmd, 0x00, true); // dummy byte
	i2c_master_write_byte(cmd, 0x01, true); // define PAGE1 as start page
	i2c_master_write_byte(cmd, 0x06, true); // set time interval between each
	                                        // scroll step as 2 frames
	i2c_master_write_byte(cmd, 0x07, true); // define PAGE7 as end page
	i2c_master_write_byte(cmd, 0x01, true); // set vertical scrolling offset as
		                                      // scroll step as 1 row

	// Setup vertical scroll area with command 0xA3 for rows 4 to 7
	// rows that are in the vertical and horrizontal scroll area AND in The
	// vertical scroll area will scroll both Horrizontally and Vertically. Else Rows
	// will only go horrizontally. In this case the vertical area is the lower
	// 4 rows.
	i2c_master_write_byte(cmd, 0xA3, true); // set vertical scroll area (p30)
	i2c_master_write_byte(cmd, 0x20, true); // set the number of rows in top non-vertical area
	i2c_master_write_byte(cmd, 0x20, true); // set the nuber of rows in vertical scrolling area

	i2c_master_write_byte(cmd, 0x2F, true); // activate scroll (p29)

	i2c_master_stop(cmd);
	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGI(tag, "Scroll command succeeded");
	} else {
		ESP_LOGE(tag, "Scroll command failed. code: 0x%.2X", espRc);
	}

	i2c_cmd_link_delete(cmd);
}

void oled_display_text(const char *text) {

	i2c_cmd_handle_t cmd;
	uint8_t cur_page = 0, i=0, cur_line_char_written=0;

	cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_CMD, true);

	i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_COL_L, true); // set column low nibble to 0
	i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_COL_H, true); // set column high nibble to 0
	i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // set row to page0

	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	for (i = 0; i < strlen(text); i++) {
		if (text[i] == '\n') {
			cur_line_char_written=0;
			cmd = i2c_cmd_link_create();
			i2c_master_start(cmd);
			i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
			i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_CMD, true);

			i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_COL_L, true); // set column low nibble to 0
			i2c_master_write_byte(cmd, OLED_CMD_SET_PAGEM_START_COL_H, true); // set column high nibble to 0
			i2c_master_write_byte(cmd, 0xB0 | ++cur_page, true); // increment page

			i2c_master_stop(cmd);
			i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
			i2c_cmd_link_delete(cmd);
		} else {
			if(cur_line_char_written < 16) { // Max chars per line is 16 (for 128x64 pixels, 8px*16chars=128px)
				cmd = i2c_cmd_link_create();
				i2c_master_start(cmd);
				i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
				i2c_master_write_byte(cmd, OLED_CONTROL_SET_WRITE_DATA, true);

				i2c_master_write(cmd, font8x8_basic_tr[(uint8_t)text[i]], 8, true);

				i2c_master_stop(cmd);
				i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
				i2c_cmd_link_delete(cmd);
				cur_line_char_written++;
			}
		}
	}
}
