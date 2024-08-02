#include "main.h"

extern "C" IWDG_HandleTypeDef hiwdg;
extern "C" I2C_HandleTypeDef hi2c1;
extern "C" UART_HandleTypeDef huart1, huart2;
extern "C" TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern "C" ADC_HandleTypeDef hadc1;
extern "C" RTC_HandleTypeDef hrtc;

extern "C" RTC_TimeTypeDef sTime;
extern "C" RTC_DateTypeDef DateToUpdate;

char trans_str[64] = {0,};

void RTC_task();

void ex_task();

int main() {
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    MX_RTC_Init();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) ("1234\n\r"), strlen("1234\n\r"));
    xTaskCreate(reinterpret_cast<TaskFunction_t>(RTC_task), "1", 256, NULL, 2, NULL);
    xTaskCreate(reinterpret_cast<TaskFunction_t>(ex_task), "2", 128, NULL, 3, NULL);
    vTaskStartScheduler();
}

void RTC_task() {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    RtcManager rtc0(hrtc, &huart1, sTime, DateToUpdate);
    rtc0.Init();
    //rtc0.SetTime(12, 0, 0);
    //rtc0.SetDate(24, 8, 1, RTC_WEEKDAY_THURSDAY);
    __enable_irq();
    while (1) {
        rtc0.RtcManager::GetTime();
        /*
        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
        snprintf(trans_str, 63, "Time %d:%d:%d\n\r", sTime.Hours, sTime.Minutes, sTime.Seconds);
        HAL_UART_Transmit(&huart1, (uint8_t *) trans_str, strlen(trans_str), 1000);

        HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
        snprintf(trans_str, 63, "Date %d-%d-20%d\n\r", DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year);
        HAL_UART_Transmit(&huart1, (uint8_t *) trans_str, strlen(trans_str), 1000);
        */
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
        vTaskDelayUntil(&xLastWakeTime, 300);
    }
}