#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;

uint8_t red_g = 50, green_g = 50, blue_g = 50;
char buff[18], uart_buff[40];
uint8_t ds1307_data[7];
DS18B20_t DS18B20_Struct;

int main() {
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    __enable_irq();
    SSD1306_Init();
    PC13_Init();
    TIM2_Init();
    HAL_TIM_Base_Start_IT(&htim2);
    ARGB_PreInit();
    ARGB_Init();
    ARGB_SetBrightness(255);
    UART2_Init();
    //SetDateTime(3, 3, 10, 23, 17, 19, 0, hi2c1);
    /*
    DS18B20_Init(&DS18B20_Struct, &huart2);
    DS18B20_InitializationCommand(&DS18B20_Struct);
    if (~DS18B20_ReadRom(&DS18B20_Struct))
        HAL_UART_Transmit_IT(&huart1, "ReadROM complete!\n\r", 19);
    DS18B20_ReadScratchpad(&DS18B20_Struct);

    uint8_t settings[3];
    settings[0] = DS18B20_Struct.temperatureLimitHigh;
    settings[1] = DS18B20_Struct.temperatureLimitLow;
    settings[2] = DS18B20_9_BITS_CONFIG;

    DS18B20_InitializationCommand(&DS18B20_Struct);
    DS18B20_SkipRom(&DS18B20_Struct);
    DS18B20_WriteScratchpad(&DS18B20_Struct, settings);
     */
    while (1) {
        /*
        DS18B20_InitializationCommand(&DS18B20_Struct);
        DS18B20_SkipRom(&DS18B20_Struct);
        DS18B20_ConvertT(&DS18B20_Struct, DS18B20_DATA);

        DS18B20_InitializationCommand(&DS18B20_Struct);
        DS18B20_SkipRom(&DS18B20_Struct);
        DS18B20_ReadScratchpad(&DS18B20_Struct);

        sprintf(uart_buff, "S/N: %02X %02X %02X %02X %02X %02X; t = %.2f\n\r",
                DS18B20_Struct.serialNumber[0],
                DS18B20_Struct.serialNumber[1],
                DS18B20_Struct.serialNumber[2],
                DS18B20_Struct.serialNumber[3],
                DS18B20_Struct.serialNumber[4],
                DS18B20_Struct.serialNumber[5],
                DS18B20_Struct.temperature);

        HAL_UART_Transmit_IT(&huart1, uart_buff, 35);
        */
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        if (HAL_I2C_IsDeviceReady(&hi2c1, RTC_I2C_ADDR, 5, 100) == HAL_OK){
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts("DS1307 = 1", &Font_11x18, SSD1306_COLOR_WHITE);
        } else {
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts("DS1307 = 0", &Font_11x18, SSD1306_COLOR_WHITE);
        }
        if (HAL_I2C_IsDeviceReady(&hi2c1, RTC_I2C_ADDR, 5, 100) == HAL_OK){
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts("AT24C32 = 1", &Font_11x18, SSD1306_COLOR_WHITE);
        } else {
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts("AT24C32 = 0", &Font_11x18, SSD1306_COLOR_WHITE);
        }
        /*
        SSD1306_GotoXY(0, 22);
        sprintf(buff, "GREEN: %d", green_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(0, 44);
        sprintf(buff, "BLUE:  %d", blue_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
         */
        GetDateTime(ds1307_data, hi2c1);
        sprintf(buff, "%d(%d).%d.%d %d:%d:%d\n\r", ds1307_data[4], ds1307_data[3], ds1307_data[5], ds1307_data[6],
                                                        ds1307_data[2], ds1307_data[1], ds1307_data[0]);
        SSD1306_GotoXY(0, 44);
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