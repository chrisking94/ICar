#include "SG90.h"
#include "DisplaySystem.h"

#ifdef COMPILE_SG90_

void SG90_init(void)
{
	dprintf("\nSG90...");
	//############################ TIM2 ######################################
	//!注意：配置IO（复用推挽）
	TIM2->ARR = 4000;//!自动重装值，4000*5us=20ms
	TIM2->PSC = 72*5-1;//!预分频值，周期为5us
	
	TIM2->CCMR1 |= 0//!(TIMx_CCMR1)，捕获/比较模式寄存器
		|6<<4//OC1M[2:0],输出比较3使能，CNT>CCR时为无效电平，否则为有效
		|0<<3//禁止OC1预装载
		;//!END
	
	TIM2->CCER |= 0//!(TIMx_CCER)
		|1<<1//OC1引脚输出低电平有效
		|1<<0//OC1输出使能
		;//!END
	
	TIM2->CR1 |= 0//!（TIMx_CR1）
		|1<<7//允许自动重装
		|1<<4//向下计数
		|1<<7//ARPE:auto reload preload enable，自动重装预装允许
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
	
	TIM2->CCR1 = TIM2->ARR - SG90_Offset -angle*(u16)2;//!设置捕获/比较1的值
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
