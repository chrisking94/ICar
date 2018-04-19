#ifndef NVIC_H_  //嵌套向量中断控制器:Nested Vectored Interrupt Controller (NVIC)
#define NVIC_H_

#include "Common.h"

#define NVIC_VectTab_RAM             ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH           ((uint32_t)0x08000000)

#define MyNVICPriorityGroup 2 //0~4

void NVIC_init(void);

#endif
