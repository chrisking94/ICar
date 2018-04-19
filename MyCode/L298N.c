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
	
	TIM4->ARR = L298N_MAX_OUTPUT;//!自动重装值
	TIM4->PSC = 0;//!预分频值
	
	/*
	TIM4->DIER |= //!（TIMx_DIER）
		 1<<0//允许更新中断
		|1<<6//允许触发中断
		|1<<3//允许捕获/比较3中断
		|1<<4//允许捕获/比较4中断
		;//!END
	*/
	
	TIM4->CCMR2 |= 0//!(TIMx_CCMR2)，捕获/比较模式寄存器
		|6<<4//OC3M[2:0],输出比较3使能
		//|1<<3//允许OC3预装载
		|6<<12//OC4M[2:0]
		//|1<<11//允许OC4预装载
		;//!END
	
	TIM4->CCER |= 0//!(TIMx_CCER)
		|0<<9//OC3引脚输出高电平有效
		|1<<8//OC3输出使能
		|0<<13//OC4引脚输出高电平有效
		|1<<12//OC4输出使能
		;//!END
	
	TIM4->CR1 |= 0//!（TIMx_CR1）
		|1<<7//允许自动重装
		|1<<0//使能计数器
		;//!END
	
	TIM4->CCR3 = 0;//!设置捕获/比较3的值
	TIM4->CCR4 = 0;//!设置捕获/比较4的值
}

//------------------------------------------------------------------
//函数名：void L298N_setLS(u32 speed)
//输入参数：speed(速度)
//返回参数：null
//说明：设置左电机速度
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
//函数名：L298N_setRS(u16 speed)
//输入参数：speed(速度)
//返回参数：null
//说明：设置右电机速度
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
	//differ=右速-左速
	s32 sp=basicSpeed;
	if(differ>0)
	{
		sp+=differ;
		if (sp>SPEED_MAX)//右速达到最大值
		{
			TIM4->CCR4=SPEED_MAX;
			sp-=SPEED_MAX;//左速追加减小量
		}
		else
		{
			TIM4->CCR4=sp;
			sp=0;//右速未达上限，左速追加减小量=0
		}
		sp=basicSpeed-differ-sp;//左速
		if(sp<0) sp=0;
		TIM4->CCR3=sp;
	}
	else
	{
		sp-=differ;
		if(sp>SPEED_MAX)//左速达到最大值
		{
			TIM4->CCR3=SPEED_MAX;
			sp-=SPEED_MAX;//右速追加减小量
		}
		else
		{
			TIM4->CCR3=sp;
			sp=0;
		}
		sp=basicSpeed+differ-sp;//右速
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
		case 0://前进
			TIM4->CCER &= ~(1<<9);//!(TIMx_CCER)，OC4引脚输出高电平有效
			L298N_LCtrl = 0;
			break;
		default://后退
			TIM4->CCER |= 1<<9;//!(TIMx_CCER)，OC4引脚输出高电平有效
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
			TIM4->CCER &= ~(1<<13);//!(TIMx_CCER)，OC4引脚输出高电平有效
			L298N_RCtrl = 0;
			break;
		default:
			TIM4->CCER |= 1<<13;//!(TIMx_CCER)，OC4引脚输出高电平有效
			L298N_RCtrl = 1;
	}
}

#endif
