#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include "main.h"
#include "stm32f1xx.h"

class RtcManager {
public:
    RtcManager(RTC_HandleTypeDef hrtc, UART_HandleTypeDef *huart, RTC_TimeTypeDef currentTime,
               RTC_DateTypeDef currentDate);

    void Init();

    void SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

    void SetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday);

    void GetTime();

    void GetDate();

    void SendTimeToUart();

    void SendDateToUart();

private:
    RTC_HandleTypeDef hrtc;
    UART_HandleTypeDef huart;
    uint8_t uartRxBuffer[100];
    RTC_TimeTypeDef currentTime;
    RTC_DateTypeDef currentDate;

    void UpdateTime();

    void UpdateDate();

    void ErrorHandler();

    void PrintTime(const RTC_TimeTypeDef &time);

    void PrintDate(const RTC_DateTypeDef &date);
};

#endif // RTC_MANAGER_H
