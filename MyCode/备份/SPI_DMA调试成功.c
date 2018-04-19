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
		|0<<1//CPOL.空闲时SCK低电平
		|1<<0//CPOA.数据采样在第2个时钟边缘开始
		|7<<3//BR[5:3].波特率控制BaudRate=fPCLK/2^(BR+1),72MHz
		|0<<15//BIDIMOD.双向数据模式
		;//!(END)
	
	SPI1->CR2 |= 0
		//|1<<5//错误中断使能
		|1<<1//TXDMAEN.启动发送缓冲区DMA
		;//!END
	
	SPI1->CR1 |= 1<<6;//使能spi1
	
	
	//SPI2配置
	SPI2->CR1 = 0//!(SPI_CR1)
		|0<<2//MSTR.配置为从设备
		|0<<8//SSI.SSM为1时才有效
		|0<<9//SSM.禁止软件从设备管理，(释放NSS引脚)
		|0<<10//RXONLY.全双工
		|0<<11//DDF.(数据帧格式)8bit格式,(16bit格式=1)
		|0<<7//LSBFIRST.先发送MSB
		|0<<1//CPOL.空闲时SCK低电平
		|1<<0//CPOA.数据采样送第2个时钟边缘开始
		|6<<3//BR[5:3].波特率控制BaudRate=fPCLK/2^(BR+1),36MHz
		;//!(END)
	
	SPI2->CR2 |= 0
		//|1<<5//错误中断使能
		|1<<6//接收缓冲区非空中断使能
		//|1<<0//RxDMA
		//|1<<1//TXDMAEN.启动发送缓冲区DMA
		;//!END
	
	SPI2->CR1 |= 1<<6;//使能spi2
	//DMA配置
	//DMA1_Channel3->CCR = 0B0010000010011011;
}

#define SPI_DDF_8SET(); SPI1->CR1 &= ~(1<<11);
#define SPI_DDF_16SET(); SPI1->CR1 |= 1<<11;

void SPI_write8(u8 d8)
{
	SPI1->CR1 &= ~( 1<<15 ) ;  //双向数据模式使能
	SPI1->CR1 &= ~( 1<<10 ) ;  //配置为只发送模式，与全双工模式配置相同
	//SPI_DDF_8SET();
	DMA1_Channel3->CNDTR &= 0x0000   ;          //传输数量寄存器清零
	DMA1_Channel3->CNDTR = buffersize ;         //传输数量设置为
	DMA1->IFCR = 0xF00 ;
	//if(SPI1->DR);//必须先读取接收缓冲器，否则会产生溢出错误
	while((SPI1->SR& 0x02) == 0);//等待发送缓冲为空
	DMA1_Channel3->CCR |= 1<<0;//开启DMA通道3
	while( ( DMA1->ISR & 0x200 ) == 0 ) beep(200,200);		 //等待DMA传输完成

	while( ( SPI1->SR & 0x02 ) == 0 );   		 //等待TXE为1

	while( ( SPI1->SR & 0x80 ) == 1 );  		 //等待忙标志为0

	DMA1_Channel3->CCR &= ~( 1 << 0 ) ;          //关闭DMA通道3
	
	//delayus(3);
}

void SPI_write16(u16 d16)
{
	SPI_DDF_16SET();
	
	SPI1->DR = d16;
	
	delayus(3);
}

u8 SPI_read8(void)
{
	while((SPI2->SR&1<<0) == 1); //等待接收缓冲非空置位
	{
		beep(100,200);
	}
	return SPI2->DR;
}

void SPI1_IRQHandler()
{
	printStr("SPI1 Error!\nErrorCode=",USART2);
	printNum(SPI1->SR,USART2);
	printStr("\n",USART2);
}

uchar rxb[8];
uchar i=0;
void SPI2_IRQHandler()
{
	if (SPI2->SR&1<<0)
	{
		rxb[i++]=SPI2->DR;
		if (i==8)
		{
			for (i=0;i<8;i++)
			{
				printChar(rxb[i],USART2);
			}
			i=0;
			printStr("\n",USART2);
		}	
	}
}
