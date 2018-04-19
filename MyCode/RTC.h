#ifndef RTC_H_
#define RTC_H_

#include "Common.h"	

#define RTCCON	0X57000040
#define RTC_SEC	0X57000070
#define RTC_MIN	RTC_SEC+4
#define RTC_HOU	RTC_MIN+4
#define RTC_DAT	RTC_HOU+4
#define RTC_MON	RTC_DAT+8
#define RTC_YEA	RTC_MON+4	

void RTC_init(void);

extern	u8	RTC_read(u32 addr);
extern	void	RTC_write(u8 val,u32 addr);

#endif
