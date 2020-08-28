# OLED-128x64-I2C-driver-demo
Example C program to display text on .96 inch OLED display from ESP32

----------
About
----------

This sample code implement procedures to write values from an ESP32 to an 128x64 OLED display with SSD1306 driver via ESP-IDF's I2C master driver. It supports all features described in `Solomon Systech's SSD1306 datasheet`. The program was initially written by Yanbe and made available through Apache 2.0 license on GITHUB. The code was altered to demonstrate how to use the OLED display by generating code with the Espressif development framework by Sutton. This code is also made available under Apache 2.0 license.

----------
For local setup
----------

For your local setup, connect SDA pin to GPIO 18 pin and the SCL to GPIO 19 pin

Be aware about there are several models on 128x64 OLED display with SSD1306. Like one model with more pins works with both SPI/I2C, and another model with lesser pins works I2C only.  This sample code is confirmed with a OLED model which have (GND, VDD, SCK, SDA) pins and it supports I2C only (no SPI available). Perhaps your model has slightly different pins, but it should work.

In case it does not work, please check your circuit. Consider inserting 10k ohm pull-up resistors on between 3.3v power supply and (SDA, SCL) pins respectively, as OLED display consumes larger current comparing with other tiny I2C sensors.


.. _ESP32 datasheet: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
.. _Solomon Systech's SSD1306 datasheet: https://www.robot-r-us.com/e/986-ssd1306-datasheet-for-096-oled.html

There is a YouTube Video of how to use the code as well

Good Luck!
Jim Sutton
August, 2020
