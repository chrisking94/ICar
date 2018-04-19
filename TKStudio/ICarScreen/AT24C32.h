#ifndef AT24C32_H_
#define AT24C32_H_

#include "Common.h"
#include "I2C.h"

#define AT24C32_WriteAddress 0xa0
#define AT24C32_ReadAddress 0xa1

void delayms(uint n);

void AT24C32_writeByte(uint add,uchar bt)
{
	IIC_start();
	//写命令
	IIC_sendByte(AT24C32_WriteAddress);
	//地址
	IIC_sendByte(add/256);
	IIC_sendByte(add%256);
	//数据
	IIC_sendByte(bt);
	IIC_stop();
}

uchar AT24C32_readByte(uint add)
{
	uchar bt;
	IIC_start();
	//写命令
	IIC_sendByte(AT24C32_WriteAddress);
	//地址
	IIC_sendByte(add/256);
	IIC_sendByte(add%256);
	IIC_start();
	//读命令
	IIC_sendByte(AT24C32_ReadAddress);
	//数据
	bt=IIC_readByte();
	IIC_sack(1);
	IIC_stop();
	return bt;
}
#endif