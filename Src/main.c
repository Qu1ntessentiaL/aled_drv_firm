#include "main.h"
#include <time32.h>

extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern ADC_HandleTypeDef hadc1;
extern RTC_HandleTypeDef hrtc;

DateTimeStruct sDateTime;

uint8_t buff_rx[3];
bool st_flag = false, sd_flag = false;

void RTC_task(void *pvParameters);

void Buttons_task(void *pvParameters);

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    Button_PreInit();
    BUTTON_Init();
    __enable_irq();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) ("STM32F103CBT6 ready!\n\r"), strlen("STM32F103CBT6 ready!\n\r"));
    xTaskCreate(RTC_task, "RTC task", 128, NULL, 3, NULL);
    xTaskCreate(Buttons_task, "Buttons task", 128, NULL, 3, NULL);
    vTaskStartScheduler();
}

void RTC_task(void *pvParameters) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    MX_RTC_Init();
    GPIOB->BSRR |= GPIO_BSRR_BR0;
    while (1) {
        //HAL_RTC_GetTime(&hrtc, &sTime_glob, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
        //HAL_RTC_GetDate(&hrtc, &sDate_glob, RTC_FORMAT_BIN);
        DateTime_Get(&sDateTime);
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
    //vTaskDelete(NULL);
}

void Buttons_task(void *pvParameters) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    char buff_tx[64] = {0,};
    Button_PreInit();
    while (1) {
        BUTTON_TimerProcess();
        BUTTON_Process();
        if (BUTTON_GetAction(REC) == BUTTON_SHORT_PRESS) {
            snprintf(buff_tx, 63, "Time %d:%d:%d\n\rDate %d-%d-%d\n\r", sDateTime.Hours, sDateTime.Minutes,
                     sDateTime.Seconds,
                     sDateTime.Date, sDateTime.Month, sDateTime.Year);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) buff_tx, strlen(buff_tx));
        }
        if (BUTTON_GetAction(PHOTO) == BUTTON_SHORT_PRESS) {
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) "Insert time [3-byte: hours, minutes, seconds]:\n\r",
                                  strlen("Insert time [3-byte: hours, minutes, seconds]:\n\r"));
            HAL_UART_Receive_IT(&huart1, buff_rx, 3);
            st_flag = true;
        }
        if (BUTTON_GetAction(UP) == BUTTON_SHORT_PRESS) {
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) "Insert date [3-byte: year, month, date]:\n\r",
                                  strlen("Insert date [3-byte: year, month, date]:\n\r"));
            HAL_UART_Receive_IT(&huart1, buff_rx, 3);
            sd_flag = true;
        }
        if (BUTTON_GetAction(LEFT) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(MENU) == BUTTON_SHORT_PRESS) {
            uint32_t cnt_local = ((uint32_t) (RTC->CNTH << 16)) + (uint32_t) RTC->CNTL;
            char buffer[12];
            sprintf(buffer, "%u\n\r", cnt_local);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) buffer, strlen(buffer));
        }
        if (BUTTON_GetAction(RIGHT) == BUTTON_SHORT_PRESS) {
            DateTime_Set(2024, 8, 9, 10, 0, 0);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) "DateTime set!\n\r", sizeof("DateTime set!\n\r"));
        }
        if (BUTTON_GetAction(DOWN) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(SCENE) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(AWB) == BUTTON_SHORT_PRESS) {

        }
        BUTTON_ResetActions();
        vTaskDelayUntil(&xLastWakeTime, 1);
    }
    //vTaskDelete(NULL);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    /*
    if (st_flag) {
        sTime_glob.Hours = buff_rx[0];
        sTime_glob.Minutes = buff_rx[1];
        sTime_glob.Seconds = buff_rx[2];
        HAL_RTC_SetTime(&hrtc, &sTime_glob, RTC_FORMAT_BIN);
        st_flag = false;
    }
    if (sd_flag) {
        sDate_glob.Year = buff_rx[0];
        sDate_glob.Month = buff_rx[1];
        sDate_glob.Date = buff_rx[2];
        HAL_RTC_SetDate(&hrtc, &sDate_glob, RTC_FORMAT_BIN);
        sd_flag = false;
    }
     */
}