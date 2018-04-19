#ifndef __FONT_H
#define __FONT_H 	   

#include "Common.h"

typedef struct 
{
	const u8 *Graphics;//��ά�����׵�ַ
	u8 width;
	u8 height;
	u8 offset;//ascii(��������)��������ַ�ͼ���ƫ����
}Font;

extern const Font SystemFont;

u16 getChinese(u16 chi);

#endif
