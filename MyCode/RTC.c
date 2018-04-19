#include "rtc.h"
#include "ds3231.h"

#ifdef COMPILE_DS3231_

void RTC_init(void)
{
	RTC->CRL = 0//!(RTC_CRL)
		|1<<5//上一次对RTC的操作已完成
		|1<<4//进入配置模式
		;//!end
	
	
	RTC->CRL &= ~(0
		|1<<5//~上一次对RTC的操作已完成
		|1<<4//~进入配置模式
		);//!end
	
	//u32 *rtc = (u32*)RTCCON;
	//*rtc = 0x9;//复位，允许读写
	//RTC_write(DS3231_readData(DS3231_SECOND),RTC_SEC);
	//RTC_write(DS3231_readData(DS3231_MINUTE),RTC_MIN);
	//RTC_write(DS3231_readData(DS3231_HOUR),RTC_HOU);
	//RTC_write(DS3231_readData(DS3231_DAY),RTC_DAT);
	//RTC_write(DS3231_readData(DS3231_MONTH),RTC_MON);
	//RTC_write(DS3231_readData(DS3231_YEAR),RTC_YEA);
	//RTC_write(9,BCDHON);
}

#endif
