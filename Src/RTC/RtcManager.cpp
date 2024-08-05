#include "RtcManager.h"

RtcManager::RtcManager(RTC_HandleTypeDef &hrtc_handle, RTC_TimeTypeDef &sTime) : hrtc(&hrtc_handle), sTime(&sTime) {
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();

    hrtc->Instance = RTC;
    hrtc->Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc->Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
    if (HAL_RTC_Init(hrtc) != HAL_OK) {
        ErrorHandler();
    }
}

void RtcManager::SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    sTime->Hours = hours;
    sTime->Minutes = minutes;
    sTime->Seconds = seconds;

    if (HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BIN) != HAL_OK) {
        ErrorHandler();
    }
}

void RtcManager::SetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday) {
    RTC_DateTypeDef sDate = {0};

    sDate.Year = year;
    sDate.Month = month;
    sDate.Date = day;
    sDate.WeekDay = weekday;

    if (HAL_RTC_SetDate(hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
        ErrorHandler();
    }
}

void RtcManager::ErrorHandler() {
    //__disable_irq();
    //const char *errorMsg = "Error occurred!\r\n";
    //HAL_UART_Transmit(&huart, (uint8_t *) errorMsg, strlen(errorMsg), HAL_MAX_DELAY);

    while (1) {}
}