#ifndef __FONT_H
#define __FONT_H 	   

#include "Common.h"

#define COMPILE_FONT_

typedef struct 
{
	u8 code *Graphics;//二维数组首地址
	u8 width;
	u8 height;
	u8 offset;//ascii(或其它编)码相对于字符图像的偏移量
}Font;

extern Font code SystemFont;

u16 getChinese(u16 chi);

#endif
