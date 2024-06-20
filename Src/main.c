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
    UART1_Init();
    I2C1_Init();
    __enable_irq();
    xTaskCreate(task0, "0", 128, NULL, 2, NULL);
    xTaskCreate(task1, "1", 256, NULL, 2, NULL);
    vTaskStartScheduler();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) "Idle task create error!\n\r", 25);
    while (1) {}
}

_Noreturn void task0() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}

_Noreturn void task1() {
    //uint8_t si7210_id = 0;
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    if (HAL_I2C_IsDeviceReady(&hi2c1, si7210_addr, 3, 100) == HAL_OK) {
        HAL_UART_Transmit_IT(&huart1, (uint8_t *) "si7210 ready!\n\r", 15);
        //HAL_I2C_Mem_Read(&hi2c1, si7210_addr, 0xC0, I2C_MEMADD_SIZE_8BIT, &si7210_id, 1, 100);
        //sprintf(buff_uart_tx, "chipid: %d, revid: %d\n\r", si7210_id & 0xF0, si7210_id & 0x0F);
        //HAL_UART_Transmit_IT(&huart1, buff_uart_tx, 40);
    } else {
        HAL_UART_Transmit_IT(&huart1, (uint8_t *) "si7210 initialization error!\n\r", 30);
    }
    while (1) {
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