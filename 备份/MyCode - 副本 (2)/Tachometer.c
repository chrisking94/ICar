#include "Tachometer.h"

void Tachometer_init(void)
{
	//PB14,15
	AFIO->EXTICR[3] |= //!(AFIO_EXTICR[1~4])
		 1<<8//PB[14]
		|1<<12//PB[15]
		;//!END
	
	EXTI->IMR |= //!(EXTI_IMR)
		 1<<14//����������14�ϵ��ж�����
		|1<<15//����������15�ϵ��ж�����
		;//!END
	
	EXTI->EMR |= //!(EXTI_EMR)
		 1<<14//����������14�ϵ�ʱ������
		|1<<15//����������15�ϵ�ʱ������
		;//!END
	
	EXTI->RTSR |= //!(EXTI_RTSR)�����أ�(EXTI->FTSR)�½���
		 1<<14//����������14�ϵ������ش���
		|1<<15//����������15�ϵ������ش���
		;//!END
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR&1<<14)//����������
	{
		beep(200,200);
		EXTI->PR |= 1<<14;
	}
	if(EXTI->PR&1<<15)//�ҵ��������
	{
		beep(200,150);
		EXTI->PR |= 1<<15;
	}
}
