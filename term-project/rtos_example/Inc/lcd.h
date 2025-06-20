#ifndef LCD_H
#define LCD_H

#include "main.h"

void LCD_Init(void);
void LCD_Send_Command(uint8_t cmd);
void LCD_Send_Char(char data);
void LCD_Send_String(char* str);
void LCD_Set_Cursor(uint8_t row, uint8_t col);

#endif