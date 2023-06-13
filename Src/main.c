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
    ARGB_PreInit();
    ARGB_Init();
    ARGB_SetBrightness(255);
    __enable_irq();
    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        i = rand() % 256;
        SSD1306_GotoXY(0, 0);
        sprintf(buff, "RED:   %d", i);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        j = rand() % 256;
        SSD1306_GotoXY(0, 22);
        sprintf(buff, "GREEN: %d", j);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        k = rand() % 256;
        SSD1306_GotoXY(0, 44);
        sprintf(buff, "BLUE:  %d", k);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
        ARGB_FillRGB(i, j, k);
        while (!ARGB_Show());
        HAL_Delay(1500);
    }
}
