#ifndef RTC_PERIPH_H
#define RTC_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"
#include <stdbool.h>

typedef struct {
    uint16_t Year;
    uint8_t Month;
    uint8_t Date;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
} DateTimeStruct;

void DateTime_Set(DateTimeStruct *sDateTime);

void DateTime_Get(DateTimeStruct *sDateTime);

void MX_RTC_Init(void);

#ifdef __cplusplus
}
#endif

#endif //RTC_PERIPH_H
