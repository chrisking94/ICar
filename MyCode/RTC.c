#include "rtc.h"
#include "ds3231.h"

#ifdef COMPILE_DS3231_

void RTC_init(void)
{
	RTC->CRL = 0//!(RTC_CRL)
		|1<<5//��һ�ζ�RTC�Ĳ��������
		|1<<4//��������ģʽ
		;//!end
	
	
	RTC->CRL &= ~(0
		|1<<5//~��һ�ζ�RTC�Ĳ��������
		|1<<4//~��������ģʽ
		);//!end
	
	//u32 *rtc = (u32*)RTCCON;
	//*rtc = 0x9;//��λ�������д
	//RTC_write(DS3231_readData(DS3231_SECOND),RTC_SEC);
	//RTC_write(DS3231_readData(DS3231_MINUTE),RTC_MIN);
	//RTC_write(DS3231_readData(DS3231_HOUR),RTC_HOU);
	//RTC_write(DS3231_readData(DS3231_DAY),RTC_DAT);
	//RTC_write(DS3231_readData(DS3231_MONTH),RTC_MON);
	//RTC_write(DS3231_readData(DS3231_YEAR),RTC_YEA);
	//RTC_write(9,BCDHON);
}

#endif
