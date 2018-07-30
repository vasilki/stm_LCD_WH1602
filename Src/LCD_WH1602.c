/*
 * LCD_WH1602.c
 *
 *  Created on: 30 июля 2018 г.
 *      Author: vasilek
 */
#include "LCD_WH1602.h"
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

#define LCM_OUT (*gl_line)

//---Функция задержки---//
void delay(int a)
{
    int i = 0;
    int f = 0;
    while(f < a)
    {
        while(i<60)
            {i++;}
        f++;
    }
}

//---Нужная функция для работы с дисплеем, по сути "дергаем ножкой" EN---//
void PulseLCD()
{
    LCM_OUT &= ~LCM_PIN_EN;
    delay(220);
    LCM_OUT |= LCM_PIN_EN;
    delay(220);
    LCM_OUT &= (~LCM_PIN_EN);
    delay(220);
}

//---Отсылка байта в дисплей---//
void SendByte(char ByteToSend, int IsData)
{
    LCM_OUT &= (~LCM_PIN_MASK);
    LCM_OUT |= (ByteToSend & 0xF0);

    if (IsData == 1)
        LCM_OUT |= LCM_PIN_RS;
    else
        LCM_OUT &= ~LCM_PIN_RS;
    PulseLCD();
    LCM_OUT &= (~LCM_PIN_MASK);
    LCM_OUT |= ((ByteToSend & 0x0F) << 4);

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
}

//---Очистка дисплея---//
void ClearLCDScreen()
{
    SendByte(0x01, 0);
    SendByte(0x02, 0);
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
    delay(32000);
    delay(32000);
    delay(32000);
    LCM_OUT &= ~LCM_PIN_RS;
    LCM_OUT &= ~LCM_PIN_EN;
    LCM_OUT = 0x20;
    PulseLCD();
    SendByte(0x28, 0);
    SendByte(0x0E, 0);
    SendByte(0x06, 0);
}

//---Печать строки---//
void PrintStr(char *Text)
{
    char *c;
    c = Text;
    while ((c != 0) && (*c != 0))
    {
        SendByte(*c, 1);
        c++;
    }
}

