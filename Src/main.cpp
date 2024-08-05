#include "main.h"

extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern ADC_HandleTypeDef hadc1;
extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime_glob;
extern RTC_DateTypeDef sDate_glob;

char trans_str[64] = {0,};

void RTC_task();

void ex_task();

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    __enable_irq();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) ("STM32F103CBT6 ready!\n\r"), strlen("STM32F103CBT6 ready!\n\r"));
    xTaskCreate(reinterpret_cast<TaskFunction_t>(RTC_task), "1", 256, NULL, 3, NULL);
    xTaskCreate(reinterpret_cast<TaskFunction_t>(ex_task), "2", 128, NULL, 1, NULL);
    vTaskStartScheduler();
}

void RTC_task() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    RtcManager rtc0(hrtc, sTime_glob);
    //MX_RTC_Init();
    rtc0.SetTime(12, 30, 15);
    rtc0.SetDate(20, 1, 1, RTC_WEEKDAY_MONDAY);

    while (1) {
        HAL_RTC_GetTime(&hrtc, &sTime_glob, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
        HAL_RTC_GetDate(&hrtc, &sDate_glob, RTC_FORMAT_BIN);
        snprintf(trans_str, 63, "Time %d:%d:%d\n\rDate %d-%d-20%d\n\r", sTime_glob.Hours, sTime_glob.Minutes,
                 sTime_glob.Seconds,
                 sDate_glob.Date, sDate_glob.Month, sDate_glob.Year);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) trans_str, strlen(trans_str));

        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
    //vTaskDelete(NULL);
}

void ex_task() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    Buttons_Init();
    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}