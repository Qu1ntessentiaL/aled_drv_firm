#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;

void Buttons_Processing(void *pvParam);

void RTC_task(void *pvParam);

uint8_t red_g = 50, green_g = 50, blue_g = 50;
char buff[18], uart_buff[40];
uint8_t ds1307_data[7];
DS18B20_t DS18B20_Struct;

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    SSD1306_Init();
    ARGB_PreInit();
    ARGB_Init();
    ARGB_SetBrightness(255);
    __enable_irq();
    xTaskCreate(Buttons_Processing, "", 128, NULL, 3, NULL);
    xTaskCreate(RTC_task, "", 128, NULL, 2, NULL);
    vTaskStartScheduler();
    /*
    ARGB_PreInit();
    ARGB_Init();
    ARGB_SetBrightness(255);
    UART2_Init();
    DS18B20_Init(&DS18B20_Struct, &huart2);
    DS18B20_InitializationCommand(&DS18B20_Struct);
    if (~DS18B20_ReadRom(&DS18B20_Struct)) {
        HAL_UART_Transmit_IT(&huart1, "ReadROM complete!\n\r", 19);
    }
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
        DS18B20_InitializationCommand(&DS18B20_Struct);
        DS18B20_SkipRom(&DS18B20_Struct);
        DS18B20_ConvertT(&DS18B20_Struct, DS18B20_DATA);

        DS18B20_InitializationCommand(&DS18B20_Struct);
        DS18B20_SkipRom(&DS18B20_Struct);
        DS18B20_ReadScratchpad(&DS18B20_Struct);
        /*
        sprintf(uart_buff, "S/N: %02X %02X %02X %02X %02X %02X; t = %.2f\n\r",
                DS18B20_Struct.serialNumber[0],
                DS18B20_Struct.serialNumber[1],
                DS18B20_Struct.serialNumber[2],
                DS18B20_Struct.serialNumber[3],
                DS18B20_Struct.serialNumber[4],
                DS18B20_Struct.serialNumber[5],
                DS18B20_Struct.temperature);
        HAL_UART_Transmit_IT(&huart1, uart_buff, 35);
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        if (HAL_I2C_IsDeviceReady(&hi2c1, RTC_I2C_ADDR, 5, 100) == HAL_OK) {
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts("DS1307 = 1", &Font_11x18, SSD1306_COLOR_WHITE);
        } else {
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts("DS1307 = 0", &Font_11x18, SSD1306_COLOR_WHITE);
        }
        if (HAL_I2C_IsDeviceReady(&hi2c1, RTC_I2C_ADDR, 5, 100) == HAL_OK) {
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts("AT24C32 = 1", &Font_11x18, SSD1306_COLOR_WHITE);
        } else {
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts("AT24C32 = 0", &Font_11x18, SSD1306_COLOR_WHITE);
        }
        SSD1306_GotoXY(0, 22);
        sprintf(buff, "GREEN: %d", green_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(0, 44);
        sprintf(buff, "BLUE:  %d", blue_g);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();

        SSD1306_GotoXY(0, 0);
        sprintf(buff, "t = %.2f", DS18B20_Struct.temperature);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        sprintf(buff, "%d(%d).%d.%d", ds1307_data[4], ds1307_data[3], ds1307_data[5], ds1307_data[6]);
        SSD1306_GotoXY(0, 22);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
        sprintf(buff, "%02d:%02d:%02d", ds1307_data[2], ds1307_data[1], ds1307_data[0]);
        SSD1306_GotoXY(0, 44);
        SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
        */
    }
}

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        HAL_UART_Transmit_IT(&huart1, "\n\r", 7);
    }
}
*/

void Buttons_Processing(void *pvParam) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    Buttons_Init();
    while (1) {
        BUTTON_TimerProcess();
        BUTTON_Process();
        if (BUTTON_GetAction(UP) == BUTTON_SHORT_PRESS) {
            red_g = 255;
            green_g = 0;
            blue_g = 0;
            HAL_UART_Transmit_IT(&huart1, "RED\n\r", 5);
        }
        if (BUTTON_GetAction(UP) == BUTTON_LONG_PRESS) {
            red_g = 255;
            green_g = 0;
            blue_g = 255;
            HAL_UART_Transmit_IT(&huart1, "PURPLE\n\r", 8);
        }
        if (BUTTON_GetAction(DOWN) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 255;
            blue_g = 0;
            HAL_UART_Transmit_IT(&huart1, "GREEN\n\r", 7);
        }
        if (BUTTON_GetAction(LEFT) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 0;
            blue_g = 255;
            HAL_UART_Transmit_IT(&huart1, "BLUE\n\r", 6);
        }
        if (BUTTON_GetAction(RIGHT) == BUTTON_SHORT_PRESS) {
            red_g = 255;
            green_g = 255;
            blue_g = 255;
            HAL_UART_Transmit_IT(&huart1, "WHITE\n\r", 7);
        }
        if (BUTTON_GetAction(MENU) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 0;
            blue_g = 0;
            HAL_UART_Transmit_IT(&huart1, "CLEAR\n\r", 7);
        }
        if (BUTTON_GetAction(REC) == BUTTON_SHORT_PRESS) {
            red_g = 255;
            green_g = 255;
            blue_g = 0;
            HAL_UART_Transmit_IT(&huart1, "YELLOW\n\r", 8);
        }
        if (BUTTON_GetAction(PHOTO) == BUTTON_SHORT_PRESS) {
            red_g = 0;
            green_g = 255;
            blue_g = 255;
            HAL_UART_Transmit_IT(&huart1, "AQUA\n\r", 6);
        }
        ARGB_FillRGB(red_g, green_g, blue_g);
        while (!ARGB_Show());
        BUTTON_ResetActions();
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        vTaskDelayUntil(&xLastWakeTime, 1);
    }
}

void RTC_task(void *pvParam) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    if (HAL_I2C_IsDeviceReady(&hi2c1, RTC_I2C_ADDR, 3, 100) == HAL_OK) {
        HAL_UART_Transmit_IT(&huart1, "RTC IC ready!\n\r", 15);
    } else {
        HAL_UART_Transmit_IT(&huart1, "RTC IC error connection!\n\r", 22);
    }
    CtrlRegCfg(1, 1, 1, 1, hi2c1);

    while (1) {
        GetDateTime(ds1307_data, hi2c1);
        sprintf(uart_buff, "%02d(%1d).%02d.%02d %02d:%02d:%02d\n\r", ds1307_data[4], ds1307_data[3], ds1307_data[5],
                ds1307_data[6], ds1307_data[2], ds1307_data[1],
                ds1307_data[0]);
        HAL_UART_Transmit_IT(&huart1, uart_buff, 22);
        vTaskDelayUntil(&xLastWakeTime, 5000);
    }
}