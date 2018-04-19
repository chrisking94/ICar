#ifndef __FONT_H
#define __FONT_H 	   

#include "Common.h"

typedef struct 
{
	const u8 *Graphics;//二维数组首地址
	u8 width;
	u8 height;
	u8 offset;//ascii(或其它编)码相对于字符图像的偏移量
}Font;

extern const Font Fixedsys;
extern const Font Digital;

u16 getChinese(u16 chi);

#endif
