#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;

uint8_t red_g = 50, green_g = 50, blue_g = 50;
char buff[20];

int main() {
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    SSD1306_Init();
    PC13_Init();
    TIM2_Init();
    HAL_TIM_Base_Start_IT(&htim2);
    ARGB_PreInit();
    ARGB_Init();
    __enable_irq();
    while (1) {
        ARGB_FillRGB(red_g, green_g, blue_g);
        while (!ARGB_Show());
        HAL_Delay(500);
    }
}

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        HAL_UART_Transmit_IT(&huart1, "\n\r", 7);
    }
}
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == htim2.Instance) {
        BUTTON_TimerProcess();
        BUTTON_Process();
        if (BUTTON_GetAction(BUTTON_UP) == BUTTON_SHORT_PRESS) {
            red_g += 20;
            HAL_UART_Transmit_IT(&huart1, "red_g increased by 10\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_UP) == BUTTON_VERY_LONG_PRESS) {
            red_g = 255;
            green_g = 35;
            blue_g = 150;
            HAL_UART_Transmit_IT(&huart1, "red_g increased by 10\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_DOWN) == BUTTON_SHORT_PRESS) {
            red_g -= 20;
            HAL_UART_Transmit_IT(&huart1, "red_g reduced by 10\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_LEFT) == BUTTON_SHORT_PRESS) {
            green_g += 20;
            HAL_UART_Transmit_IT(&huart1, "red_g increased by 10\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_RIGHT) == BUTTON_SHORT_PRESS) {
            green_g -= 20;
            HAL_UART_Transmit_IT(&huart1, "red_g reduced by 10\n\r", 20);
        }
        BUTTON_ResetActions();
    }
}