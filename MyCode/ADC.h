#ifndef ADC_H_
#define ADC_H_

#include "Common.h"

#define ADC_DMA_BUFFER_LENGTH 6

void ADC_init(void);
u16 ADC_getVal(u8 CHx);//����ģʽ��ֵ

void ADC_DMA_start(void);//����DMA
u16 ADC_DMA_getVal(u8 CHx);//DMAģʽ��ֵ

#endif
