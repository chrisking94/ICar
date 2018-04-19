#include "SPI.h"

void SPI_init(void)
{
	GPIOA->ODR |=7<<5;//PA567上拉
	//SPI1配置
	SPI1->CR1 = 0//!(SPI_CR1)
		|1<<2//MSTR.配置为主设备
		|1<<8//SSI.SSM为1时才有效,要保持主机模式,必须NSS 接到高电平信号
		|1<<9//SSM:0.软件从设备管理(可释放SPI_NSS所在引脚)
		|0<<10//RXONLY.全双工
		|0<<11//DDF.(数据帧格式)8bit格式,(16bit格式=1)
		|0<<7//LSBFIRST.先发送MSB
		|1<<1//CPOL.空闲时SCK为(CPOL)电平
		|1<<0//CPOA.数据采样在第2个时钟边缘开始
		|0<<3//BR[5:3].波特率控制BaudRate=fPCLK/2^(BR+1),72MHz
		|0<<15//BIDIMOD.双向数据模式
		;//!(END)
	
	SPI1->CR2 |= 0//!(SPI_CR2)
		//|1<<7
		//|1<<5//错误中断使能
		|1<<1//TXDMAEN.启动发送缓冲区DMA
		;//!END
	
	SPI1->CR1 |= 1<<6;//使能spi1
	
	#if DEBUG_SPI==true
	//SPI2配置
	SPI2->CR1 = 0//!(SPI_CR1)
		|0<<2//MSTR.配置为从设备
		|0<<8//SSI.SSM为1时才有效
		|0<<9//SSM.禁止软件从设备管理，(释放NSS引脚)
		|0<<10//RXONLY.全双工
		|0<<11//DDF.(数据帧格式)8bit格式,(16bit格式=1)
		|0<<7//LSBFIRST.先发送MSB
		|1<<1//CPOL.空闲时SCK为(CPOL)电平
		|1<<0//CPOA.数据采样送第2个时钟边缘开始
		|0<<3//BR[5:3].波特率控制BaudRate=fPCLK/2^(BR+1),36MHz
		;//!(END)
	
	SPI2->CR2 |= 0
		//|1<<5//错误中断使能
		|1<<6//接收缓冲区非空中断使能
		//|1<<0//RxDMA
		//|1<<1//TXDMAEN.启动发送缓冲区DMA
		;//!END
	
	SPI2->CR1 |= 1<<6;//使能spi2
	#endif
}

bool SPI_isBusy(void)
{
	if((DMA1->ISR & 0x200) == 0) return true;//DMA是否传输完成
	if(( SPI1->SR & 0x02 ) == 0) return true;//等待TXE为1
	if(( SPI1->SR & 0x80 ) == 1) return true;//等待忙标志为0
	
	return false;
}

void SPI_DMAWrite(u16 Count)
{
	SPI_DDF_8SET();
	DMA1->IFCR = 0xF00 ; //清除通道3的标志位
	DMACH_SPI1->CNDTR = Count;//16bit寄存器
	while((SPI1->SR & (1<<1)) == 0);//等待发送缓冲为空
	DMACH_SPI1->CCR |= 1<<0;//开启DMA通道3	 
}

void SPI_setDMADataSize(u32 SPIDataSize)
{
	if(SPIDataSize==SPI_DATASIZE_8BIT)
	{
		DMA_setDataSize(DMACH_SPI1,DMA_SIZE_8_BIT);
		SPI_DDF_8SET();
	}
	else
	{
		DMA_setDataSize(DMACH_SPI1,DMA_SIZE_16_BIT);
		SPI_DDF_16SET();
	}
}

void SPI_setDMAMemeryAddr(vu8* Buffer,u32 BufferLen)
{
	DMA_setMemeryAddr(DMACH_SPI1,(u32)Buffer,BufferLen);
}

void SPI_write8(u8 d8)
{
	//SPI_DDF_8SET();
	while(SPI1->SR & (1<<7));//等待忙标志为0
	SPI1->DR = (u16)d8;
	while(!(SPI1->SR & (1<<0)));//等待接收缓存器为非空
	SPI1->DR;//读取
}

/*
void SPI_write16(u16 d16)
{
	SPI_DDF_16SET();
	while(( SPI1->SR & 0x80 ) == 1);//等待忙标志为0
	SPI1->DR = d16;
	delayus(50);
}
*/

#ifdef DEBUG_SPI
void SPI2_IRQHandler()
{
	if (SPI2->SR&1<<0)
	{
		//printChar(SPI2->DR,USART2);
	}
}

void SPI1_IRQHandler()
{
	//printStr("SPI1,errorCode=",USART2);
	//printNum(SPI1->SR,USART2);
	//printStr("\n",USART2);
}
#endif
