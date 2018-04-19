#ifndef L289N_H_
#define L289N_H_

#include "Common.h"

void L298N_init(void);//初始化
void L298N_setLS(s16 speed);//设置左电机速度 
void L298N_setRS(s16 speed);//设置右电机速度 

#endif
