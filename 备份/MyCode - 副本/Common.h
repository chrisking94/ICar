#ifndef COMMON_H
#define COMMON_H

#include "stm32f10x.h"
#include "system.h"

#define DEBUG

#define uchar unsigned char
#define u8 unsigned char
#define s8 char
#define uint u32
#define int s32
#define bool u8
#define true 1
#define false 0

#define ledp PAout(13)
#define obeep PAout(14)

void common_init(void);
void delay(u32 s);
void delayms(u16 ms);
void delayus(u16 us);
void beep(u16 time ,u16 interval);
void startTiming(void);
u32 readTimingms(void);

#endif
