#ifndef PERIPH_H
#define PERIPH_H

#include "stm32f1xx.h"

void Error_Handler(void);

void SysCLK_DeInit(void);

void SysCLK_Config(void);

void SystemClock_Config(void);

void PC13_Init(void);

void I2C1_Init(void);

void UART1_Init(void);

void Encoder_Init(void);

void ARGB_PreInit(void);

#endif //PERIPH_H
