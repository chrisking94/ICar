#include "DisplaySystem.h"
#include "stdarg.h"
#include "ds3231.h"

#ifdef COMPILE_DISPLAY_SYSTEM_

//**********对象**********
TextField tfCmd;

//##########数据##########


void DisplaySystem_init(void)
{
	//主输出框TextField
	tfCmd.backColor=BLACK;
	tfCmd.x=0;
	tfCmd.y=0;
	tfCmd.width=LCD18_W+1;
	tfCmd.height=LCD18_H+1;
	tfCmd.foreColor=WHITE;
	tfCmd.font=&SystemFont;
	
	lcd18_initTextField(&tfCmd);
	
	lcd18_setTextField(&tfCmd);
	lcd18_clrTextField();
}

void dprintf(char* format,...)
{
	va_list args;
	
	va_start(args,format);
	
	while(*format)
	{
		if(*format=='%')
		{
			switch(*++format)
			{
				case 'c':
					lcd18_printChr((char)va_arg(args,int));
					break;
				case 's':
					lcd18_printStr(va_arg(args,char*));
					break;
				case 'd':
					lcd18_printNum((s32)va_arg(args,s32));
					break;
				case '%':
					lcd18_printChr('%');
					break;
			}
		}
		else
		{
			lcd18_printChr(*format);
		}
		format++;
	}
	
	va_end(args);
}

void DS_printTime(void)
{
	uchar str[3];
	str[2]=0;
	DS3231_writeData(DS3231_MINUTE,8);

	ToClockString(DS3231_readData(DS3231_HOUR),str);
	dprintf(" %s:",str);
	
	ToClockString(DS3231_readData(DS3231_MINUTE),str);
	dprintf("%s:",str);
	
	ToClockString(DS3231_readData(DS3231_SECOND),str);
	dprintf("%s\n",str);
}

void DS_printDate(void)
{
	dprintf(" %d-%d-%d",DS3231_readData(DS3231_YEAR),DS3231_readData(DS3231_MONTH),DS3231_readData(DS3231_DAY));
}

#endif
