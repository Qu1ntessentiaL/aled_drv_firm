#include "RtcManager.h"

RtcManager::RtcManager(UART_HandleTypeDef *uart_handle) : huart(uart_handle) {
    // Constructor implementation
}

void RtcManager::Init() {
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
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

void RtcManager::ProcessUartCommand() {
    if (huart->RxXferCount > 0) {
        // Пример простого парсинга команды (предполагается, что команды заканчиваются '\n')
        char command[100];
        uint16_t len = sizeof(command) - 1;
        memcpy(command, uartRxBuffer, len);
        command[len] = '\0';

        // Обработка команды
        uint8_t hours, minutes, seconds;
        uint8_t year, month, day, weekday;

        if (sscanf(command, "SET_TIME %hhu %hhu %hhu", &hours, &minutes, &seconds) == 3) {
            SetTime(hours, minutes, seconds);
        } else if (sscanf(command, "SET_DATE %hhu %hhu %hhu %hhu", &year, &month, &day, &weekday) == 4) {
            SetDate(year, month, day, weekday);
        } else if (strcmp(command, "GET_TIME") == 0) {
            GetTime();
        } else if (strcmp(command, "GET_DATE") == 0) {
            GetDate();
        }

        // Очистите буфер после обработки
        memset(uartRxBuffer, 0, sizeof(uartRxBuffer));
    }
}

void RtcManager::UpdateTime() {
    HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
}

void RtcManager::UpdateDate() {
    HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
}

void RtcManager::PrintTime(const RTC_TimeTypeDef &time) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Time: %02d:%02d:%02d\n", time.Hours, time.Minutes, time.Seconds);
    HAL_UART_Transmit(huart, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
}

void RtcManager::PrintDate(const RTC_DateTypeDef &date) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Date: %02d-%02d-%02d, Weekday: %d\n", date.Year, date.Month, date.Date,
             date.WeekDay);
    HAL_UART_Transmit(huart, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
}

void RtcManager::ErrorHandler() {
    __disable_irq();
    const char *errorMsg = "Error occurred!\r\n";
    HAL_UART_Transmit(huart, (uint8_t *) errorMsg, strlen(errorMsg), HAL_MAX_DELAY);

    while (1) {}
}