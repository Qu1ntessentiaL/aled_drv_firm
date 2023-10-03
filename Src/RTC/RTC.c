#include "RTC.h"

extern I2C_HandleTypeDef hi2c1, hi2c2;
extern UART_HandleTypeDef huart1;

uint8_t SetDateTime(uint8_t day, uint8_t weekday, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute,
                    uint8_t second, I2C_HandleTypeDef hi2cx) {
    uint8_t data[7] = {0};

    if ((second > 59) || (minute > 59) || (hour > 23) || (day == 0) || (day > 31) ||
        (weekday == 0) || (weekday > 7) || (month == 0) || (month > 12) || (year > 99)) {
        return 1;
    } else {
        for (uint8_t i = 0; i <= 6; i++) {
            data[i] = HEX2BCD(data[i]);
        }
    }

    HAL_I2C_Mem_Write(&hi2cx, RTC_I2C_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, data, 7, 100);
    return 0;
}

void GetDateTime(uint8_t *pData, I2C_HandleTypeDef hi2cx) {
    HAL_I2C_Mem_Read(&hi2cx, RTC_I2C_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, pData, 7, 100);
    for (uint8_t i = 0; i <= 6; i++) {
        pData[i] = BCD2HEX(pData[i]);
    }
}