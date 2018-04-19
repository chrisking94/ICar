#include "lcd1.8.h"

#ifdef COMPILE_LCD18_

/*边框*/

Border* p_bd;

void lcd18_setBorder(Border* bder)
{
	p_bd=bder;
}

void lcd18_drawBorder(void)
{
	u8 i,k=0;
	char inc;
	u8 zero,end;//起点和长度
	
	switch (p_bd->position)
	{
	case BDLeft:
		zero=p_bd->y;
		end=p_bd->y+p_bd->height;
		inc=(char)-1;
		break;
	case BDTop:
		zero=p_bd->x;
		end=p_bd->x+p_bd->width;
		inc=(char)-1;
		break;
	case  BDRight:
		zero=p_bd->y+p_bd->width-1;
		end=p_bd->y+p_bd->height-p_bd->width;
		inc=1;
		break;
	case BDBottom:
		zero=p_bd->x+p_bd->height-1;
		end=p_bd->x+zero+p_bd->width-p_bd->height;
		inc=1;
		break;
	}
	
	end--;
	
	switch(p_bd->position)
	{
		case BDLeft:
		case BDRight:
			for(i=0;i<p_bd->width;i++)
			{
				pColor=p_bd->color[k];
				if(k<p_bd->colorCount-1)
				{
					k++;
				}
				else
				{
					k=0;
				}
				lcd18_drawLine(p_bd->x+i,zero,p_bd->x+i,end);
				zero-=inc;
				end+=inc;
			}
			break;
		case BDTop:
		case BDBottom:
			for(i=0;i<p_bd->height;i++)
			{
				pColor=p_bd->color[k];
				if(k<p_bd->colorCount-1)
				{
					k++;
				}
				else
				{
					k=0;
				}
				lcd18_drawLine(zero,p_bd->y+i,end,p_bd->y+i);
				zero-=inc;
				end+=inc;
			}
			break;
	}
}

#endif
