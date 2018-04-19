#include "SRF05.h"

#define Trig PBout(11)//超声波Trig
#define Echo PBin(10)//超声波Echo

/*
u16 SRF05_measure(void)
{
	Trig=1;
	
	TIM2->CNT = 0;
	
	delayus(100);//!警告：delay会用到systick
	
	Trig=0;
	
    while(!Echo);
	T2EN();
	while(Echo&&TIM2->CNT!=0);
    T2CE();
	
	return (u16)(TIM2->ARR - TIM2->CNT);
}
*/


u16 SRF05_measure(void)
{
	Trig=1;
	
	delayus(100);//!警告：delay会用到systick
	
	SysTick->LOAD=324000;//36MHz,超声波回声脉宽最多18ms
	SysTick->VAL=0;
	Trig=0;
	
    while(!Echo);
	SysTick->CTRL |=  5;//(bit0，en)开始倒数，(bit2)内部36MHz时钟频率
	while(Echo&&!(SysTick->CTRL&(1<<16)));
    SysTick->CTRL &= ~1;
	
	return (u16)((SysTick->LOAD - SysTick->VAL)/100);
}

