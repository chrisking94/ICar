#include "SPI.h"

void SPI_init(void)
{
	GPIOA->ODR |=7<<5;//PA567����
	//SPI1����
	SPI1->CR1 = 0//!(SPI_CR1)
		|1<<2//MSTR.����Ϊ���豸
		|1<<8//SSI.SSMΪ1ʱ����Ч,Ҫ��������ģʽ,����NSS �ӵ��ߵ�ƽ�ź�
		|1<<9//SSM:0.������豸����(���ͷ�SPI_NSS��������)
		|0<<10//RXONLY.ȫ˫��
		|0<<11//DDF.(����֡��ʽ)8bit��ʽ,(16bit��ʽ=1)
		|0<<7//LSBFIRST.�ȷ���MSB
		|1<<1//CPOL.����ʱSCKΪ(CPOL)��ƽ
		|1<<0//CPOA.���ݲ����ڵ�2��ʱ�ӱ�Ե��ʼ
		|0<<3//BR[5:3].�����ʿ���BaudRate=fPCLK/2^(BR+1),72MHz
		|0<<15//BIDIMOD.˫������ģʽ
		;//!(END)
	
	SPI1->CR2 |= 0//!(SPI_CR2)
		//|1<<7
		//|1<<5//�����ж�ʹ��
		|1<<1//TXDMAEN.�������ͻ�����DMA
		;//!END
	
	SPI1->CR1 |= 1<<6;//ʹ��spi1
	
	#if DEBUG_SPI==true
	//SPI2����
	SPI2->CR1 = 0//!(SPI_CR1)
		|0<<2//MSTR.����Ϊ���豸
		|0<<8//SSI.SSMΪ1ʱ����Ч
		|0<<9//SSM.��ֹ������豸����(�ͷ�NSS����)
		|0<<10//RXONLY.ȫ˫��
		|0<<11//DDF.(����֡��ʽ)8bit��ʽ,(16bit��ʽ=1)
		|0<<7//LSBFIRST.�ȷ���MSB
		|1<<1//CPOL.����ʱSCKΪ(CPOL)��ƽ
		|1<<0//CPOA.���ݲ����͵�2��ʱ�ӱ�Ե��ʼ
		|0<<3//BR[5:3].�����ʿ���BaudRate=fPCLK/2^(BR+1),36MHz
		;//!(END)
	
	SPI2->CR2 |= 0
		//|1<<5//�����ж�ʹ��
		|1<<6//���ջ������ǿ��ж�ʹ��
		//|1<<0//RxDMA
		//|1<<1//TXDMAEN.�������ͻ�����DMA
		;//!END
	
	SPI2->CR1 |= 1<<6;//ʹ��spi2
	#endif
}

bool SPI_isBusy(void)
{
	if((DMA1->ISR & 0x200) == 0) return true;//DMA�Ƿ������
	if(( SPI1->SR & 0x02 ) == 0) return true;//�ȴ�TXEΪ1
	if(( SPI1->SR & 0x80 ) == 1) return true;//�ȴ�æ��־Ϊ0
	
	return false;
}

void SPI_DMAWrite(u16 Count)
{
	SPI_DDF_8SET();
	DMA1->IFCR = 0xF00 ; //���ͨ��3�ı�־λ
	DMACH_SPI1->CNDTR = Count;//16bit�Ĵ���
	while((SPI1->SR & (1<<1)) == 0);//�ȴ����ͻ���Ϊ��
	DMACH_SPI1->CCR |= 1<<0;//����DMAͨ��3	 
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
	while(SPI1->SR & (1<<7));//�ȴ�æ��־Ϊ0
	SPI1->DR = (u16)d8;
	while(!(SPI1->SR & (1<<0)));//�ȴ����ջ�����Ϊ�ǿ�
	SPI1->DR;//��ȡ
}

/*
void SPI_write16(u16 d16)
{
	SPI_DDF_16SET();
	while(( SPI1->SR & 0x80 ) == 1);//�ȴ�æ��־Ϊ0
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
