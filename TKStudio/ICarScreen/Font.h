#ifndef __FONT_H
#define __FONT_H 	   

#include "Common.h"

#define COMPILE_FONT_

typedef struct 
{
	u8 code *Graphics;//��ά�����׵�ַ
	u8 width;
	u8 height;
	u8 offset;//ascii(��������)��������ַ�ͼ���ƫ����
}Font;

extern Font code SystemFont;

u16 getChinese(u16 chi);

#endif
