#ifndef SYSTEM_H
#define SYSTEM_H

#include "stm32f1xx.h"

void SysCLK_DeInit(void);

void SysCLK_Config(void);

void SystemClock_Config(void);

void PC13_Init(void);

void I2C1_Init(void);

void UART1_Init(void);

void Encoder_Init(void);

void ARGB_PreInit(void);

void TIM2_Init(void);

void OneWire_PreInit();

#endif //SYSTEM_H
