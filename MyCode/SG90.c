#include "SG90.h"
#include "DisplaySystem.h"

#ifdef COMPILE_SG90_

void SG90_init(void)
{
	dprintf("\nSG90...");
	//############################ TIM2 ######################################
	//!ע�⣺����IO���������죩
	TIM2->ARR = 4000;//!�Զ���װֵ��4000*5us=20ms
	TIM2->PSC = 72*5-1;//!Ԥ��Ƶֵ������Ϊ5us
	
	TIM2->CCMR1 |= 0//!(TIMx_CCMR1)������/�Ƚ�ģʽ�Ĵ���
		|6<<4//OC1M[2:0],����Ƚ�3ʹ�ܣ�CNT>CCRʱΪ��Ч��ƽ������Ϊ��Ч
		|0<<3//��ֹOC1Ԥװ��
		;//!END
	
	TIM2->CCER |= 0//!(TIMx_CCER)
		|1<<1//OC1��������͵�ƽ��Ч
		|1<<0//OC1���ʹ��
		;//!END
	
	TIM2->CR1 |= 0//!��TIMx_CR1��
		|1<<7//�����Զ���װ
		|1<<4//���¼���
		|1<<7//ARPE:auto reload preload enable���Զ���װԤװ����
		;//!END
	SG90_turn(SG90_MID);
	dprintf("OK!");
}

u16 SG90_Offset = 134;
void SG90_turn(u16 angle)//1~161
{
	static u16 lastAngle;
	u16 dms;
	
	if(angle==0) return;
	
	TIM2->CCR1 = TIM2->ARR - SG90_Offset -angle*(u16)2;//!���ò���/�Ƚ�1��ֵ
	T2EN();
	
	if(lastAngle>angle)
	{
		dms=(lastAngle-angle)*8;
	}
	else
	{
		dms=(angle-lastAngle)*8;
	}
	
	lastAngle=angle;
	
	delayms(dms);
	T2CE();
}

#endif
