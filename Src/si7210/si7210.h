#ifndef SI7210_H
#define SI7210_H

#include "stm32f1xx.h"
#include <stdbool.h>

/** I2C registers for Si72xx */
#define SI72XX_HREVID 0xC0
#define SI72XX_DSPSIGM 0xC1
#define SI72XX_DSPSIGL 0xC2
#define SI72XX_DSPSIGSEL 0xC3
#define SI72XX_POWER_CTRL 0xC4
#define SI72XX_ARAUTOINC 0xC5
#define SI72XX_CTRL1 0xC6
#define SI72XX_CTRL2 0xC7
#define SI72XX_SLTIME 0xC8
#define SI72XX_CTRL3 0xC9
#define SI72XX_A0 0xCA
#define SI72XX_A1 0xCB
#define SI72XX_A2 0xCC
#define SI72XX_CTRL4 0xCD
#define SI72XX_A3 0xCE
#define SI72XX_A4 0xCF
#define SI72XX_A5 0xD0
#define SI72XX_OTP_ADDR 0xE1
#define SI72XX_OTP_DATA 0xE2
#define SI72XX_OTP_CTRL 0xE3
#define SI72XX_TM_FG 0xE4

#define SI72XX_I2C_ADDR (0x33 << 1)

int32_t Si72xx_ReadFieldValue(I2C_TypeDef *i2c, uint8_t i2cAddr, uint8_t range200mT);

uint32_t Si72xx_WakeUp(I2C_TypeDef *i2c, uint8_t i2cAddr);

uint32_t Si72xx_Write_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t data);

uint32_t Si72xx_Read_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t *data);

uint32_t Si72xx_Read_OTP(I2C_TypeDef *i2c, uint8_t addr, uint8_t otpAddr, uint8_t *data);

uint32_t Si72xx_Read_Data(I2C_TypeDef *i2c, uint8_t addr, int16_t *data);

uint32_t Si72xx_Sleep(I2C_TypeDef *i2c, uint8_t addr);

uint32_t Si72xx_Set_200mT_Range(I2C_TypeDef *i2c, uint8_t addr);

#endif //SI7210_H
