#ifndef I2C_H_
#define I2C_H_

#include <Common.h>

sbit SDA=P1^0;
sbit SCL=P1^1;

/*******************
启动IIC
********************/
void IIC_start()
{
    SCL=1;
    SDA=1;
    delay5us();
    SDA=0;
    delay5us();
    SCL=0;
    /*起始信号结束*/
}
/*******************
停止IIC
********************/
void IIC_stop()
{
    SDA=0;
    SCL=1;
    delay5us();
    SDA=1;
    delay5us();
    /*结束信号完成*/
}
/*******************
接收应答
********************/
bool IIC_rack()
{
    SCL = 1;
    delay5us();
    CY = SDA;
    SCL = 0;
    delay5us();

    return CY;
}
/*******************
发送应答
********************/
void IIC_sack(bool ack)//为0则发送ack
{
    SDA = ack;
    SCL = 1;
    delay5us();
    SCL = 0;
    delay5us();
}
/*******************
读字节
********************/
uchar i;
bool IIC_sendByte(uchar bt)
{
    for (i=0;i<8;i++)
    {
        bt <<= 1;
        SDA = CY;
        SCL = 1;
        delay5us();
        SCL = 0;
        delay5us();
    }
    return ~IIC_rack();
}
/*******************
写字节
********************/
uchar IIC_readByte()
{
    uchar bt=0;
		
		SDA=1;
    for (i=0;i<8;i++)
    {
        bt<<=1;
        SCL=1;
        delay5us();
        bt|=SDA;
        SCL=0;
        delay5us();
    }
    return bt;
}
#endif