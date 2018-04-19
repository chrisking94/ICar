#ifndef COMMON_H
#define COMMON_H

#include "stm32f10x.h"
#include "system.h"
#include <stdlib.h>

#define DEBUG


#define COMPILE_IR1838_0
#define COMPILE_TACHOMETER_0
#define COMPILE_L298N_0
#define COMPILE_DS3231_0
#define COMPILE_SG90_0
#define COMPILE_SRF05_0
#define COMPILE_TCR5000_0
#define COMPILE_LCD18_0
#define COMPILE_HMC5883_0
#define COMPILE_AT24C32_0

#define COMPILE_POWER_SYSTEM_0
#define COMPILE_MASTER_CONTROL_
#define COMPILE_OBSTACLE_AVOIDING_SYSTEM_0
#define COMPILE_DISPLAY_SYSTEM_0
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

#endif
