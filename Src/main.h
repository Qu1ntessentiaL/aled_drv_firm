#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f1xx_hal.h"
#include "ssd1306/ssd1306.h"
#include "system.h"
#include "ARGB/ARGB.h"
#include "button/button.h"
#include "ds18x20/OneWire.h"
#include "ds18x20/ds18x20.h"
#include "RTC/RTC.h"
#include "at24cxx/at24cxx.h"

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
