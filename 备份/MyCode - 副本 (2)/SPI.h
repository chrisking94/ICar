#ifndef SPI_H
#define SPI_H

#include "Common.h"
#include "usart.h"
#include "DMA.h"

void SPI_init(void);
void SPI_write8(u8 d8);
void SPI_write16(u16 d16);
u8 SPI_read8(void);

#endif
