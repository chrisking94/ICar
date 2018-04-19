#ifndef DS3221_H_
#define DS3221_H_

#include "Common.h"

	#ifdef COMPILE_DS3231_
	
#define DS3231_WriteAddress 0xD0
#define DS3231_ReadAddress  0xD1

#define DS3231_SECOND       0x00
#define DS3231_MINUTE       0x01
#define DS3231_HOUR         0x02
#define DS3231_WEEK         0x03
#define DS3231_DAY          0x04
#define DS3231_MONTH        0x05
#define DS3231_YEAR         0x06
//ƒ÷÷”1
#define DS3231_SALARM1ECOND 0x07
#define DS3231_ALARM1MINUTE 0x08
#define DS3231_ALARM1HOUR   0x09
#define DS3231_ALARM1WEEK   0x0A
//ƒ÷÷”2
#define DS3231_ALARM2MINUTE 0x0b
#define DS3231_ALARM2HOUR   0x0c
#define DS3231_ALARM2WEEK   0x0d
#define DS3231_CONTROL      0x0e
#define DS3231_STATUS       0x0f
#define BSY                 2
#define OSF                 7
#define DS3231_XTAL         0x10
#define DS3231_TEMPERATUREH 0x11
#define DS3231_TEMPERATUREL 0x12

void DS3231_read(void);
uchar DS3231_readData(uchar addr);
void DS3231_writeData(uchar addr , uchar data);
void ToClockString(uchar num,uchar* str);

	#endif
		
#endif
