#ifndef I2C_H_
#define I2C_H_

#include <Common.h>

sbit SDA=P1^0;
sbit SCL=P1^1;

/*******************
����IIC
********************/
void IIC_start()
{
    SCL=1;
    SDA=1;
    delay5us();
    SDA=0;
    delay5us();
    SCL=0;
    /*��ʼ�źŽ���*/
}
/*******************
ֹͣIIC
********************/
void IIC_stop()
{
    SDA=0;
    SCL=1;
    delay5us();
    SDA=1;
    delay5us();
    /*�����ź����*/
}
/*******************
����Ӧ��
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
����Ӧ��
********************/
void IIC_sack(bool ack)//Ϊ0����ack
{
    SDA = ack;
    SCL = 1;
    delay5us();
    SCL = 0;
    delay5us();
}
/*******************
���ֽ�
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
д�ֽ�
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