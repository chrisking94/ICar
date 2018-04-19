#include "LCD1.8.h"

#include "LCD1.8.h"

#define LCD18_DAT 1
#define LCD18_REG 0
#define LCD18_H 160
#define LCD18_W 128

sbit sCs=P1^0;//!�͵�ƽ��Ч
sbit sRst=P1^1;
sbit sAO=P1^2;//!Reg=0,Dat=1;

u16 pColor;//point color

void lcd18_writeByte(uchar da,bit mod)//!mod=LCD18_DAT��LCD18_REG
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
	lcd18_writeByte(0x2A,LCD18_REG);//�����üĴ���
	lcd18_writeInt16(x0,LCD18_DAT);
	lcd18_writeInt16(x1,LCD18_DAT);
	
	lcd18_writeByte(0x2B,LCD18_REG);//�����üĴ���
	lcd18_writeInt16(y0,LCD18_DAT);
	lcd18_writeInt16(y1,LCD18_DAT);
	
	lcd18_writeByte(0x2C,LCD18_REG);//�ڴ�д�Ĵ���
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
	
	//____________________ST7735S֡������________________________________//
	lcd18_writeByte(0xB1,LCD18_REG);//%����ģʽ��ȫ�ʣ�
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA ����һ�е�ʱ��
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA��ǰ��
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA�����ȡ�PS����ǰ�Ⱥ���ʱ������ʾ���ݲ���RAMˢ�µ���Ļ
	
	lcd18_writeByte(0xB2,LCD18_REG);//%����(idle)ģʽ��8ɫ��
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA ����һ�е�ʱ��
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA��ǰ��
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA������
	
	lcd18_writeByte(0xB3,LCD18_REG);//%ƫ��(Partial)ģʽ+ȫ��
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA ����һ�е�ʱ��
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA��ǰ��
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA������
	lcd18_writeByte(0x05,LCD18_DAT);//RTNA ����һ�е�ʱ��
	lcd18_writeByte(0x3C,LCD18_DAT);//FPA��ǰ��
	lcd18_writeByte(0x3C,LCD18_DAT);//BPA������
	//__End__________________ST7735S֡������________________________________//
	
	//____________________ST7735S��ʾ����Դ����________________________________//
	lcd18_writeByte(0xB4,LCD18_REG);//%��ʾ��ת����
	lcd18_writeByte(0x03,LCD18_DAT);//NLA,NLB,NLC ���÷�ת
	
	lcd18_writeByte(0xC0,LCD18_REG);//%��Դ��������1
	lcd18_writeByte(0x28,LCD18_DAT);//AVDD,VRHP��Positive+)  VRHP������AVDD��ѹ
	lcd18_writeByte(0x08,LCD18_DAT);//VRHN (-)
	lcd18_writeByte(0x04,LCD18_DAT);//MODE
	
	lcd18_writeByte(0xC1,LCD18_REG);//%��Դ��������2
	lcd18_writeByte(0xC0,LCD18_DAT);//����VGH/VGL��ѹ
	
	lcd18_writeByte(0xC2,LCD18_REG);//%��Դ��������3������ģʽ��ȫ�ʣ�
	lcd18_writeByte(0x0D,LCD18_DAT);//DCA(2) SAPA(3) APA(3)  APA���������õ�(operational)�Ŵ���(amplifier)
	lcd18_writeByte(0x00,LCD18_DAT);//DCA(8)
	
	lcd18_writeByte(0xC3,LCD18_REG);//%��Դ��������4������ģʽ��8ɫ��
	lcd18_writeByte(0x8D,LCD18_DAT);//DCA(2) SAPA(3) APA(3)  APA���������õ�(operational)�Ŵ���(amplifier)
	lcd18_writeByte(0x2A,LCD18_DAT);//DCA(8)
	
	lcd18_writeByte(0xC4,LCD18_REG);//%��Դ��������4��ƫ��ģʽ+ȫ��
	lcd18_writeByte(0x8D,LCD18_DAT);//DCA(2) SAPA(3) APA(3)  APA���������õ�(operational)�Ŵ���(amplifier)
	lcd18_writeByte(0xEE,LCD18_DAT);//DCA(8)
	//__End__________________ST7735S��ʾ����Դ����________________________________//
	
	//____________________ST7735S VCOM����________________________________//
	lcd18_writeByte(0xC5,LCD18_REG);//%��ʾ��ת����
	lcd18_writeByte(0x1A,LCD18_DAT);//VCOM��������
	//__End__________________ST7735S VCOM����________________________________//
	
	//____________________ST7735S �ڴ����ݽӿ�����________________________________//
	lcd18_writeByte(0x36,LCD18_REG);//%�ڴ����ݽӿڿ���
	lcd18_writeByte(0xC0,LCD18_DAT);//MX,MY,RGBģʽ
	//__End__________________ST7735S �ڴ����ݽӿ�����________________________________//
	
	//____________________ST7735S Gamma����________________________________//
	lcd18_writeByte(0xE0,LCD18_REG);//%Gamma������+��
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
	
	lcd18_writeByte(0xE1,LCD18_REG);//%Gamma������-��
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
	//__End__________________ST7735S Gamma����________________________________//
	
	//____________________��������________________________________//
	lcd18_writeByte(0x3A,LCD18_REG); //65k mode
	lcd18_writeByte(0x05,LCD18_DAT);
	lcd18_writeByte(0x29,LCD18_REG); //Display on
	//__End__________________��������________________________________//
}

void lcd18_cls(u16 color)
{
	u16 i,j;//����255x255�����Ϊu16��u32��
	lcd18_setAddressRect(0,0,LCD18_W-1,LCD18_H-1);
	for(i=0;i<LCD18_H;i++)//�������
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