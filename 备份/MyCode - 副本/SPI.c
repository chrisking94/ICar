#include "SPI.h"

void SPI_init(void)
{
	//SPI1����
	/*CR1*/
	SPI1->CR1 = 0;//���SPE��SPI Enable=False
	//SPI1->CR1 = 0B0100000000011100;
	/*CR2*/
	//SPI1->CR2 = 0B0000000000100010;
	
	//SPI1->CR1 |= 0B0000000001000000;//��λSPE��ʹ��SPI
	//DMA����
	//DMA1_Channel3->CCR = 0B0010000010011011;
}
