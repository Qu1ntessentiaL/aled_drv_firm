#ifndef RTC_H
#define RTC_H

#include "stm32f1xx.h"
#include <stdbool.h>

#define RTC_IC 0
#if (RTC_IC == 0)
#define RTC_RST_CMD  0x58
#define RTC_I2C_ADDR 0xD0
#elif (RTC_IC == 1)
#define RTC_RST_CMD  0x58
#define RTC_I2C_ADDR 0xD0
#endif

uint8_t SetDateTime(uint8_t day, uint8_t weekday, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute,
                    uint8_t second, I2C_HandleTypeDef hi2cx);

void GetDateTime(uint8_t *pData, I2C_HandleTypeDef hi2cx);

static inline uint8_t BCD2HEX(uint8_t BCD) {
    if ((((BCD & 0xF0) >> 4) > 9) || ((BCD & 0x0F) > 9))
        return 0xFF;
    else
        return (((BCD & 0xF0) >> 4) * 10) + (BCD & 0x0F);
}

static inline uint8_t HEX2BCD(uint8_t HEX) {
    return (((HEX / 10) << 4) | (HEX % 10));
}

#endif //RTC_H
