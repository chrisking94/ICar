#ifndef LCD1_8_H_
#define LCD1_8_H_

#include "Common.h"
#include "SPI.h"
#include "Font.h"

#define	COMPILE_LCD18_

#define LCD18_H 160
#define LCD18_W 128

#define LCD18_DAT 1
#define LCD18_REG 0

sbit lcdLed=P1^7;

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

extern u16 bgColor;//background color
extern u16 pColor;//point color

//�ı���
typedef struct
{
	u8 x;
	u8 y;
	u8 width;
	u8 height;
	u8 charsPerRow;
	u8 charsPerColumn;
	u8 charx;
	u8 chary;
	u16 foreColor;
	u16 backColor;
	const Font *font;
	char* text;
}TextField;

/*����*/
void lcd18_init(void);
void lcd18_cls(void);
void lcd18_writeInt16(u16 nDa,bit mod);
void lcd18_beginFill(u16 color,bool lineless);
void lcd18_endFill(void);
void lcd18_drawLine(u16 x0,u16 y0,u16 x1,u16 y1);
void lcd18_setAddressRect(u16 x0,u16 y0,u16 x1,u16 y1);
void lcd18_drawRect(u16 left,u16 top,u16 width,u16 height);
/*�ַ���*/
void lcd18_printChr(char chr);
void lcd18_printStr(char* str);
void lcd18_printNum(int num);
void lcd18_initTextField(TextField* pTextField);//��ʼ��TextField������һЩ��������
void lcd18_setTextField(TextField* pTxtField);
void lcd18_clrTextField(void);

#endif
