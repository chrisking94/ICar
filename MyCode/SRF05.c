#include "SRF05.h"

#ifdef COMPILE_SRF05_

#define Trig PBout(11)//������Trig
#define Echo PBin(10)//������Echo

/*
u16 SRF05_measure(void)
{
	Trig=1;
	
	TIM2->CNT = 0;
	
	delayus(100);//!���棺delay���õ�systick
	
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
	
	delayus(20);//!���棺delay���õ�systick
	
	SysTick->LOAD=324000;//36MHz,�����������������18ms
	SysTick->VAL=0;
	Trig=0;
	
    while(!Echo);
	SysTick->CTRL |=  5;//(bit0��en)��ʼ������(bit2)�ڲ�36MHzʱ��Ƶ��
	while(Echo&&!(SysTick->CTRL&(1<<16)));
	while(Echo);
    SysTick->CTRL &= ~1;
	
	
	return (u16)((SysTick->LOAD - SysTick->VAL));
}

#endif
