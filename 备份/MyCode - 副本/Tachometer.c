#include "Tachometer.h"

void Tachometer_init(void)
{
	//PB14,15
	AFIO->EXTICR[3] |= //!(AFIO_EXTICR[1~4])
		 1<<8//PB[14]
		|1<<12//PB[15]
		;//!END
	
	EXTI->IMR |= //!(EXTI_IMR)
		 1<<14//开放来自线14上的中断请求
		|1<<15//开放来自线15上的中断请求
		;//!END
	
	EXTI->EMR |= //!(EXTI_EMR)
		 1<<14//开放来自线14上的时间请求
		|1<<15//开放来自线15上的时间请求
		;//!END
	
	EXTI->RTSR |= //!(EXTI_RTSR)上升沿，(EXTI->FTSR)下降沿
		 1<<14//允许输入线14上的上升沿触发
		|1<<15//允许输入线15上的上升沿触发
		;//!END
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR&1<<14)//左电机测速器
	{
		beep(200,200);
		EXTI->PR |= 1<<14;
	}
	if(EXTI->PR&1<<15)//右电机测速器
	{
		beep(200,150);
		EXTI->PR |= 1<<15;
	}
}
