//###测速器###//
#include "FC03.h"
#include "DisplaySystem.h"

#ifdef COMPILE_FC03_

void FC03_init(void)
{
	dprintf("\nFC03...");
	//PB14,15
	AFIO->EXTICR[3] |= 0//!(AFIO_EXTICR[1~4])
		|1<<8//PB[14]
		|1<<12//PB[15]
		;//!END
	
	EXTI->IMR |= 0//!(EXTI_IMR)
		|1<<14//开放来自线14上的中断请求
		|1<<15//开放来自线15上的中断请求
		;//!END
	
	EXTI->EMR |= 0//!(EXTI_EMR)
		//|1<<14//开放来自线14上的事件请求
		//|1<<15//开放来自线15上的事件请求
		;//!END
	
	EXTI->FTSR |= 0//!(EXTI_RTSR)上升沿，(EXTI->FTSR)下降沿
		|1<<14//允许输入线14上的上升沿触发
		|1<<15//允许输入线15上的上升沿触发
		;//!END
	dprintf("OK!");
}

u16 TMLSpeed=0;
u16 TMRSpeed=0;
void EXTI15_10_IRQHandler(void)//外部中断里面使用delayus会出错，导致卡在中断里跳不出
{
	if(EXTI->PR&1<<15)//右电机测速器
	{
		TMRSpeed++;
		EXTI->PR |= 1<<15;
	}
	if(EXTI->PR&1<<14)//左电机测速器
	{
		TMLSpeed++;
		EXTI->PR |= 1<<14;
	}
}

#endif
