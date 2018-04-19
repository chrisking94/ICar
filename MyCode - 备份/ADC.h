#ifndef ADC_H_
#define ADC_H_

#include "Common.h"

#define ADC_DMA_BUFFER_LENGTH 6

void ADC_init(void);
u16 ADC_getVal(u8 CHx);//常规模式读值

void ADC_DMA_start(void);//触发DMA
u16 ADC_DMA_getVal(u8 CHx);//DMA模式读值

#endif
