#include "LCD1.8.h"

#include "LCD1.8.h"

#define LCD18_DAT 1
#define LCD18_REG 0
#define LCD18_H 160
#define LCD18_W 128

sbit sCs=P1^0;//!低电平有效
sbit sRst=P1^1;
sbit sAO=P1^2;//!Reg=0,Dat=1;

u16 pColor;//point color

void lcd18_writeByte(uchar da,bit mod)//!mod=LCD18_DAT或LCD18_REG
{
	sAO=mod;
	SPI_writeByte(da);
}

void lcd18_writeInt16(uint nDa,bit mod)
{
	sAO=mod;
	SPI_writeByte(nDa>>8);
	SPI_writeByte(nDa);
}

void lcd18_setAddressRect(uint x0,uint y0,uint x1,uint y1)
{
	lcd18_writeByte(0x2A,LCD18_REG);//列设置寄存器
	lcd18_writeInt16(x0,LCD18_DAT);
	lcd18_writeInt16(x1,LCD18_DAT);
	
	lcd18_writeByte(0x2B,LCD18_REG);//行设置寄存器
	lcd18_writeInt16(y0,LCD18_DAT);
	lcd18_writeInt16(y1,LCD18_DAT);
	
	lcd18_writeByte(0x2C,LCD18_REG);//内存写寄存器
}

