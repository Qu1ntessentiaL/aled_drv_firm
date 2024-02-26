#include "main.h"

extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern ADC_HandleTypeDef hadc1;

void Buttons_task(void *pvParam);

void RTC_task(void *pvParam);

void OLED_task(void *pvParam);

void ADC_task(void *pvParam);

const char *weekdayName[7] = {"Sunday\0", "Monday\0", "Tuesday\0", "Wednesday\0", "Thursday\0",
                              "Friday\0", "Saturday\0"};

#define BUFFER_SIZE_TX 40
#define BUFFER_SIZE_RX 2

uint8_t red_g = 50, green_g = 50, blue_g = 50, display = 2;
char buff[18], buff_uart_tx[BUFFER_SIZE_TX];
uint8_t buff_uart_rx[BUFFER_SIZE_RX];
uint8_t ds1307_data[7];
uint16_t adc1_in0 = 1703;

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    IWDG_Init();
    UART1_Init();
    SSD1306_Init();
    if (HAL_I2C_IsDeviceReady(&hi2c1, AT24CXX_I2C_ADDR, 3, 100) == HAL_OK) {
        HAL_UART_Transmit_IT(&huart1, "EEPROM_IC ready!\n\r", 18);
    } else {
        HAL_UART_Transmit_IT(&huart1, "EEPROM_IC error connection!\n\r", 25);
    }

    ARGB_PreInit();
    ARGB_Init();
    ARGB_SetBrightness(255);
    __enable_irq();
    xTaskCreate(Buttons_task, "1", 128, NULL, 2, NULL);
    xTaskCreate(RTC_task, "2", 128, NULL, 2, NULL);
    xTaskCreate(OLED_task, "3", 256, NULL, 3, NULL);
    xTaskCreate(ADC_task, "4", 256, NULL, 2, NULL);
    vTaskStartScheduler();
    while (1) {
        HAL_Delay(1000);
    }
}

void Buttons_task(void *pvParam) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    Buttons_Init();
    while (1) {
        HAL_IWDG_Refresh(&hiwdg);
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
        if (BUTTON_GetAction(DOWN) == BUTTON_LONG_PRESS) {
            red_g = 0;
            green_g = 255;
            blue_g = 255;
            HAL_UART_Transmit_IT(&huart1, "AQUA\n\r", 6);
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
            red_g = 0;
            green_g = 255;
            blue_g = 255;
            HAL_UART_Transmit_IT(&huart1, "AQUA\n\r", 6);
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
    uint16_t cnt = 0;
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    CtrlRegCfg(1, 1, 1, 1, hi2c1);
    // SetDateTime(20, 4, 12, 23, 11, 32, 0, hi2c1); // Установка времени, закомментировать после использования

    HAL_UART_Receive_IT(&huart1, buff_uart_rx, 1);
    while (1) {
        HAL_IWDG_Refresh(&hiwdg);
        GetDateTime(ds1307_data, hi2c1);
        if (++cnt == 300) {
            sprintf(buff_uart_tx, "%02d(%1d).%02d.%02d %02d:%02d:%02d\n\r", ds1307_data[4], ds1307_data[3],
                    ds1307_data[5],
                    ds1307_data[6], ds1307_data[2], ds1307_data[1],
                    ds1307_data[0]);
            HAL_UART_Transmit_IT(&huart1, buff_uart_tx, 22);
            cnt = 0;
        }
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
    vTaskDelete(NULL);
}

void OLED_task(void *pvParam) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        HAL_IWDG_Refresh(&hiwdg);
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
        } else if (display == 1) {
            SSD1306_Fill(SSD1306_COLOR_BLACK);
            sprintf(buff, "R: %03d", red_g);
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "G: %03d", green_g);
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "B: %03d", blue_g);
            SSD1306_GotoXY(0, 44);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            SSD1306_UpdateScreen();
        } else if (display == 2) {
            SSD1306_Fill(SSD1306_COLOR_BLACK);
            sprintf(buff, "ADC1_IN0:");
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            sprintf(buff, "%04d", adc1_in0);
            SSD1306_GotoXY(0, 22);
            SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
            SSD1306_UpdateScreen();
        }
        vTaskDelayUntil(&xLastWakeTime, 250);
    }
    vTaskDelete(NULL);
}

void ADC_task(void *pvParam) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    ADC_Init();

    while (1) {
        HAL_IWDG_Refresh(&hiwdg);
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adc1_in0, 1);
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
    vTaskDelete(NULL);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (buff_uart_rx[0] == 'r') {
            red_g = 255;
            green_g = 0;
            blue_g = 0;
        } else if (buff_uart_rx[0] == 'g') {
            red_g = 0;
            green_g = 255;
            blue_g = 0;
        } else if (buff_uart_rx[0] == 'b') {
            red_g = 0;
            green_g = 0;
            blue_g = 255;
        }
        HAL_UART_Receive_IT(&huart1, buff_uart_rx, 1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) {}
}