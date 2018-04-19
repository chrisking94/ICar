#include "L298N.h"

#ifdef COMPILE_L298N_

#ifdef DEBUG
	#include "usart.h"
#endif

#define L298N_LCtrl PBout(5)
#define L298N_RCtrl PBout(4)

#define SPEED_MAX 2000

u16 L298N_LOFFSET=0;
u16 L298N_ROFFSET=0;

void L298N_init(void)
{
	L298N_LCtrl = 0;
	L298N_RCtrl = 0;
	
	TIM4->ARR = L298N_MAX_OUTPUT;//!�Զ���װֵ
	TIM4->PSC = 0;//!Ԥ��Ƶֵ
	
	/*
	TIM4->DIER |= //!��TIMx_DIER��
		 1<<0//��������ж�
		|1<<6//�������ж�
		|1<<3//������/�Ƚ�3�ж�
		|1<<4//������/�Ƚ�4�ж�
		;//!END
	*/
	
	TIM4->CCMR2 |= 0//!(TIMx_CCMR2)������/�Ƚ�ģʽ�Ĵ���
		|6<<4//OC3M[2:0],����Ƚ�3ʹ��
		//|1<<3//����OC3Ԥװ��
		|6<<12//OC4M[2:0]
		//|1<<11//����OC4Ԥװ��
		;//!END
	
	TIM4->CCER |= 0//!(TIMx_CCER)
		|0<<9//OC3��������ߵ�ƽ��Ч
		|1<<8//OC3���ʹ��
		|0<<13//OC4��������ߵ�ƽ��Ч
		|1<<12//OC4���ʹ��
		;//!END
	
	TIM4->CR1 |= 0//!��TIMx_CR1��
		|1<<7//�����Զ���װ
		|1<<0//ʹ�ܼ�����
		;//!END
	
	TIM4->CCR3 = 0;//!���ò���/�Ƚ�3��ֵ
	TIM4->CCR4 = 0;//!���ò���/�Ƚ�4��ֵ
}

//------------------------------------------------------------------
//��������void L298N_setLS(u32 speed)
//���������speed(�ٶ�)
//���ز�����null
//˵�������������ٶ�
//------------------------------------------------------------------
void L298N_setLS(s32 speed)
{
	/*
	if(speed<0)
	{
		L298N_setLD(L298N_DIR_BACKWARD);
	}
	else
	{
		L298N_setLD(L298N_DIR_FOREWARD);
	}
	*/
    TIM4->CCR3=speed+L298N_LOFFSET;
}

//------------------------------------------------------------------
//��������L298N_setRS(u16 speed)
//���������speed(�ٶ�)
//���ز�����null
//˵���������ҵ���ٶ�
//------------------------------------------------------------------
void L298N_setRS(s32 speed)
{
	/*
	if(speed<0)
	{
		L298N_setRD(L298N_DIR_BACKWARD);
	}
	else
	{
		L298N_setRD(L298N_DIR_FOREWARD);
	}
	*/
    TIM4->CCR4=speed+L298N_ROFFSET;
}

void L298N_setSpeed(s32 basicSpeed,s32 differ)
{
	//differ=����-����
	s32 sp=basicSpeed;
	if(differ>0)
	{
		sp+=differ;
		if (sp>SPEED_MAX)//���ٴﵽ���ֵ
		{
			TIM4->CCR4=SPEED_MAX;
			sp-=SPEED_MAX;//����׷�Ӽ�С��
		}
		else
		{
			TIM4->CCR4=sp;
			sp=0;//����δ�����ޣ�����׷�Ӽ�С��=0
		}
		sp=basicSpeed-differ-sp;//����
		if(sp<0) sp=0;
		TIM4->CCR3=sp;
	}
	else
	{
		sp-=differ;
		if(sp>SPEED_MAX)//���ٴﵽ���ֵ
		{
			TIM4->CCR3=SPEED_MAX;
			sp-=SPEED_MAX;//����׷�Ӽ�С��
		}
		else
		{
			TIM4->CCR3=sp;
			sp=0;
		}
		sp=basicSpeed+differ-sp;//����
		if(sp<0) sp=0;
		TIM4->CCR4=sp;
	}
}

void L298N_setLD(u8 dir)
{
	static u8 lastDir;
	if(dir==lastDir) return;
	lastDir=dir;
	switch(dir)
	{
		case 0://ǰ��
			TIM4->CCER &= ~(1<<9);//!(TIMx_CCER)��OC4��������ߵ�ƽ��Ч
			L298N_LCtrl = 0;
			break;
		default://����
			TIM4->CCER |= 1<<9;//!(TIMx_CCER)��OC4��������ߵ�ƽ��Ч
			L298N_LCtrl = 1;
	}
}

void L298N_setRD(u8 dir)
{
	static u8 lastDir;
	if(dir==lastDir) return;
	lastDir=dir;
	switch(dir)
	{
		case 0:
			TIM4->CCER &= ~(1<<13);//!(TIMx_CCER)��OC4��������ߵ�ƽ��Ч
			L298N_RCtrl = 0;
			break;
		default:
			TIM4->CCER |= 1<<13;//!(TIMx_CCER)��OC4��������ߵ�ƽ��Ч
			L298N_RCtrl = 1;
	}
}

#endif
