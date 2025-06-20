/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
 static void LED_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;           // PA5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Turn LED off initially
}
 
__NO_RETURN static void app_main (void *argument) {
  (void)argument;
  // ...
  for (;;) {}
}

__NO_RETURN static void led_blink_thread(void *argument) {
    (void)argument;
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Toggle LED
        osDelay(100);                          // Delay 500 ms
    }
}

void SystemClock_Config(void); 

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  // ...
 
	HAL_Init();
	SystemClock_Config();
	LED_Init();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
	osThreadNew(led_blink_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