void lcd18_init(void)
{
	sCs=1;
	sRst=0;
	delayms(20);
	sRst=1;
	delayms(20);
	sCs=0;
	
	lcd18_writeByte(0x11,LCD18_REG);//sleep out
	delayms(120);
	
	//____________________ST7735S帧率设置________________________________//
	lcd18_writeByte(0xB1,LCD18_REG);//%正常模式（全彩）
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA 设置一行的时间
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA：前廊
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA：后廊。PS：在前廊后廊时间内显示数据不从RAM刷新到屏幕
	
	lcd18_writeByte(0xB2,LCD18_REG);//%空闲(idle)模式（8色）
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA 设置一行的时间
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA：前廊
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA：后廊
	
	lcd18_writeByte(0xB3,LCD18_REG);//%偏好(Partial)模式+全彩
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA 设置一行的时间
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA：前廊
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA：后廊
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA 设置一行的时间
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA：前廊
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA：后廊
	//__End__________________ST7735S帧率设置________________________________//
	
	//____________________ST7735S显示、电源设置________________________________//
	lcd18_writeByte(0xB4,LCD18_REG);//%显示反转控制
	lcd18_writeByte(0x03,LCD18_DAT);//NLA,NLB,NLC 设置反转
	
	lcd18_writeByte(0xC0,LCD18_REG);//%电源控制设置1
	lcd18_writeByte(0x28,LCD18_DAT);//AVDD,VRHP（Positive+)  VRHP：设置AVDD电压
	lcd18_writeByte(0x08,LCD18_DAT);//VRHN (-)
	lcd18_writeByte(0x04,LCD18_DAT);//MODE
	
	lcd18_writeByte(0xC1,LCD18_REG);//%电源控制设置2
	lcd18_writeByte(0xC0,LCD18_DAT);//设置VGH/VGL电压
	
	lcd18_writeByte(0xC2,LCD18_REG);//%电源控制设置3，正常模式（全彩）
	lcd18_writeByte(0x0D,LCD18_DAT);//DCA(2) SAPA(3) APA(3)  APA：调整可用的(operational)放大器(amplifier)
	lcd18_writeByte(0x00,LCD18_DAT);//DCA(8)
	
	lcd18_writeByte(0xC3,LCD18_REG);//%电源控制设置4，空闲模式（8色）
	lcd18_writeByte(0x8D,LCD18_DAT);//DCA(2) SAPA(3) APA(3)  APA：调整可用的(operational)放大器(amplifier)
	lcd18_writeByte(0x2A,LCD18_DAT);//DCA(8)
	
	lcd18_writeByte(0xC4,LCD18_REG);//%电源控制设置4，偏好模式+全彩
	lcd18_writeByte(0x8D,LCD18_DAT);//DCA(2) SAPA(3) APA(3)  APA：调整可用的(operational)放大器(amplifier)
	lcd18_writeByte(0xEE,LCD18_DAT);//DCA(8)
	//__End__________________ST7735S显示、电源设置________________________________//
	
	//____________________ST7735S VCOM设置________________________________//
	lcd18_writeByte(0xC5,LCD18_REG);//%显示反转控制
	lcd18_writeByte(0x1A,LCD18_DAT);//VCOM控制设置
	//__End__________________ST7735S VCOM设置________________________________//
	
	//____________________ST7735S 内存数据接口设置________________________________//
	lcd18_writeByte(0x36,LCD18_REG);//%内存数据接口控制
	lcd18_writeByte(0xC0,LCD18_DAT);//MX,MY,RGB模式
	//__End__________________ST7735S 内存数据接口设置________________________________//
	
	//____________________ST7735S Gamma设置________________________________//
	lcd18_writeByte(0xE0,LCD18_REG);//%Gamma调整，+极
	lcd18_writeByte(0x04,LCD18_DAT);
	lcd18_writeByte(0x22,LCD18_DAT);
	lcd18_writeByte(0x07,LCD18_DAT);
	lcd18_writeByte(0x0A,LCD18_DAT);
	lcd18_writeByte(0x2E,LCD18_DAT);
	lcd18_writeByte(0x30,LCD18_DAT);
	lcd18_writeByte(0x25,LCD18_DAT);
	lcd18_writeByte(0x2A,LCD18_DAT);
	lcd18_writeByte(0x28,LCD18_DAT);
	lcd18_writeByte(0x26,LCD18_DAT);
	lcd18_writeByte(0x2E,LCD18_DAT);
	lcd18_writeByte(0x3A,LCD18_DAT);
	lcd18_writeByte(0x00,LCD18_DAT);
	lcd18_writeByte(0x01,LCD18_DAT);
	lcd18_writeByte(0x03,LCD18_DAT);
	lcd18_writeByte(0x13,LCD18_DAT);
	
	lcd18_writeByte(0xE1,LCD18_REG);//%Gamma调整，-极
	lcd18_writeByte(0x04,LCD18_DAT);
	lcd18_writeByte(0x16,LCD18_DAT);
	lcd18_writeByte(0x06,LCD18_DAT);
	lcd18_writeByte(0x0D,LCD18_DAT);
	lcd18_writeByte(0x2D,LCD18_DAT);
	lcd18_writeByte(0x26,LCD18_DAT);
	lcd18_writeByte(0x23,LCD18_DAT);
	lcd18_writeByte(0x27,LCD18_DAT);
	lcd18_writeByte(0x27,LCD18_DAT);
	lcd18_writeByte(0x25,LCD18_DAT);
	lcd18_writeByte(0x2D,LCD18_DAT);
	lcd18_writeByte(0x3B,LCD18_DAT);
	lcd18_writeByte(0x00,LCD18_DAT);
	lcd18_writeByte(0x01,LCD18_DAT);
	lcd18_writeByte(0x04,LCD18_DAT);
	lcd18_writeByte(0x13,LCD18_DAT);
	//__End__________________ST7735S Gamma设置________________________________//
	
	//____________________其他设置________________________________//
	lcd18_writeByte(0x3A,LCD18_REG); //65k mode
	lcd18_writeByte(0x05,LCD18_DAT);
	lcd18_writeByte(0x29,LCD18_REG); //Display on
	//__End__________________其他设置________________________________//
}

void lcd18_cls(u16 color)
{
	u16 i,j;//大于255x255则需改为u16或u32等
	lcd18_setAddressRect(0,0,LCD18_W-1,LCD18_H-1);
	for(i=0;i<LCD18_H;i++)//逐行清除
	{
		for(j=0;j<LCD18_W;j++)
		{
			lcd18_writeInt16(color,LCD18_DAT);
		}
	}
}

void lcd18_drawPoint(u16 x,u16 y)
{
	lcd18_setAddressRect(x,y,x,y);
	lcd18_writeInt16(pColor,LCD18_DAT);
}

void lcd18_fillRect(u16 x0,u16 y0,u16 x1,u16 y1)
{
	u16 i,j;
	
	lcd18_setAddressRect(x0,y0,x1,y1);
	for(i=y0;i<y1;i++)
	{
		for(j=x0;j<x1;j++)
		{
			lcd18_writeInt16(pColor,LCD18_DAT);
		}
	}
}

void lcd18_line(u16 x0,u16 y0,u16 x1,u16 y1)
{
	
}