#include "RTC_periph.h"

// Константы
#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_DAY 86400

RTC_HandleTypeDef hrtc;

uint32_t days_since_epoch(uint16_t year, uint8_t month, uint8_t day);

static uint32_t get_unix_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

static void unix_time_to_date(uint32_t unix_time, uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour,
                              uint8_t *minute, uint8_t *second);

// Функция для проверки високосного года
static inline bool is_leap_year(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Предварительно рассчитанные накопленные дни с начала эпохи для каждого месяца (до февраля каждого года)
const uint16_t cumulative_days_in_month[2][13] = {
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}, // Невисокосный год
        {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}  // Високосный год
};

// Функция для получения количества дней с 1 января 1970 года
static uint32_t days_since_epoch(uint16_t year, uint8_t month, uint8_t day) {
    uint32_t days = 0;

    // Добавляем дни за полные года
    for (uint16_t y = 1970; y < year; y++) {
        days += is_leap_year(y) ? 366 : 365;
    }

    // Используем накопленные дни для месяцев
    days += cumulative_days_in_month[is_leap_year(year)][month - 1];

    // Добавляем дни текущего месяца
    days += day - 1;

    return days;
}

// Функция для получения UNIX-времени
static uint32_t get_unix_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    uint32_t days = days_since_epoch(year, month, day);
    uint32_t total_seconds = days * SECONDS_IN_DAY;
    total_seconds += hour * SECONDS_IN_HOUR;
    total_seconds += minute * SECONDS_IN_MINUTE;
    total_seconds += second;

    return total_seconds;
}

// Функция для преобразования UNIX-времени в дату и время
static void
unix_time_to_date(uint32_t unix_time, uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute,
                  uint8_t *second) {
    uint32_t seconds = unix_time;
    uint16_t days = seconds / SECONDS_IN_DAY;
    seconds %= SECONDS_IN_DAY;
    *hour = seconds / SECONDS_IN_HOUR;
    seconds %= SECONDS_IN_HOUR;
    *minute = seconds / SECONDS_IN_MINUTE;
    *second = seconds % SECONDS_IN_MINUTE;

    // Найти год
    *year = 1970;
    while (true) {
        uint16_t days_in_year = is_leap_year(*year) ? 366 : 365;
        if (days < days_in_year) break;
        days -= days_in_year;
        (*year)++;
    }

    // Найти месяц и день
    const uint16_t *cumulative_days = cumulative_days_in_month[is_leap_year(*year)];
    *month = 1;
    while (days >= cumulative_days[*month]) {
        (*month)++;
    }
    *day = days - cumulative_days[*month - 1] + 1;
}

void DateTime_Set(DateTimeStruct *sDateTime) {
    uint32_t cnt = get_unix_time(sDateTime->Year,
                                 sDateTime->Month,
                                 sDateTime->Date,
                                 sDateTime->Hours,
                                 sDateTime->Minutes,
                                 sDateTime->Seconds);

    // Разблокируем доступ к регистрам RTC
    RTC->CRL |= RTC_CRL_CNF;

    // Записываем значение в регистры CNTH и CNTL
    RTC->CNTH = (cnt >> 16) & 0xFFFF;
    RTC->CNTL = cnt & 0xFFFF;

    // Блокируем доступ к регистрам RTC
    RTC->CRL &= ~RTC_CRL_CNF;

    // Ждем окончания обновления регистров RTC
    while (!(RTC->CRL & RTC_CRL_RTOFF));
}

void DateTime_Get(DateTimeStruct *sDateTime) {
    // Извлечение UNIX-времени из регистров RTC
    uint32_t unix_time = ((uint32_t) RTC->CNTH << 16) | RTC->CNTL;

    // Временные переменные для хранения результата преобразования
    uint16_t year;
    uint8_t month, day, hour, minute, second;

    // Преобразование UNIX-времени в дату и время
    unix_time_to_date(unix_time, &year, &month, &day, &hour, &minute, &second);

    // Заполнение структуры
    sDateTime->Year = year;
    sDateTime->Month = month;
    sDateTime->Date = day;
    sDateTime->Hours = hour;
    sDateTime->Minutes = minute;
    sDateTime->Seconds = second;
}

void MX_RTC_Init(void) {
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
    HAL_RTC_Init(&hrtc);
}