#ifndef	SPI_H
#define SPI_H

#include <Common.h> 

void SPISendByte(unsigned char ch);
unsigned char SPIreceiveByte();
unsigned char SPIsend_receiveByte(unsigned char ch);

#endif
