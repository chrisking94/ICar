#include "LCD1.8.h"
#include "Font.h"

#ifdef COMPILE_LCD18_

/*lcd1.8基类*/

#define lcdRst PAout(6)
#define lcdCs PBout(12)
#define lcdAO PAout(4)//!Reg=0,Dat=1;

u16 bgColor;//background color
u16 pColor;//point color
u16 fColor;//fill color

u16 px=0,py=0;//画笔坐标

bool beginFill=false;
bool lineLess =false;

void lcd18_writeByte(u8 da,bit mod)//!mod=LCD18_DAT或LCD18_REG
{
	lcdAO=mod;
	SPI_write8(da);
}

void lcd18_writeInt16(u16 nDa,bit mod)
{
	lcdAO=mod;
	SPI_write8(nDa>>8);
	SPI_write8(nDa);
}

void lcd18_setAddressRect(u16 x0,u16 y0,u16 x1,u16 y1)
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
	lcdCs=1;
	lcdRst=0;
	delayms(30);
	lcdRst=1;
	delayms(30);
	lcdCs=0;
	
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

void lcd18_cls(void)
{
	u16 i,j;//大于255x255则需改为u16或u32等
	
	lcd18_setAddressRect(0,0,LCD18_W-1,LCD18_H-1);
	for(i=0;i<LCD18_H;i++)//逐行清除
	{
		for(j=0;j<LCD18_W;j++)
		{
			lcd18_writeInt16(bgColor,LCD18_DAT);
		}
	}
}

void lcd18_drawPoint(u16 x,u16 y)
{
	lcd18_setAddressRect(x,y,x,y);
	lcd18_writeInt16(pColor,LCD18_DAT);
}

void lcd18_beginFill(u16 color,bool lineless)
{
	fColor=color;
	beginFill=true;
	lineLess=lineless;//是否有边框
}

void lcd18_endFill(void)
{
	beginFill=false;
	lineLess=false;
}

void lcd18_drawLine(u16 x0,u16 y0,u16 x1,u16 y1)
{
	int xinc,yinc;//x,y增量
	int dltx,dlty;//在x,y轴上的投影长度
	int i,j;
	int x,y;//起点（坐标）
	int incCycle;//表示x或y每格多少个自变量增加(xinc或yinc)
	int tmp;
	
	if(x0==x1)//垂直线
	{
		if(y0>y1)
		{
			tmp=y1;
			y1=y0;
			y0=tmp;
		}
		lcd18_setAddressRect(x0,y0,x0,y1);
		y1-=y0-1;//y1:线长
		for(i=y1;i>0;i--) lcd18_writeInt16(pColor,LCD18_DAT);
		return;
	}else if(y0==y1)//水平线
	{
		if(x0>x1)
		{
			tmp=x1;
			x1=x0;
			x0=tmp;
		}
		lcd18_setAddressRect(x0,y0,x1,y0);
		x1-=x0-1;//x1:线长
		for(i=x1;i>0;i--) lcd18_writeInt16(pColor,LCD18_DAT);
		return;
	}
	
	if(x1>x0)
	{
		dltx=x1-x0;
		x=x0;
		xinc=1;
	}
	else
	{
		dltx=x0-x1;
		x=x1;
		xinc=-1;
	}
	
	if(y1>y0)
	{
		dlty=y1-y0;
		y=y0;
		yinc=1;
	}
	else
	{
		dlty=y0-y1;
		y=y1;
		yinc=-1;
	}
	
	if(dltx>dlty)
	{
		incCycle=dltx/dlty;
		j=0;
		for(i=0;i<dltx;i++)
		{
			x++;
			if(j<incCycle)
			{
				j++;
			}
			else
			{
				j=0;
				y+=yinc;
			}
			lcd18_drawPoint(x,y);
		}
	}
	else
	{
		incCycle=dlty/dltx;
		j=0;
		for(i=0;i<dlty;i++)
		{
			y++;
			if(j<incCycle)
			{
				j++;
			}
			else
			{
				j=0;
				x+=xinc;
			}
			lcd18_drawPoint(x,y);
		}
	}
}

void lcd18_moveTo(u16 x,u16 y)
{
	px=x;
	py=y;
}

void lcd18_lineTo(u16 x,u16 y)
{
	lcd18_drawLine(px,py,x,y);
	px=x;
	py=y;
}

void lcd18_drawRect(u16 left,u16 top,u16 width,u16 height)
{
	u8 i,j;
	
	px=left;
	py=top;
	
	width--;
	height--;
	
	if (!lineLess)
	{
		lcd18_lineTo(left,top+height);
		lcd18_lineTo(left+width,top+height);
		lcd18_lineTo(left+width,top);
		lcd18_lineTo(left,top);
		
		left++;
		top++;
		width--;
		height--;
	}
	
	if(beginFill)
	{
		lcd18_setAddressRect(left,top,left+width-1,top+height-1);
		for(i=height;i>0;i--)
		{
			for(j=width;j>0;j--)
			{
				lcd18_writeInt16(fColor,LCD18_DAT);
			}
		}
	}
}

#endif
