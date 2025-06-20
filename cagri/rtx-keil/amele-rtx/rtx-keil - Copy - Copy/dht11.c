
/* =====================================================================
 * File: dht11.c
 * Description: robust, timeout-protected bit-bang driver for DHT11
 * ===================================================================*/
#include "dht11.h"
#include "delay.h"
#include <stdbool.h>

#define DHT_MAX_WAIT_US  100   /* generic 100 탎 guard */

static GPIO_TypeDef *DHT_PORT;
static uint16_t      DHT_PIN;

/* helpers -----------------------------------------------------------*/
static inline bool DHT_Wait_Pin(uint8_t target_state, uint32_t timeout_us)
{
    while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN) == target_state) {
        if (timeout_us-- == 0) return false;
        delay_us(1);
    }
    return true;
}

static void DHT_Pin_Output(void)
{
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin   = DHT_PIN;
    cfg.Mode  = GPIO_MODE_OUTPUT_OD;
    cfg.Pull  = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT_PORT, &cfg);
}

static void DHT_Pin_Input(void)
{
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin  = DHT_PIN;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT_PORT, &cfg);
}

void DHT11_Init(GPIO_TypeDef *port, uint16_t pin)
{
    DHT_PORT = port;
    DHT_PIN  = pin;
}

/* core --------------------------------------------------------------*/
static DHT11_Status DHT11_ReadRaw(uint8_t raw[5])
{
    uint8_t byte, bit;

    /* 1. START SIGNAL */
    DHT_Pin_Output();
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);                 /* =18 ms LOW */
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);
    delay_us(30);                  /* 20-40 탎 HIGH */
    DHT_Pin_Input();

    /* 2. SENSOR RESPONSE: 80 탎 LOW + 80 탎 HIGH */
    if (!DHT_Wait_Pin(GPIO_PIN_SET,  DHT_MAX_WAIT_US)) return DHT11_TIMEOUT; /* wait LOW  */
    if (!DHT_Wait_Pin(GPIO_PIN_RESET, DHT_MAX_WAIT_US)) return DHT11_TIMEOUT; /* wait HIGH */

    /* 3. READ 40 BITS */
    for (uint8_t i = 0; i < 5; ++i) {
        byte = 0;
        for (bit = 0; bit < 8; ++bit) {
            /* Each bit starts with 50 탎 LOW */
            if (!DHT_Wait_Pin(GPIO_PIN_SET,  DHT_MAX_WAIT_US)) return DHT11_TIMEOUT;

            /* Measure HIGH duration */
            delay_us(40); /* sample at 40 탎: 0 ? ~26-28 탎, 1 ? ~70 탎 */
            if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN) == GPIO_PIN_SET)
                byte |= (1U << (7 - bit));

            /* wait for line to return LOW (end of bit) */
            if (!DHT_Wait_Pin(GPIO_PIN_RESET, DHT_MAX_WAIT_US)) return DHT11_TIMEOUT;
        }
        raw[i] = byte;
    }

    /* 4. CHECKSUM */
    if ((uint8_t)(raw[0] + raw[1] + raw[2] + raw[3]) != raw[4])
        return DHT11_CRC;

    return DHT11_OK;
}

DHT11_Status DHT11_Read(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t raw[5];
    DHT11_Status st = DHT11_ReadRaw(raw);
    if (st == DHT11_OK) {
        *humidity    = raw[0];
        *temperature = raw[2];
    }
    return st;
}
