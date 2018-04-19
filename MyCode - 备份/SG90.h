#ifndef _SG90_H_
#define _SG90_H_

#include "common.h"

	#ifdef COMPILE_SG90_

#define T2CE(); TIM2->CR1 &= ~1;//关闭T2
#define T2EN(); TIM2->CR1 |=  1;//使能T2，在单脉冲模式下，当发生更新事件时，CEN被自动清除!
#define T2CUIF(); TIM2->SR &= ~1;//(Clear Update Interrupt Flag)清除更新中断位

#define SEC PAout(0) //Steering Engine Control

void SG90_init(void);
void SG90_turn(u16 angle);//161

	#endif

#endif
