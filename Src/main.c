#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;

void Buttons_Processing(void *pvParam);

void RTC_task(void *pvParam);

void BQ25892_task(void *pvParam);

const char *weekdayName[7] = {"Sunday\0", "Monday\0", "Tuesday\0", "Wednesday\0", "Thursday\0",
                              "Friday\0", "Saturday\0"};

uint8_t red_g = 50, green_g = 50, blue_g = 50, display = 0;
char buff[18], uart_buff[100];
uint8_t ds1307_data[7], si7210_data[21] = {0};

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
    xTaskCreate(Buttons_Processing, "1", 128, NULL, 2, NULL);
    xTaskCreate(RTC_task, "2", 128, NULL, 3, NULL);
    xTaskCreate(BQ25892_task, "3", 256, NULL, 3, NULL);
    vTaskStartScheduler();
    while (1) {}
}

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
        if (BUTTON_GetAction(UP) == BUTTON_VERY_LONG_PRESS) {
            red_g = 255;
            green_g = 255;
            blue_g = 0;
            HAL_UART_Transmit_IT(&huart1, "YELLOW\n\r", 8);
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
            if (++display > 2) {
                display = 0;
            }
        }
        ARGB_FillRGB(red_g, green_g, blue_g);
        while (!ARGB_Show());
        BUTTON_ResetActions();
        vTaskDelayUntil(&xLastWakeTime, 1);
    }
    vTaskDelete(NULL);
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
        if (display == 0) {
            SSD1306_Fill(SSD1306_COLOR_BLACK);
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts((char *) weekdayName[ds1307_data[3] - 1], &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "%02d.%02d.%02d", ds1307_data[4], ds1307_data[5], ds1307_data[6]);
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "%02d:%02d:%02d", ds1307_data[2], ds1307_data[1], ds1307_data[0]);
            SSD1306_GotoXY(0, 44);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            SSD1306_UpdateScreen();
        }
        else if (display == 1) {
            SSD1306_Fill(SSD1306_COLOR_BLACK);
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts("MagField:", &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "%#X", si7210_data[1]);
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "%#X", si7210_data[2]);
            SSD1306_GotoXY(0, 44);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            SSD1306_UpdateScreen();
        }
        else if (display == 2) {}
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
    vTaskDelete(NULL);
}

void BQ25892_task(void *pvParam) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    if (HAL_I2C_IsDeviceReady(&hi2c1, 0x33 << 1, 3, 100) == HAL_OK) {
        HAL_UART_Transmit_IT(&huart1, "si7210 ready!\n\r", 15);
    } else {
        HAL_UART_Transmit_IT(&huart1, "si7210 connection!\n\r", 22);
    }

    while (1) {
        HAL_I2C_Mem_Read(&hi2c1, 0x33 << 1, 0xC1, I2C_MEMADD_SIZE_8BIT, si7210_data, 21, 100);
        vTaskDelayUntil(&xLastWakeTime, 200);
    }
    vTaskDelete(NULL);
}