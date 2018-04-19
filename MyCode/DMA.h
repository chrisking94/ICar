#ifndef _DMA_H_
#define _DMA_H_

#include "common.h"

#define DMA_SIZE_8_BIT  0
#define DMA_SIZE_16_BIT 1
#define DMA_SIZE_32_BIT 2

void DMA_init(void);
void DMA_Restart(DMA_Channel_TypeDef* CHx,u16 NumberOfDataToTransfer);//启动也可以使用这个函数
void DMA_setMemeryAddr(DMA_Channel_TypeDef* DMA_CHx,u32 MemeryAddr,u32 BufferLen);
void DMA_setDataSize(DMA_Channel_TypeDef* DMA_CHx,u32 DataSize);

#endif
