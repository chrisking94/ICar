#include "spi.h"

//-----------------------函数声明，变量定义-------------------------------------------------------- 
#include <Common.h> 
#include <intrins.h> 
sbit SCK=P1^6; // 将p1.0口模拟时钟输出 
sbit MOSI=P1^5; // 将p1.1口模拟主机输出 
sbit MISO=P1^0; // 将p1.1口模拟主机输入 
sbit SS1=P1^2; // 将p1.1口模拟片选 
#define delayNOP(); //{_nop_();_nop_();_nop_();_nop_();}; 
//-------------------------------------------------------------------------------------------------- 
// 函数名称： SPISendByte 
// 入口参数： ch 
// 函数功能： 发送一个字节 
//-------------------------------------------------------------------------------------------------- 
void SPISendByte(unsigned char ch) 
{ 
	unsigned char idata n=8; // 向SDA上发送一位数据字节，共八位 
	SCK = 1 ; //时钟置高 
	SS1 = 0 ; //选择从机 
	while(n--) 
	{ 
		delayNOP(); 
		SCK = 0 ; //时钟置低 
		if((ch&0x80) == 0x80) // 若要发送的数据最高位为1则发送位1 
		{ 
			MOSI = 1; // 传送位1 
		} 
		else
		{ 
			MOSI = 0; // 否则传送位0 
		} 
		delayNOP(); 
		ch = ch<<1; // 数据左移一位 
		SCK = 1 ; //时钟置高 
	} 
} 
//-------------------------------------------------------------------------------------------------- 
// 函数名称： SPIreceiveByte 
// 返回接收的数据 
// 函数功能： 接收一字节子程序 
//-------------------------------------------------------------------------------------------------- 
unsigned char SPIreceiveByte() 
{ 
	unsigned char idata n=8; // 从MISO线上读取一上数据字节，共八位 
	unsigned char tdata; 
	SCK = 1; //时钟为高 
	SS1 = 0; //选择从机 
	while(n--) 
	{ 
		delayNOP(); 
		SCK = 0; //时钟为低 
		delayNOP(); 
		tdata = tdata<<1; // 左移一位，或_crol_(temp,1) 
		if(MISO == 1) 
			tdata = tdata|0x01; // 若接收到的位为1，则数据的最后一位置1 
		else
			tdata = tdata&0xfe; // 否则数据的最后一位置0 
		SCK=1; 
	} 
	return(tdata); 
} 
//-------------------------------------------------------------------------------------------------- 
// 函数名称： SPIsend_receiveByte 
// 入口参数： ch 
// 返回接收的数据 
// 函数功能：串行输入/输出子程序 
//-------------------------------------------------------------------------------------------------- 
unsigned char SPIsend_receiveByte(unsigned char ch) 
{ 
	unsigned char idata n=8; // 从MISO线上读取一上数据字节，共八位 
	unsigned char tdata; 
	SCK = 1; //时钟为高 
	SS1 = 0; //选择从机 
	while(n--) 
	{ 
		delayNOP(); 
		SCK = 0; //时钟为低 
		delayNOP(); 
		{ 
			tdata = tdata<<1; // 左移一位，或_crol_(temp,1) 
			if(MISO == 1) 
				tdata = tdata|0x01; // 若接收到的位为1，则数据的最后一位置1 
			else
				tdata = tdata&0xfe; // 否则数据的最后一位置0 
		} 
		{ 
			if((ch&0x80) == 0x80) // 若要发送的数据最高位为1则发送位1 
			{ 
				MOSI = 1; // 传送位1 
			} 
			else
			{ 
				MOSI = 0; // 否则传送位0 
			} 
			ch = ch<<1; // 数据左移一位 
		} 
		SCK=1; 
	} 
	return(tdata); 
}
