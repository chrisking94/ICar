#include "lcd1.8.h"

#ifdef COMPILE_LCD18_

/*�ı���*/

TextField* p_tf;

void lcd18_initTextField(TextField* pTextField)
{
	pTextField->charsPerRow=pTextField->width/pTextField->font->width;
	pTextField->charsPerColumn=pTextField->height/pTextField->font->height;
}

void lcd18_setTextField(TextField* pTxtField)
{
	p_tf=pTxtField;
}

void lcd18_clrTextField(void)
{
	lcd18_beginFill(p_tf->backColor,true);
	lcd18_drawRect(p_tf->x,p_tf->y,p_tf->width,p_tf->height);
	lcd18_endFill();
	p_tf->charx=0;
	p_tf->chary=0;
}

void lcd18_drawChr(const uchar* chGraphics)
{
	u8 i,j;
	u16 tmpC;
	uchar row;
	u8 x,y;
	
	x=p_tf->x+p_tf->charx*p_tf->font->width;
	y=p_tf->y+p_tf->chary*p_tf->font->height;
	lcd18_setAddressRect(x,y,x+p_tf->font->width-1,y+p_tf->font->height-1);
	
	for(i=0;i<p_tf->font->height;i++)//����
	{
		row=chGraphics[i];//asc2_1206[33][i];
		for(j=0;j<p_tf->font->width;j++)
		{
			if(row&0x80>>j)//˳������
			{
				tmpC=p_tf->foreColor;
			}
			else
			{
				tmpC=p_tf->backColor;
			}
			lcd18_writeInt16(tmpC,LCD18_DAT);
		}
	}
	
	if(p_tf->charx==p_tf->charsPerRow-1)
	{
		p_tf->charx=0;
		p_tf->chary++;
	}
	else
	{
		p_tf->charx++;
	}
	
	if(p_tf->chary==p_tf->charsPerColumn)
	{
		p_tf->chary=0;
	}
}

void lcd18_printChr(char chr)
{
	static u16 SBC_H=0;//ȫ�Ƿ���λ
	u8 i;
	if(chr=='\n')
	{
		if (p_tf->chary==p_tf->charsPerColumn-1)
		{
			//����
			//for (i=p_tf->charx;i<p_tf->charsPerRow;i++)
			//{
			//	lcd18_printChr(' ');
			//}
			p_tf->chary=0;//�ص�����
			p_tf->foreColor=~p_tf->foreColor;//��ɫ
			p_tf->backColor=~p_tf->backColor;//��ɫ
		}
		else
		{
			//����
			//for (i=p_tf->charx;i<p_tf->charsPerRow;i++)
			//{
			//	lcd18_printChr(' ');
			//}
			p_tf->chary++;
		}
		p_tf->charx=0;
		return;
	}
	
	if(chr>127&&SBC_H==0)//��ȫ�ǣ�����
	{
		SBC_H=chr;
		SBC_H<<=8;
		if (p_tf->charx==p_tf->charsPerRow-2)
		{
			p_tf->charx=0;
			if (p_tf->chary==p_tf->charsPerColumn-1)
			{
				p_tf->chary=0;
				p_tf->foreColor=~p_tf->foreColor;//��ɫ
				p_tf->backColor=~p_tf->backColor;//��ɫ
			}
			else
			{
				p_tf->chary++;
			}
		}
	}
	else
	{
		if(SBC_H!=0)//����
		{
			SBC_H|=chr;//��λд��
			SBC_H=getChinese(SBC_H);
			SBC_H*=p_tf->font->height;
			lcd18_drawChr(&p_tf->font->Graphics[SBC_H]);
			SBC_H+=p_tf->font->height;
		}
		else
		{
			SBC_H=(chr-p_tf->font->offset)*p_tf->font->height;//�����ַ���ת�����һά���飨ת���Զ�ά���飩�е�λ��
		}
		
		lcd18_drawChr(&p_tf->font->Graphics[SBC_H]);
		
		SBC_H=0;//�ָ�Ϊ0
	}
}

void lcd18_printStr(char* str)
{
	while(*str)
	{
		lcd18_printChr(*str++);
	}
}

void lcd18_printNum(int num)
{
	uchar buf[10],i=0;
    if (num<0)
    {
        num=~num;
        num+=1;
        lcd18_printChr('-');
    }
    do
    {
        buf[i++]=(uchar)(num%10);
    } while (num/=10);
    while (i--)
    {
        lcd18_printChr(buf[i]+0x30);
    }
}

#endif
