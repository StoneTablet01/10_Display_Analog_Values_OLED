#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

/**
 * @file oled_ssd1306.h
 * @brief Header file provides variables and default values for enabling a
 * I2c serial connection from an ESP32 to SSD106 OLED display.
 *
 * @details This demonstration program is derivative work from code on GITHUB
 * Yanbe/ssd1306-esp-idf-i2c. The Yanbe work license is Apache License 2.0.
 * Notice is herby given the files have been changed, and in some cases renamed.
 * The Yanbe license provides this notice:
 * "Some of the Following definitions are bollowed from
 * http://robotcantalk.blogspot.com/2015/03/interfacing-arduino-with-ssd1306-driven.html"
 *
 * @see License in the root directory
 * @see Readme in the root directory

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
//

// I2c 7 bit address + R/W bit
// SLA (0x3C) + WRITE_MODE (0x00) =  0x78 (0b01111000)
#define OLED_I2C_ADDRESS   0x3C              //I2c address of most 1306 OLED Displays

// A Control byte sent after I2c address
#define OLED_CONTROL_SET_WRITE_CMD      0X00 // Next bytes sent are one or more commands
#define OLED_CONTROL_SET_WRITE_DATA     0X40 // Next bytes sent are data for storage in GDDRAM
#define OLED_CONTROL_CONTINUATION_MODE  0X80 // Prepare to allow combined commands and data
                                             // such as mixed R/W transactions with
                                             // multiple start commands

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST           0x81  // 256 Contrast steps (RESET = 0x7F)
                                              // follow with byte setting contrast
#define OLED_CMD_DISPLAY_RAM            0xA4
#define OLED_CMD_DISPLAY_ALLON          0xA5
#define OLED_CMD_DISPLAY_NORMAL         0xA6
#define OLED_CMD_DISPLAY_INVERTED       0xA7
#define OLED_CMD_DISPLAY_OFF            0xAE
#define OLED_CMD_DISPLAY_ON             0xAF

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE   0x20  // Sets ADDR Mode. Follow with 0x00,
#define OLED_CMD_SET_HORZ_ADDR_MODE     0x00  // follows MEMORY_ADDR_MODE to choose Horz mode
#define OLED_CMD_SET_VERT_ADDR_MODE     0x01  // follows MEMORY_ADDR_MODE to choose Vert mode
#define OLED_CMD_SET_PAGE_ADDR_MODE     0x10  // follows MEMORY_ADDR_MODE to choose Page mode

// Page Mode Commands to set Row and Column pointer (only valid if in page mode)
#define OLED_CMD_SET_PAGEM_START_COL_L  0x00  // Set lower nibble of column (00 to 0F)
#define OLED_CMD_SET_PAGEM_START_COL_H  0x10  // Set higher nibble of column (10 to 1F)
#define OLED_CMD_SET_PAGEM_START_PAGE   0xB0  // Set start page (B0-B7)

// Horz and Vertical Mode Commands to set Rows and Columns pointer
#define OLED_CMD_SET_COLUMN_RANGE       0x21  // can be used only in HORZ and VERT mode
                                              // follow with 0x00 and 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE         0x22  // can be used only in HORZ/VERT mode
                                              // follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE 0x40  // Set RAM start line (40F to 7F)
                                              // 40F = 0 offset (On reset set to 40F)
#define OLED_CMD_SET_SEGMENT_REMAP      0xA1
#define OLED_CMD_SET_MUX_RATIO          0xA8  // follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE      0xC8
#define OLED_CMD_SET_DISPLAY_OFFSET     0xD3  // follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP        0xDA  // follow with 0x12
#define OLED_CMD_NOP                    0xE3  // NOP

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV    0xD5  // follow with 0x80
#define OLED_CMD_SET_PRECHARGE          0xD9  // follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT      0xDB  // follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP        0x8D  // follow with 0x14

#endif /* OLED_SSD1306_H */
