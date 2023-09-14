#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;

uint8_t red_g = 50, green_g = 50, blue_g = 50;
char buff[20];
float temperature;
uint8_t ROM_tmp[8];

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
    ARGB_SetBrightness(255);
    OneWire_PreInit();
    DS18B20_Init(DS18B20_Resolution_12bits);
    __enable_irq();
    while (1) {
        DS18B20_ReadAll();
        DS18B20_StartAll();
        for (uint8_t i = 0; i < DS18B20_Quantity(); i++) {
            if (DS18B20_GetTemperature(i, &temperature)) {
                DS18B20_GetROM(i, ROM_tmp);
            }
        }
        sprintf(buff, "%f", temperature);
        HAL_UART_Transmit_IT(&huart1, buff, 10);
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        SSD1306_GotoXY(0, 0);
        sprintf(buff, "RED:   %d", red_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(0, 22);
        sprintf(buff, "GREEN: %d", green_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(0, 44);
        sprintf(buff, "BLUE:  %d", blue_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
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
    static uint16_t cnt = 0;
    cnt++;
    if (cnt == 1000) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        cnt = 0;
    }
    if (htim->Instance == htim2.Instance) {
        BUTTON_TimerProcess();
        BUTTON_Process();
        if (BUTTON_GetAction(BUTTON_UP) == BUTTON_SHORT_PRESS) {
            red_g = 255;
            green_g = 0;
            blue_g = 0;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "RED\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_UP) == BUTTON_LONG_PRESS) {
            red_g = 0;
            green_g = 0;
            blue_g = 0;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "CLEAR\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_DOWN) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 255;
            blue_g = 0;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "GREEN\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_LEFT) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 0;
            blue_g = 255;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "BLUE\n\r", 20);
        }
        if (BUTTON_GetAction(BUTTON_RIGHT) == BUTTON_SHORT_PRESS) {
            red_g = 255;
            green_g = 255;
            blue_g = 255;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "WHITE\n\r", 20);
        }
        BUTTON_ResetActions();
    }
}