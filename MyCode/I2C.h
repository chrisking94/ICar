/*********************************************/
/*ʹ��PB6��PB7��50MHz�����ÿ�©�����I2C1     */
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
    hmc5883=0x3C,//����
    at24c32=0xA0, //EEPROM
    ds3231=0xD0,
	mpu6050=0xD0//������
};

typedef struct
{
    enum I2CDeviceAddress iaddr;
    uchar idata[8];//imodΪ��ȡʱ�û���������ָ��
    u32 iwl;//write length
    u32 imod;//process mod: read or write
    u32 irl;//read length
}I2CTransmitter;//���ݴ������ṹ��

/***���ݽӿ�***/
extern I2CTransmitter I2CT;

/***�ӿں�������***/
//����
void I2C_init(void);
void I2C_go(void);
void I2C_wait(void);

void I2C_SingleWrite(uchar REG_Address,uchar REG_Data);
uchar I2C_SingleRead(uchar REG_Address);

#endif