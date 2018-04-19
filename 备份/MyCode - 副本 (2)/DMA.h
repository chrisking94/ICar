#ifndef _DMA_H_
#define _DMA_H_

#include "common.h"

#define SPI1_DR_Addr ((u32)0x4001300C)
#define SPI2_DR_Addr ((u32)0x4000380C)

#define buffersize 8

void DMA_init(void);

#endif
