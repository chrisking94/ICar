#ifndef SPI_H
#define SPI_H

#include "Common.h"
#include "usart.h"
#include "DMA.h"

#define DEBUG_SPI false

#define DMACH_SPI1 DMA1_Channel3

#define SPI_DATASIZE_8BIT  0
#define SPI_DATASIZE_16BIT 1
#define SPI_DDF_8SET(); SPI1->CR1 &= ~(1<<11);//数据帧8bit模式
#define SPI_DDF_16SET(); SPI1->CR1 |= 1<<11;//数据帧16bit模式

void SPI_init(void);
void SPI_setDMAMemeryAddr(vu8* Buffer,u32 BufferLen);
void SPI_setDMADataSize(u32 SPIDataSize);
void SPI_DMAWrite(u16 Count);//使用前可能需要一次SPI_setDMAMemeryAddr();
void SPI_write8(u8 d8);
//void SPI_write16(u16 d16);//该函数暂时有问题
u8 SPI_read8(void);

#endif
