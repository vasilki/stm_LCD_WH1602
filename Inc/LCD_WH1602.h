/*
 * LCD_WH1602.h
 *
 *  Created on: 30 июля 2018 г.
 *      Author: vasilek
 */

#ifndef LCD_WH1602_H_
#define LCD_WH1602_H_

//#include "stm32f4xx_hal_gpio.h"
#include "stm32f401xe.h"

typedef struct
{
  GPIO_TypeDef *pLine;
  uint32_t RS;
  uint32_t EN;
  uint32_t D4;
  uint32_t D5;
  uint32_t D6;
  uint32_t D7;
}T_LCD_GPIO_Parameters;

void PrintStr(char *par_string);
void ClearLCDScreen();
void Cursor(char Row, char Col);
void InitializeLCD(T_LCD_GPIO_Parameters par_parameters /*GPIOx*/);


#endif /* LCD_WH1602_H_ */
