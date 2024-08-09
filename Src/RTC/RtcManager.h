#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <iostream>

#ifdef __cplusplus

extern "C" {
#endif

#include "stm32f1xx.h"

class RtcManager {
public:
    RtcManager(UART_HandleTypeDef &huart);

    void SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

    void SetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday);

    void PrintUART();

private:
    RTC_HandleTypeDef hrtc;
    UART_HandleTypeDef *huart;
    void ErrorHandler();
};

#ifdef __cplusplus
}
#endif

#endif // RTC_MANAGER_H
