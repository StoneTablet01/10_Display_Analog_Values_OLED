# Display_Analog_Values_OLED
Example C program to display measured voltage on a .96 inch OLED display using ESP32 SOC

----------
About
----------

This project implements a basic digital voltmeter using an ESP32 and a OLED disply. The code implements c functions to create an output voltage by using one of the ESP32's digital to analog converter, reads the voltage just created using one of the ESP32's onboard digital to analog converters, and finaly displays the measured voltage on a 128x64 OLED display with SSD1306 driver via ESP-IDF's I2C master driver. The program uses example code from Espressif and some code for the OLED from Yanbe. As mentioned, the OLED function code was initially written by Yanbe and made available through Apache 2.0 license on GITHUB. This code is available at https://github.com/yanbe/ssd1306-esp-idf-i2c . The code in this project was substantially altered and converted to a component by Sutton. This code, as with the Espressif code and Yanbe code, is also made available under Apache 2.0 license.

----------
For local setup
----------

For your local setup, connect the OLED display to the ESP32 by wiring the OLED SDA pin to GPIO 21 pin and the OLED SCL pin to GPIO 22 pin. In order to provide the voltag input you must connect the ES32 DAC output (IO 25) pin to the ESP32 ADC 7 (IO 35) pin.

If you want to change the DAC or ADC, this can be done with the idf.py menuconfig program. If you want to change the I2c serial pins on the ESP32 you can do so in the main program where they are defined.

Once the program is cloned, built and flashed to the ESP32 the OLED display will show a voltage that slowly ramps from 0 to 3.3 VDC and starts over again.

In case it does not work, please check the wiring connections. Yanbe ponts out that you should consider inserting a 10k ohm pull-up resistors on between 3.3v power supply and (SDA, SCL) pins respectively, as OLED display consumes larger current comparing with other tiny I2C sensors. My application worked well without one.

.. _ESP32 datasheet: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
.. _Solomon Systech's SSD1306 datasheet: https://www.robot-r-us.com/e/986-ssd1306-datasheet-for-096-oled.html

There is a YouTube Video of how to use the code as well

Good Luck!
Jim Sutton
August, 2020
