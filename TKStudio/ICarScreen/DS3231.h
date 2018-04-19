#ifndef DS3231_H_
#define DS3231_H_

#include "I2C.h"

#define DS3231_WriteAddress 0xD0
#define DS3231_ReadAddress  0xD1

#define DS3231_SECOND       0x00
#define DS3231_MINUTE       0x01
#define DS3231_HOUR         0x02
#define DS3231_WEEK         0x03
#define DS3231_DAY          0x04
#define DS3231_MONTH        0x05
#define DS3231_YEAR         0x06
//闹钟1
#define DS3231_SALARM1ECOND 0x07
#define DS3231_ALARM1MINUTE 0x08
#define DS3231_ALARM1HOUR   0x09
#define DS3231_ALARM1WEEK   0x0A
//闹钟2
#define DS3231_ALARM2MINUTE 0x0b
#define DS3231_ALARM2HOUR   0x0c
#define DS3231_ALARM2WEEK   0x0d
#define DS3231_CONTROL      0x0e
#define DS3231_STATUS       0x0f
#define BSY                 2
#define OSF                 7
#define DS3231_XTAL         0x10
#define DS3231_TEMPERATUREH 0x11
#define DS3231_TEMPERATUREL 0x12

uchar BCD_HEX(uchar val)
{
	//BCD编码：4bit表示1个十进制数码
	uchar tmp;
	tmp=val&0x0f;//高四位置零
	val>>=4;
	val&=0x0f;//高四位已经为零?
	val*=10;
	tmp+=val;
	//0xxx xxxx => (0xxx)*10+xxxx
	
	return tmp;
}

uchar HEX_BCD(uchar val)
{
	uchar i,j,k;
	i=val/10;
	j=val;
	k=j+(i<<4);
	return k;
}

bool DS3231_writeByte(uchar add,uchar dat)
{
	IIC_start();
	IIC_sendByte(DS3231_WriteAddress);
	IIC_sendByte(add);
	IIC_sendByte(dat);
	IIC_stop();
	return 1;
}

uchar DS3231_readCurrent()//读取当前字节
{
	uchar ret;
	IIC_start();
	IIC_sendByte(DS3231_ReadAddress);
	ret=IIC_readByte();
	IIC_sack(1);
	IIC_stop();
	return ret;
}

uchar DS3231_readByte(uchar add)
{
	IIC_start();
	IIC_sendByte(DS3231_WriteAddress);
	IIC_sendByte(add);
	return (IIC_readByte());
}

void DS3231_writeData(uchar add,uchar dat)//DS3231_YEAR,...
{
	uchar tmp=0;
	
	tmp=HEX_BCD(dat);
	DS3231_writeByte(add,tmp);
}

uchar DS3231_readData(uchar add)//add=DS3231_HOUR,...
{
	uchar tmp;
	
	tmp=DS3231_readByte(add);
	if(add==DS3231_HOUR) tmp&=0x3f;
	tmp=BCD_HEX(tmp);
	
	return tmp;
}

int DS3231_readTemperature()
{
	int tmp;
	tmp=DS3231_readData(DS3231_TEMPERATUREH);
	tmp<<=8;
	tmp|=DS3231_readData(DS3231_TEMPERATUREL);
	
	return tmp;
}
#endif