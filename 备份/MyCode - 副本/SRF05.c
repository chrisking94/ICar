#include "SRF05.h"

#define Trig PBout(11)//超声波Trig
#define Echo PBin(10)//超声波Echo

u16 SRF05_measure(void)
{
	Trig=1;
	SysTick->LOAD=648000;//!最大18ms
	delayus(100);
	Trig=0;
    while(!Echo);
	SysTick->CTRL |=  5;//!开始倒数，内部36MHz时钟频率
	while(Echo);
    SysTick->CTRL &= ~1;
	
	return (u16)(SysTick->LOAD);
}