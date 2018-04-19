#ifndef L289N_H_
#define L289N_H_

#include "Common.h"

	#ifdef COMPILE_L298N_

#define L298N_MAX_OUTPUT 2000

void L298N_init(void);//初始化
void L298N_setLS(s32 speed);//设置左电机速度 
void L298N_setRS(s32 speed);//设置右电机速度 

void L298N_setSpeed(s32 basicSpeed,s32 differ);//基础速度，实际差速为2*differ

#define L298N_DIR_FOREWARD 0
#define L298N_DIR_BACKWARD 1
void L298N_setLD(u8 dir);//设置左电机转动方向，0为前，非0为后
void L298N_setRD(u8 dir);//设置右电机转动方向

	#endif

#endif
