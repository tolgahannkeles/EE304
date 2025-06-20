
/* =====================================================================
 * File: dht11.h
 * ===================================================================*/
#ifndef DHT11_H
#define DHT11_H
#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum { DHT11_OK, DHT11_TIMEOUT, DHT11_CRC } DHT11_Status;

void DHT11_Init(GPIO_TypeDef *port, uint16_t pin);          /* call once */
DHT11_Status DHT11_Read(uint8_t *temperature, uint8_t *humidity);

#endif /* DHT11_H */