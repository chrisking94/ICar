#ifndef L289N_H_
#define L289N_H_

#include "Common.h"

	#ifdef COMPILE_L298N_

#define L298N_MAX_OUTPUT 2000

void L298N_init(void);//��ʼ��
void L298N_setLS(s32 speed);//���������ٶ� 
void L298N_setRS(s32 speed);//�����ҵ���ٶ� 

void L298N_setSpeed(s32 basicSpeed,s32 differ);//�����ٶȣ�ʵ�ʲ���Ϊ2*differ

#define L298N_DIR_FOREWARD 0
#define L298N_DIR_BACKWARD 1
void L298N_setLD(u8 dir);//��������ת������0Ϊǰ����0Ϊ��
void L298N_setRD(u8 dir);//�����ҵ��ת������

	#endif

#endif
