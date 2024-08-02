#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

void SysCLK_DeInit(void);

void SysCLK_Config(void);

void SystemClock_Config(void);

void IWDG_Init(void);

void Buttons_Init(void);

void I2C1_Init(void);

void UART1_Init(void);

void UART2_Init(void);

void Encoder_Init(void);

void ARGB_PreInit(void);

void TIM2_Init(void);

void OneWire_PreInit(void);

void ADC_Init(void);

#ifdef __cplusplus
}
#endif

#endif //SYSTEM_H
