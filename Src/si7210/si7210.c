#include "si7210.h"
/*
int32_t Si72xx_ReadFieldValue(I2C_TypeDef *i2c, uint8_t i2cAddr, uint8_t range200mT) {
    int32_t mT;
    uint8_t read;
    uint32_t result;
    uint8_t flag;
    //wake up the si7200
    result = Si72xx_WakeUp(i2c, addr);
    result |= Si72xx_Write_Register(i2c, addr, 0xC4, ONEBURST_MASK);
    if (range200mT) {
        result |= Si72xx_Set_200mT_Range(i2c, addr);
    }
    result |= Si72xx_Read_Data(i2c, addr, &data);
    if (range200mT) {
        mT = data * 125 / 10; //convert to mT*1000
    } else {
        mT = data * 125 / 100; //convert to mT*1000
    }
    //go back to sleep
    result |= Si72xx_Sleep(i2c, addr);
    return mT;
}

uint32_t Si72xx_WakeUp(I2C_TypeDef *i2c, uint8_t i2cAddr) {
    I2C_TransferSeq_TypeDef seq;
    I2C_TransferReturn_TypeDef ret;
    uint8_t i2c_write_data[1];
    uint8_t i2c_read_data[1];

    seq.addr = i2cAddr;
    seq.flags = I2C_FLAG_WRITE;
    // Select register and data to write
    seq.buf[0].data = i2c_write_data;
    seq.buf[0].len = 0;
    seq.buf[1].data = i2c_read_data;
    seq.buf[1].len = 0;
    ret = I2CSPM_Transfer(i2c, &seq);
    if (ret != i2cTransferDone) {
        return (uint32_t) ret;
    }
    return (uint32_t) 0;
}

uint32_t Si72xx_Write_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t data) {
    I2C_TransferSeq_TypeDef seq;
    I2C_TransferReturn_TypeDef ret;
    uint8_t i2c_write_data[2];
    uint8_t i2c_read_data[1];
    seq.addr = addr;
    seq.flags = I2C_FLAG_WRITE;
    // Select register and data to write
    i2c_write_data[0] = reg;
    i2c_write_data[1] = data;
    seq.buf[0].data = i2c_write_data;
    seq.buf[0].len = 2;
    seq.buf[1].data = i2c_read_data;
    seq.buf[1].len = 0;
    ret = I2CSPM_Transfer(i2c, &seq);
    if (ret != i2cTransferDone) {
        return (uint32_t) ret;
    }
    return (uint32_t) 0;
}

uint32_t Si72xx_Read_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t *data) {
    I2C_TransferSeq_TypeDef seq;
    I2C_TransferReturn_TypeDef ret;
    uint8_t i2c_write_data[1];
    seq.addr = addr;
    seq.flags = I2C_FLAG_WRITE_READ;
    // Select register to start reading from
    i2c_write_data[0] = reg;
    seq.buf[0].data = i2c_write_data;
    seq.buf[0].len = 1;
    // Select length of data to be read
    seq.buf[1].data = data;
    seq.buf[1].len = 1;
    ret = I2CSPM_Transfer(i2c, &seq);
    if (ret != i2cTransferDone) {
        *data = 0xff;
        return (uint32_t) ret;
    }
    return (uint32_t) 0;
}

uint32_t Si72xx_Read_OTP(I2C_TypeDef *i2c, uint8_t addr, uint8_t otpAddr, uint8_t *data) {
    uint8_t read;
    uint32_t result;
    result = Si72xx_Read_Register(i2c, addr, SI72XX_OTP_CTRL, &read);
    if (read & OTP_BUSY_MASK) {
        return SI72XX_ERROR_BUSY;
    }
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_OTP_ADDR, otpAddr);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_OTP_CTRL, OTP_READ_EN_MASK);
    result |= Si72xx_Read_Register(i2c, addr, SI72XX_OTP_DATA, &read);
    *data = read;
    return result;
}

uint32_t Si72xx_Read_Data(I2C_TypeDef *i2c, uint8_t addr, int16_t *data) {
    uint8_t read;
    uint8_t flag;
    uint32_t result;
    int32_t field;
    result = Si72xx_Read_Register(i2c, addr, SI72XX_DSPSIGM, &read);
    flag = read >> 7;
    *data = (((uint16_t) read) & 0x7f) << 8;
    result |= Si72xx_Read_Register(i2c, addr, SI72XX_DSPSIGL, &read);
    *data |= read;
    *data = *data - 16384;
    if (flag == 0) {
        result = SI72XX_ERROR_NODATA;
    }
    return result;
}

uint32_t Si72xx_Sleep(I2C_TypeDef *i2c, uint8_t addr) {
    uint32_t result;
    uint8_t read;
    result = Si72xx_Read_Register(i2c, addr, SI72XX_POWER_CTRL, &read);
    read = read | 0x1;
    read = read & 0x81;
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_POWER_CTRL, read);
    return result;
}

uint32_t Si72xx_Set_200mT_Range(I2C_TypeDef *i2c, uint8_t addr) {
    uint8_t data;
    uint32_t result;
    uint8_t srcAddr = 0x27;
    result = Si72xx_Read_OTP(i2c, addr, srcAddr++, &data);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_A0, data);
    result |= Si72xx_Read_OTP(i2c, addr, srcAddr++, &data);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_A1, data);
    result |= Si72xx_Read_OTP(i2c, addr, srcAddr++, &data);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_A2, data);
    result |= Si72xx_Read_OTP(i2c, addr, srcAddr++, &data);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_A3, data);
    result |= Si72xx_Read_OTP(i2c, addr, srcAddr++, &data);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_A4, data);
    result |= Si72xx_Read_OTP(i2c, addr, srcAddr++, &data);
    result |= Si72xx_Write_Register(i2c, addr, SI72XX_A5, data);
    return result;
}
*/