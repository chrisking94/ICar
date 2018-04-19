#include "SRF05.h"

#define Trig PBout(11)//������Trig
#define Echo PBin(10)//������Echo

u16 SRF05_measure(void)
{
	Trig=1;
	SysTick->LOAD=648000;//!���18ms
	delayus(100);
	Trig=0;
    while(!Echo);
	SysTick->CTRL |=  5;//!��ʼ�������ڲ�36MHzʱ��Ƶ��
	while(Echo);
    SysTick->CTRL &= ~1;
	
	return (u16)(SysTick->LOAD);
}