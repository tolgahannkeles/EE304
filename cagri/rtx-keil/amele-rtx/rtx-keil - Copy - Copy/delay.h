/* =====================================================================
 * File: delay.h
 * Description: µs delay utilities using DWT cycle counter
 * ===================================================================*/
#ifndef DELAY_H
#define DELAY_H
#include "stm32f4xx.h"
void DWT_Delay_Init(void);
void delay_us(uint32_t us);
#endif /* DELAY_H */