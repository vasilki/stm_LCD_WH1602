/*
 * LCD_WH1602.c
 *
 *  Created on: 30 июля 2018 г.
 *      Author: vasilek
 */
#include "LCD_WH1602.h"
//#include "stm32f4xx_hal_rcc.h"
#include "uart.h"
#include <string.h>
#include "timers.h"

extern UART_HandleTypeDef huart1; /*declared in main.c*/
//---Переопределяем порты для подключения дисплея, для удобства---//
/*
#define     LCM_OUT               GPIOB->ODR
#define     LCM_PIN_RS            GPIO_Pin_0          // PB0
#define     LCM_PIN_EN            GPIO_Pin_1          // PB1
#define     LCM_PIN_D7            GPIO_Pin_7          // PB7
#define     LCM_PIN_D6            GPIO_Pin_6          // PB6
#define     LCM_PIN_D5            GPIO_Pin_5          // PB5
#define     LCM_PIN_D4            GPIO_Pin_4          // PB4
#define     LCM_PIN_MASK  ((LCM_PIN_RS | LCM_PIN_EN | LCM_PIN_D7 | LCM_PIN_D6 | LCM_PIN_D5 | LCM_PIN_D4))
*/
static uint32_t *gl_line = 0;
static uint32_t     LCM_PIN_RS;           // GPIO_Pin_0          // PB0
static uint32_t     LCM_PIN_EN;           // GPIO_Pin_1          // PB1
static uint32_t     LCM_PIN_D7;           // GPIO_Pin_7          // PB7
static uint32_t     LCM_PIN_D6;           // GPIO_Pin_6          // PB6
static uint32_t     LCM_PIN_D5;           // GPIO_Pin_5          // PB5
static uint32_t     LCM_PIN_D4;           // GPIO_Pin_4          // PB4
#define      LCM_PIN_MASK  ((LCM_PIN_RS | LCM_PIN_EN | LCM_PIN_D7 | LCM_PIN_D6 | LCM_PIN_D5 | LCM_PIN_D4))
#define MT_WH1602_DELAY_uS            (10000000)

#define LCM_OUT (*gl_line)

//---Функция задержки---//
/*
void delay(int a)
{
    int i = 0;
    int f = 0;
  //  a=a*10;
    while(f < a)
    {
        while(i<60)
            {i++;}
        f++;
    }
}
*/
/*
void delay(uint32_t par_us)
{
  extern uint32_t HAL_RCC_GetSysClockFreq();

  uint32_t i = HAL_RCC_GetSysClockFreq();

  i = (i/MT_WH1602_DELAY_uS)*par_us;

  for (; i != 0; i--);

  return;
}

*/


void delay(uint32_t par_us)
{
  tim_Delay_us(par_us);
  return;
}

//---Нужная функция для работы с дисплеем, по сути "дергаем ножкой" EN---//
void PulseLCD()
{
    LCM_OUT &= ~LCM_PIN_EN;
    delay(2);
    LCM_OUT |= LCM_PIN_EN;
    delay(2);
    LCM_OUT &= (~LCM_PIN_EN);
    delay(2);
}


