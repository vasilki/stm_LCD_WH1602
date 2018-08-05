# stm_LCD_WH1602
Connecting LCD 1602A (QAPASS) to the Nucleo F401RE and migration libraries to HAL\Cubemx
VSS = GND
VDD = +5V
PINOUT:
PB9-CN10.5 - D7
PB8-CN10.3 - D6
PB7-CN7.21 - D5
PB6-CN10.17 - D4
PB5-CN10.29 - EN
PB4-CN10.27 - RS

Manuals was taken from the following webpages:
  STM32:https://microtechnics.ru/podklyuchenie-i-rabota-s-displeem-wh1602/
  STM32:http://cxem.net/mc/mc263.php
  Arduino:https://goo.gl/EqXi9U

Datasheet 1(for qapass):https://funduino.de/DL/1602LCD.pdf
Datasheet 2:https://www.waveshare.com/datasheet/LCD_en_PDF/LCD1602.pdf
Lib DWT_STM32_DELAY:https://controllerstech.com/create-1-microsecond-delay-stm32/


Environment:
STM32 NUCLEO-F401RE
Linux Mint 18.2 Sonya
STM32Cube_1.0 version 4.24.0.Repository is STM32Cube_FW_F4_V1.19.0.
System Workbench for STM32 (Version: 1.13.2.201703061529) based on Eclipse IDE version Mars.2.Release 4.5.2
