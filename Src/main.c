#include "main.h"

extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern ADC_HandleTypeDef hadc1;

_Noreturn void task0();
_Noreturn void task1();

#define BUFFER_SIZE_TX 40
#define BUFFER_SIZE_RX 2

char buff[18], buff_uart_tx[BUFFER_SIZE_TX];
uint8_t buff_uart_rx[BUFFER_SIZE_RX];

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    //IWDG_Init();
    Buttons_Init();
    UART1_Init();
    __enable_irq();
    xTaskCreate(task0, "0", 256, NULL, 2, NULL);
    xTaskCreate(task1, "1", 128, NULL, 2, NULL);
    vTaskStartScheduler();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) "Idle task create error!\n\r", 25);
    while (1) {}
}

_Noreturn void task0() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    //TIM1_PWM_Init();
    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

    while (1) {
        //HAL_IWDG_Refresh(&hiwdg);
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        HAL_UART_Transmit_IT(&huart1, "Idle\n\r", 6);
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}

_Noreturn void task1() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        //HAL_IWDG_Refresh(&hiwdg);
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}
/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        HAL_UART_Receive_IT(&huart1, buff_uart_rx, 1);
    }
}
*/