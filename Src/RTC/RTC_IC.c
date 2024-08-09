#include "RTC_IC.h"

extern I2C_HandleTypeDef hi2c1, hi2c2;
extern UART_HandleTypeDef huart1;
const char *weekdayName[7] = {"Sunday\0", "Monday\0", "Tuesday\0", "Wednesday\0", "Thursday\0",
                              "Friday\0", "Saturday\0"};

uint8_t CtrlRegCfg(uint8_t OUT, uint8_t SQWE, uint8_t RS1, uint8_t RS0, I2C_HandleTypeDef hi2cx) {
    if (OUT > 1 || SQWE > 1 || RS1 > 1 || RS0 > 1) {
        return 1;
    } else {
        uint8_t CTRL = OUT << 7 | SQWE << 4 | RS1 << 1 | RS0;
        HAL_I2C_Mem_Write(&hi2cx, RTC_I2C_ADDR, 0x07, I2C_MEMADD_SIZE_8BIT, &CTRL, 1, 100);
        return 0;
    }
}

uint8_t SetDateTime(uint8_t day, uint8_t weekday, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute,
                    uint8_t second, I2C_HandleTypeDef hi2cx) {
    uint8_t data[7] = {0};

    if (second <= 59)
        data[0] = HEX2BCD(second);
    else
        return 1;

    if (minute <= 59)
        data[1] = HEX2BCD(minute);
    else
        return 1;

    // Команда, устанавливающая 24-часовой формат времени
    //HAL_I2C_Mem_Write(&hi2cx, RTC_I2C_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, data, 7, 100);

    if (hour <= 23)
        data[2] = HEX2BCD(hour);
    else
        return 1;

    if ((day >= 1) && (day <= 31))
        data[4] = HEX2BCD(day);
    else
        return 1;

    //Sunday = 0 weekday

    if ((weekday >= 1) && (weekday <= 7))
        data[3] = HEX2BCD(weekday);
    else
        return 1;

    if ((month >= 1) && (month <= 12))
        data[5] = HEX2BCD(month);
    else
        return 1;

    if (year <= 99)
        data[6] = HEX2BCD(year);
    else
        return 1;

    HAL_I2C_Mem_Write(&hi2cx, RTC_I2C_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, data, 7, 100);
    return 0;
}

void GetDateTime(uint8_t *pData, I2C_HandleTypeDef hi2cx) {
    HAL_I2C_Mem_Read(&hi2cx, RTC_I2C_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, pData, 7, 100);
    for (uint8_t i = 0; i <= 6; i++) {
        pData[i] = BCD2HEX(pData[i]);
    }
}