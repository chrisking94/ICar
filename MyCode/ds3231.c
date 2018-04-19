#include "ds3231.h"
#include "I2C.h"
#include "lcd1.8.h"
#include "DisplaySystem.h"
#include "rtc.h"

#ifdef COMPILE_DS3231_

//extern I2CTransmitter I2CT;

void ToClockString(uchar num,uchar* str)
{
	if(num<10)
	{
		str[0]=0x30;
		str[1]=num+0x30;
	}
	else
	{
		str[0]=num/10+0x30;
		str[1]=num%10+0x30;
	}
}

void DS3231_read()
{
	uchar str[3];
	str[2]=0;
	DS3231_writeData(DS3231_MINUTE,8);

	ToClockString(DS3231_readData(DS3231_HOUR),str);
	//lcd18_setTextField(&tfClockH);
	//lcd18_printStr(str);
	printf("%s:",str);
	
	ToClockString(DS3231_readData(DS3231_MINUTE),str);
	//lcd18_setTextField(&tfClockM);
	//lcd18_printStr(str);
	printf("%s:",str);
	
	ToClockString(DS3231_readData(DS3231_SECOND),str);
	//lcd18_setTextField(&tfClockS);
	//lcd18_printStr(str);
	printf("%s\r\n",str);
}

uchar DS3231_readData(uchar addr)
{
	I2CT.iaddr = ds3231;
	I2CT.iwl = 1;
    I2CT.irl = 1;
    I2CT.imod=I2CRead;
	I2CT.idata[0]=addr;
	I2C_go();
	I2C_wait();
	return Bcd2Dec(I2CT.idata[0]);
}

void DS3231_writeData(uchar addr , uchar data)
{
	I2CT.iaddr = ds3231;
	I2CT.iwl = 2;
    I2CT.imod=I2CWrite;
	I2CT.idata[0]=addr;
	I2CT.idata[1]=Dec2Bcd(data);
	I2C_go();
	I2C_wait();
}

#endif
