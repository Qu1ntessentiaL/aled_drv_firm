#include "RtcManager.h"

RtcManager::RtcManager(RTC_HandleTypeDef hrtc_handle, UART_HandleTypeDef *huart_handle,
                       RTC_TimeTypeDef currentTime_handle,
                       RTC_DateTypeDef currentDate_handle) : hrtc(hrtc_handle), huart(*huart_handle),
                                                             currentTime(currentTime_handle),
                                                             currentDate(currentDate_handle) {

}

void RtcManager::Init() {
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();

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
    // Обновляем сохраненное время
    UpdateTime();
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
    // Обновляем сохраненную дату
    UpdateDate();
}

void RtcManager::GetTime() {
    // Возвращаем текущее сохраненное время
    PrintTime(currentTime);
}

void RtcManager::GetDate() {
    // Возвращаем текущую сохраненную дату
    PrintDate(currentDate);
}

void RtcManager::SendTimeToUart() {
    // Отправляем сохраненное время по UART
    PrintTime(currentTime);
}

void RtcManager::SendDateToUart() {
    // Отправляем сохраненную дату по UART
    PrintDate(currentDate);
}

void RtcManager::UpdateTime() {
    HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
}

void RtcManager::UpdateDate() {
    HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
}

void RtcManager::PrintTime(const RTC_TimeTypeDef &time) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Time: %02d:%02d:%02d\r\n", time.Hours, time.Minutes, time.Seconds);
    HAL_UART_Transmit(&huart, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
}

void RtcManager::PrintDate(const RTC_DateTypeDef &date) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Date: %02d-%02d-%02d, Weekday: %d\r\n", date.Year, date.Month, date.Date,
             date.WeekDay);
    HAL_UART_Transmit(&huart, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
}

void RtcManager::ErrorHandler() {
    __disable_irq();
    const char *errorMsg = "Error occurred!\r\n";
    HAL_UART_Transmit(&huart, (uint8_t *) errorMsg, strlen(errorMsg), HAL_MAX_DELAY);

    while (1) {}
}