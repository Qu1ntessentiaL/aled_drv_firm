#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;

uint8_t red_g = 50, green_g = 50, blue_g = 50;
char buff[20];
DS18B20_t DS18B20_Struct;

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
    __enable_irq();
    UART2_Init();
    DS18B20_Init(&DS18B20_Struct, &huart2);

    DS18B20_InitializationCommand(&DS18B20_Struct);
    DS18B20_ReadRom(&DS18B20_Struct);
    DS18B20_ReadScratchpad(&DS18B20_Struct);

    uint8_t settings[3];
    settings[0] = DS18B20_Struct.temperatureLimitHigh;
    settings[1] = DS18B20_Struct.temperatureLimitLow;
    settings[2] = DS18B20_12_BITS_CONFIG;

    DS18B20_InitializationCommand(&DS18B20_Struct);
    DS18B20_SkipRom(&DS18B20_Struct);
    DS18B20_WriteScratchpad(&DS18B20_Struct, settings);
    while (1) {
        DS18B20_InitializationCommand(&DS18B20_Struct);
        DS18B20_SkipRom(&DS18B20_Struct);
        DS18B20_ConvertT(&DS18B20_Struct, DS18B20_DATA);

        DS18B20_InitializationCommand(&DS18B20_Struct);
        DS18B20_SkipRom(&DS18B20_Struct);
        DS18B20_ReadScratchpad(&DS18B20_Struct);

        HAL_UART_Transmit_IT(&huart1, DS18B20_Struct.serialNumber, 1);

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
            HAL_UART_Transmit_IT(&huart1, "RED\n\r", 5);
        }
        if (BUTTON_GetAction(BUTTON_UP) == BUTTON_LONG_PRESS) {
            red_g = 0;
            green_g = 0;
            blue_g = 0;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "CLEAR\n\r", 7);
        }
        if (BUTTON_GetAction(BUTTON_DOWN) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 255;
            blue_g = 0;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "GREEN\n\r", 7);
        }
        if (BUTTON_GetAction(BUTTON_LEFT) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 0;
            blue_g = 255;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "BLUE\n\r", 6);
        }
        if (BUTTON_GetAction(BUTTON_RIGHT) == BUTTON_SHORT_PRESS) {
            red_g = 255;
            green_g = 255;
            blue_g = 255;
            ARGB_FillRGB(red_g, green_g, blue_g);
            while (!ARGB_Show());
            HAL_UART_Transmit_IT(&huart1, "WHITE\n\r", 7);
        }
        BUTTON_ResetActions();
    }
}