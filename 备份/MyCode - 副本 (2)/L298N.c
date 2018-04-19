#include "L298N.h"

#define L298N_LCtrl PBout(5)
#define L298N_RCtrl PBout(4)

void L298N_init(void)
{
	L298N_LCtrl = 0;
	L298N_RCtrl = 0;
	
	TIM4->ARR = 900;//!自动重装值
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
		|1<<3//允许OC3预装载
		|6<<12//OC4M[2:0]
		|1<<11//允许OC4预装载
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
	
	TIM4->CCR3 = 300;//!设置捕获/比较3的值
	TIM4->CCR4 = 300;//!设置捕获/比较4的值
}

//------------------------------------------------------------------
//函数名：void L298N_setLS(u32 speed)
//输入参数：speed(速度)
//返回参数：null
//说明：设置左电机速度
//------------------------------------------------------------------
void L298N_setLS(s16 speed)
{
	if(speed>=0)
	{
		TIM4->CCER &= ~(1<<9);//!(TIMx_CCER)，OC4引脚输出高电平有效
		L298N_LCtrl = 0;
	}
	else
	{
		TIM4->CCER |= 1<<9;//!(TIMx_CCER)，OC4引脚输出高电平有效
		L298N_LCtrl = 1;
		speed=-speed;
	}
	
    TIM4->CCR3=(s16)400+speed;
}

//------------------------------------------------------------------
//函数名：L298N_setRS(u16 speed)
//输入参数：speed(速度)
//返回参数：null
//说明：设置右电机速度
//------------------------------------------------------------------
void L298N_setRS(s16 speed)
{
	if(speed>=0)
	{
		TIM4->CCER &= ~(1<<13);//!(TIMx_CCER)，OC4引脚输出高电平有效
		L298N_RCtrl = 0;
	}
	else
	{
		TIM4->CCER |= 1<<13;//!(TIMx_CCER)，OC4引脚输出高电平有效
		L298N_RCtrl = 1;
		speed=-speed;
	}
	
    TIM4->CCR4=(s16)400+speed;
}
