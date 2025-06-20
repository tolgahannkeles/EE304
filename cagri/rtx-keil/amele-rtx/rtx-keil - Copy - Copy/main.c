
/* =====================================================================
 * File: main.c
 * ===================================================================*/
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "dht11.h"
#include "delay.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal_uart.h"

/* CONFIG ----------------------------------------------------------------*/
#define DHT_SAMPLE_INTERVAL_MS  30000
#define LED_HEARTBEAT_MS        1000
#define OVERHEAT_CHECK_MS       500
#define TEMP_OVERHEAT_LIMIT_C   40
#define UART_BAUDRATE           115200

/* SHARED DATA ------------------------------------------------------------*/
volatile uint8_t g_temperature = 0;
volatile uint8_t g_humidity    = 0;
volatile bool    g_overheat    = false;
osMutexId_t      g_dhtMutex;

/* UART handle -----------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* HARDWARE --------------------------------------------------------------*/
static void LED_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_2 | GPIO_PIN_3; /* PA5 HB, PA6 DBG, PA2 TX, PA3 RX */
    cfg.Pull  = GPIO_NOPULL;

    /* LED pins */
    cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    cfg.Speed = GPIO_SPEED_FREQ_LOW;
    cfg.Pin   = GPIO_PIN_5 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &cfg);

    /* UART pins */
    cfg.Mode       = GPIO_MODE_AF_PP;
    cfg.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate  = GPIO_AF7_USART2;
    cfg.Pin        = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &cfg);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);
}

static void UART_Init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = UART_BAUDRATE;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

#ifdef __GNUC__
int _write(int fd, char *data, int len)
{
    (void)fd;
    HAL_UART_Transmit(&huart2, (uint8_t*)data, len, HAL_MAX_DELAY);
    return len;
}
#endif

/* THREADS ---------------------------------------------------------------*/
__NO_RETURN static void led_thread(void *arg)
{
    (void)arg;
    for (;;) {
        if (g_overheat)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        else
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        osDelay(LED_HEARTBEAT_MS);
    }
}

__NO_RETURN static void dht_thread(void *arg)
{
    (void)arg;
    uint8_t t, h;
    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
        if (DHT11_Read(&t, &h) == DHT11_OK) {
            osMutexAcquire(g_dhtMutex, osWaitForever);
            g_temperature = t;
            g_humidity    = h;
            osMutexRelease(g_dhtMutex);
        }
        osDelay(DHT_SAMPLE_INTERVAL_MS);
    }
}

__NO_RETURN static void overheat_thread(void *arg)
{
    (void)arg;
    for (;;) {
        osMutexAcquire(g_dhtMutex, osWaitForever);
        uint8_t t = g_temperature;
        osMutexRelease(g_dhtMutex);
        g_overheat = (t > TEMP_OVERHEAT_LIMIT_C);
        osDelay(OVERHEAT_CHECK_MS);
    }
}

__NO_RETURN static void logger_thread(void *arg)
{
    (void)arg;
    char msg[64];
    for (;;) {
        osMutexAcquire(g_dhtMutex, osWaitForever);
        uint8_t t = g_temperature;
        uint8_t h = g_humidity;
        osMutexRelease(g_dhtMutex);

        int len = snprintf(msg, sizeof(msg), "Temp=%uC Hum=%u%%\r\n", t, h);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
        osDelay(1000);
    }
}

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    DWT_Delay_Init();

    LED_Init();
    UART_Init();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    DHT11_Init(GPIOB, GPIO_PIN_6);

    osKernelInitialize();
    g_dhtMutex = osMutexNew(NULL);

    osThreadNew(led_thread,      NULL, NULL);
    osThreadNew(dht_thread,      NULL, NULL);
    osThreadNew(overheat_thread, NULL, NULL);
    osThreadNew(logger_thread,   NULL, NULL);

    osKernelStart();
    while (1) {}
}
