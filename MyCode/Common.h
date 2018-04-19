#ifndef COMMON_H
#define COMMON_H

#include "stm32f10x.h"
#include "system.h"
#include <stdlib.h>

#define DEBUG


#define COMPILE_IR1838_
#define COMPILE_FC03_
#define COMPILE_L298N_
#define COMPILE_DS3231_					;ds3231,rtc
#define COMPILE_SG90_
#define COMPILE_SRF05_
#define COMPILE_TCR5000_0
#define COMPILE_LCD18_
	#ifdef COMPILE_LCD18_
		#define COMPILE_FONT_
		#define COMPILE_BMP_
	#endif
#define COMPILE_HMC5883_
#define COMPILE_AT24C32_
#define COMPILE_MPU6050_

#define COMPILE_POWER_SYSTEM_
#define COMPILE_MASTER_CONTROL_
#define COMPILE_OBSTACLE_AVOIDING_SYSTEM_
#define COMPILE_DISPLAY_SYSTEM_
#define COMPILE_PATROL_0
#define COMPILE_CELLPHONE_CONTROL_0


#define uchar unsigned char
#define u8 unsigned char
#define s8 char
#define uint u32
#define int s32
#define bit u8
#define bool bit
#define true 1
#define false 0

#define ledp PAout(15)
#define obeep PAout(14)

void common_init(void);
void delay(u32 s);
void delayms(u32 ms);
void delayus(u16 us);
void waitWhile(bool condition);
void beep(u16 time ,u16 interval);
void startTiming(void);
u32 readTimingms(void);

extern u32 Div_U32(u32 n1,u32 n2);
extern	u8	Bcd2Dec(u8 bcd);
extern	u8	Dec2Bcd(u8 dec);

#endif