void SetTetradePins(char par_ByteToSend, unsigned int *par_MSTetradePins, unsigned int *par_LSTetradePins)
{
  *par_MSTetradePins = 0;
  *par_LSTetradePins = 0;

  if(((par_ByteToSend >> 7) & 0x1) == 1)
  {
    *par_MSTetradePins = LCM_PIN_D7;
  }
  if(((par_ByteToSend >> 6) & 0x1) == 1)
  {
    *par_MSTetradePins |= LCM_PIN_D6;
  }
  if(((par_ByteToSend >> 5) & 0x1) == 1)
  {
    *par_MSTetradePins |= LCM_PIN_D5;
  }
  if(((par_ByteToSend >> 4) & 0x1) == 1)
  {
    *par_MSTetradePins |= LCM_PIN_D4;
  }

  if(((par_ByteToSend >> 3) & 0x1) == 1)
  {
    *par_LSTetradePins = LCM_PIN_D7;
  }
  if(((par_ByteToSend >> 2) & 0x1) == 1)
  {
    *par_LSTetradePins = LCM_PIN_D6;
  }
  if(((par_ByteToSend >> 1) & 0x1) == 1)
  {
    *par_LSTetradePins |= LCM_PIN_D5;
  }
  if((par_ByteToSend & 0x1) == 1)
  {
    *par_LSTetradePins |= LCM_PIN_D4;
  }


  return;
}
//---Отсылка байта в дисплей---//
void SendByte(char ByteToSend, int IsData)
{
  unsigned int loc_MSTetradePins = 0;
  unsigned int loc_LSTetradePins = 0;

    LCM_OUT &= (~LCM_PIN_MASK);
    //LCM_OUT |= (ByteToSend & 0xF0);

    SetTetradePins(ByteToSend,&loc_MSTetradePins,&loc_LSTetradePins);
    LCM_OUT |= loc_MSTetradePins;
    if (IsData == 1)
        LCM_OUT |= LCM_PIN_RS;
    else
        LCM_OUT &= ~LCM_PIN_RS;
    PulseLCD();


    LCM_OUT &= (~LCM_PIN_MASK);
    //LCM_OUT |= ((ByteToSend & 0x0F) << 4);
    LCM_OUT |= loc_LSTetradePins;

    if (IsData == 1)
        LCM_OUT |= LCM_PIN_RS;
    else
        LCM_OUT &= ~LCM_PIN_RS;

    PulseLCD();
}

//---Установка позиции курсора---//
void Cursor(char Row, char Col)
{
   char address;
   if (Row == 0)
   address = 0;
   else
   address = 0x40;
   address |= Col;
   SendByte(0x80 | address, 0);
   delay(50);
}

void CursorOFF()
{
  SendByte(0xC, 0); //Курсор выключен
  return;
}

void SetCursorFreeze()
{
  SendByte(0xE, 0); //Курсор не мигает
  return;
}


void SetCursorBlink()
{
  SendByte(0xF, 0); //Курсор мигает
  return;
}


//---Очистка дисплея---//
void ClearLCDScreen()
{
    SendByte(0x01, 0);
    delay(32000);
    SendByte(0x02, 0);
    delay(32000);
}

//---Инициализация дисплея---//
void InitializeLCD(T_LCD_GPIO_Parameters par_parameters /*GPIOx*/)
{
    gl_line = &par_parameters.pLine->ODR;
    LCM_PIN_RS = par_parameters.RS;
    LCM_PIN_EN = par_parameters.EN;
    LCM_PIN_D4 = par_parameters.D4;
    LCM_PIN_D5 = par_parameters.D5;
    LCM_PIN_D6 = par_parameters.D6;
    LCM_PIN_D7 = par_parameters.D7;

    LCM_OUT &= ~(LCM_PIN_MASK);
    LCM_OUT &= ~LCM_PIN_RS;
    LCM_OUT &= ~LCM_PIN_EN;
    delay(32000);
    delay(32000);
    delay(32000);

    SendByte(0x20,0);
    delay(100);
    SendByte(0x20,0);
    delay(100);
    SendByte(0xC0,0);
    delay(100);
    SendByte(0,0);
    delay(100);
    SendByte(0xF0,0);
    delay(100);
    delay(100);
    SendByte(0,0);
    delay(100);
    SendByte(0x10,0);
    delay(10000);

    SendByte(0,0);
    delay(100);
    SendByte(0x60,0);
    delay(100);

    /*
    LCM_OUT = 0x20;
    PulseLCD();
    SendByte(0x28, 0);
    SendByte(0x0E, 0);
    SendByte(0x06, 0);*/
}

//---Печать строки---//
void PrintStr(char *par_string)
{
    char *c;
    c = par_string;
    while ((c != 0) && (*c != 0))
    {
        SendByte(*c, 1);
        delay(50);
        c++;
    }
}

