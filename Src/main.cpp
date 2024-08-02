#include "main.h"

extern "C" IWDG_HandleTypeDef hiwdg;
extern "C" I2C_HandleTypeDef hi2c1;
extern "C" UART_HandleTypeDef huart1, huart2;
extern "C" TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern "C" ADC_HandleTypeDef hadc1;

void RTC_task();

#define BUFFER_SIZE_TX 40
#define BUFFER_SIZE_RX 2

char buff[18], buff_uart_tx[BUFFER_SIZE_TX];
uint8_t buff_uart_rx[BUFFER_SIZE_RX];

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    __enable_irq();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) ("1234\n\r"), strlen("1234\n\r"));
    xTaskCreate(reinterpret_cast<TaskFunction_t>(RTC_task), "2", 128, NULL, 2, NULL);
    vTaskStartScheduler();
}

void RTC_task() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    Buttons_Init();
    RtcManager rtc0(&huart1);
    rtc0.Init();
    rtc0.SetTime(12, 0, 0);
    rtc0.SetDate(24, 8, 1, RTC_WEEKDAY_THURSDAY);
    while (1) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
    //vTaskDelete(NULL);
}
