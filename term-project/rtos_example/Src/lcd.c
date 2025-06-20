#include "lcd.h"
#include "cmsis_os.h" // for FreeRTOS delays

// Define pin macros (match CubeMX)
#define RS_PIN GPIO_PIN_2
#define EN_PIN GPIO_PIN_1
#define D4_PIN GPIO_PIN_4
#define D5_PIN GPIO_PIN_5
#define D6_PIN GPIO_PIN_6
#define D7_PIN GPIO_PIN_7
#define LCD_PORT GPIOA

static void LCD_Enable(void) {
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    osDelay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    osDelay(1);
}

static void LCD_Send_4Bits(uint8_t data) {
		HAL_GPIO_WritePin(LCD_PORT, D4_PIN, ((data >> 0) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LCD_PORT, D5_PIN, ((data >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LCD_PORT, D6_PIN, ((data >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LCD_PORT, D7_PIN, ((data >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LCD_Send_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET);
    LCD_Send_4Bits(cmd >> 4);
    LCD_Enable();
    LCD_Send_4Bits(cmd & 0x0F);
    LCD_Enable();
    osDelay(2);
}

void LCD_Send_Char(char data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET);
    LCD_Send_4Bits(data >> 4);
    LCD_Enable();
    LCD_Send_4Bits(data & 0x0F);
    LCD_Enable();
    osDelay(2);
}

void LCD_Send_String(char* str) {
    while (*str) {
        LCD_Send_Char(*str++);
    }
}

void LCD_Set_Cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    LCD_Send_Command(address + col);
}

void LCD_Init(void) {
    osDelay(40);
    LCD_Send_4Bits(0x03); LCD_Enable(); osDelay(5);
    LCD_Send_4Bits(0x03); LCD_Enable(); osDelay(1);
    LCD_Send_4Bits(0x03); LCD_Enable();
    LCD_Send_4Bits(0x02); LCD_Enable(); // Set 4-bit mode

    LCD_Send_Command(0x28); // 4-bit, 2 line, 5x8 font
    LCD_Send_Command(0x0C); // Display ON, Cursor OFF
    LCD_Send_Command(0x06); // Increment cursor
    LCD_Send_Command(0x01); // Clear display
    osDelay(2);
}