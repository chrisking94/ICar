#include "SPI.h"

void SPI_init(void)
{
	//SPI1配置
	/*CR1*/
	SPI1->CR1 = 0;//清除SPE，SPI Enable=False
	//SPI1->CR1 = 0B0100000000011100;
	/*CR2*/
	//SPI1->CR2 = 0B0000000000100010;
	
	//SPI1->CR1 |= 0B0000000001000000;//置位SPE，使能SPI
	//DMA配置
	//DMA1_Channel3->CCR = 0B0010000010011011;
}
