/*********************************************/
/*使用PB6，PB7。50MHz，复用开漏输出。I2C1     */
/*********************************************/
#ifndef I2C_H
#define I2C_H

#include "Common.h"
#include "usart.h"

#define I2C_SCL PBout(6)
#define I2C_SDA PBout(7)

#define I2C_DutyCycle2 0xBFFF//duty cycle
#define I2C_DutyCycle16_9 0x4000
#define I2C_DutyCycle I2C_DutyCycle2
#define I2C_PCLK 8000000
#define I2C_Speed 40000

#define I2CWrite 0
#define I2CRead 1

enum I2CDeviceAddress
{
    hmc5883=0x3C,//罗盘
    at24c32=0xA0, //EEPROM
    ds3231=0xD0
};

typedef struct
{
    enum I2CDeviceAddress iaddr;
    uchar idata[8];//imod为读取时该缓存器存入指令
    u32 iwl;//write length
    u32 imod;//process mod: read or write
    u32 irl;//read length
}I2CTransmitter;

/***数据接口***/
//extern I2CTransmitter I2CT;

/***接口函数声明***/
//总线
void I2C_init(void);
void I2C_go(void);
//设备
void DS3231_read(void);

#endif
