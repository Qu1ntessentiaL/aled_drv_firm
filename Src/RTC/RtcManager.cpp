#include "RtcManager.h"

RtcManager::RtcManager(UART_HandleTypeDef &huart_handle) : huart(&huart_handle) {
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
    if (HAL_RTC_Init(&hrtc) != HAL_OK) {
        ErrorHandler();
    }
}

void RtcManager::SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    RTC_TimeTypeDef sTime = {0};

    sTime.Hours = hours;
    sTime.Minutes = minutes;
    sTime.Seconds = seconds;

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
        ErrorHandler();
    }
}

void RtcManager::SetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday) {
    RTC_DateTypeDef sDate = {0};

    sDate.Year = year;
    sDate.Month = month;
    sDate.Date = day;
    sDate.WeekDay = weekday;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
        ErrorHandler();
    }
}

void RtcManager::ErrorHandler() {
    __disable_irq();
    const char *errorMsg = "Error occurred!\r\n";
    HAL_UART_Transmit(huart, (uint8_t *) errorMsg, strlen(errorMsg), HAL_MAX_DELAY);

    while (1) {}
}

void RtcManager::PrintUART() {
    char trans_str[64] = {0,};
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    snprintf(trans_str, sizeof(trans_str), "Time %d:%d:%d\n\rDate %d-%d-20%d\n\r", sTime.Hours, sTime.Minutes,
             sTime.Seconds,
             sDate.Date, sDate.Month, sDate.Year);
    while (HAL_UART_Transmit_IT(huart, (uint8_t *) trans_str, strlen(trans_str)) != HAL_OK) {}
}
