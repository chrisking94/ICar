#ifndef _SG90_H_
#define _SG90_H_

#include "common.h"

#define T2CE(); TIM2->CR1 &= ~1;//�ر�T2
#define T2EN(); TIM2->CR1 |=  1;//ʹ��T2���ڵ�����ģʽ�£������������¼�ʱ��CEN���Զ����!
#define T2CUIF(); TIM2->SR &= ~1;//(Clear Update Interrupt Flag)��������ж�λ

#define SEC PAout(0) //Steering Engine Control

void SG90_init(void);
void SG90_turn(uchar angle);//161

#endif
