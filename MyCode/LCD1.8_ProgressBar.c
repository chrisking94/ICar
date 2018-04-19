#include "lcd1.8.h"

#ifdef COMPILE_LCD18_

/*进度条*/

ProgressBar* p_pb;

void lcd18_initProgressBar(ProgressBar* pPB)
{
	lcd18_beginFill(pPB->backColor,true);
	lcd18_drawRect(pPB->x,pPB->y,pPB->width,pPB->height);
	lcd18_endFill();
}

void lcd18_setProgressBar(ProgressBar* pProgressBar)
{
	p_pb=pProgressBar;
}

void lcd18_setPBVal(u8 val)
{
	val=val*p_pb->width/100;
	if(val<p_pb->value)//清除以前画的
	{
		lcd18_beginFill(p_pb->backColor,true);
		lcd18_drawRect(p_pb->x+val,p_pb->y,p_pb->value-val,p_pb->height);
	}
	else
	{
		lcd18_beginFill(p_pb->foreColor,true);
		lcd18_drawRect(p_pb->x+p_pb->value,p_pb->y,val-p_pb->value,p_pb->height);
	}
	
	p_pb->value=val;
	
	lcd18_endFill();
}

#endif
