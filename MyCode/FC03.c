//###������###//
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
		|1<<14//����������14�ϵ��ж�����
		|1<<15//����������15�ϵ��ж�����
		;//!END
	
	EXTI->EMR |= 0//!(EXTI_EMR)
		//|1<<14//����������14�ϵ��¼�����
		//|1<<15//����������15�ϵ��¼�����
		;//!END
	
	EXTI->FTSR |= 0//!(EXTI_RTSR)�����أ�(EXTI->FTSR)�½���
		|1<<14//����������14�ϵ������ش���
		|1<<15//����������15�ϵ������ش���
		;//!END
	dprintf("OK!");
}

u16 TMLSpeed=0;
u16 TMRSpeed=0;
void EXTI15_10_IRQHandler(void)//�ⲿ�ж�����ʹ��delayus��������¿����ж���������
{
	if(EXTI->PR&1<<15)//�ҵ��������
	{
		TMRSpeed++;
		EXTI->PR |= 1<<15;
	}
	if(EXTI->PR&1<<14)//����������
	{
		TMLSpeed++;
		EXTI->PR |= 1<<14;
	}
}

#endif
