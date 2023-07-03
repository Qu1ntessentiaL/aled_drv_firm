#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1, htim3;

uint8_t i, j, k;
char buff[20];

int main() {
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    SSD1306_Init();
    PC13_Init();
    TIM2_Init();
    ARGB_PreInit();
    ARGB_Init();
    __enable_irq();
    while (1) {
        i = rand() % 256;
        j = rand() % 256;
        k = rand() % 256;
        ARGB_FillRGB(i, j, k);
        while (!ARGB_Show());
        HAL_Delay(2000);
    }
}
