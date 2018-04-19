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
		|0<<1//CPOL.����ʱSCK�͵�ƽ
		|1<<0//CPOA.���ݲ����ڵ�2��ʱ�ӱ�Ե��ʼ
		|7<<3//BR[5:3].�����ʿ���BaudRate=fPCLK/2^(BR+1),72MHz
		|0<<15//BIDIMOD.˫������ģʽ
		;//!(END)
	
	SPI1->CR2 |= 0
		//|1<<5//�����ж�ʹ��
		|1<<1//TXDMAEN.�������ͻ�����DMA
		;//!END
	
	SPI1->CR1 |= 1<<6;//ʹ��spi1
	
	
	//SPI2����
	SPI2->CR1 = 0//!(SPI_CR1)
		|0<<2//MSTR.����Ϊ���豸
		|0<<8//SSI.SSMΪ1ʱ����Ч
		|0<<9//SSM.��ֹ������豸����(�ͷ�NSS����)
		|0<<10//RXONLY.ȫ˫��
		|0<<11//DDF.(����֡��ʽ)8bit��ʽ,(16bit��ʽ=1)
		|0<<7//LSBFIRST.�ȷ���MSB
		|0<<1//CPOL.����ʱSCK�͵�ƽ
		|1<<0//CPOA.���ݲ����͵�2��ʱ�ӱ�Ե��ʼ
		|6<<3//BR[5:3].�����ʿ���BaudRate=fPCLK/2^(BR+1),36MHz
		;//!(END)
	
	SPI2->CR2 |= 0
		//|1<<5//�����ж�ʹ��
		|1<<6//���ջ������ǿ��ж�ʹ��
		//|1<<0//RxDMA
		//|1<<1//TXDMAEN.�������ͻ�����DMA
		;//!END
	
	SPI2->CR1 |= 1<<6;//ʹ��spi2
	//DMA����
	//DMA1_Channel3->CCR = 0B0010000010011011;
}

#define SPI_DDF_8SET(); SPI1->CR1 &= ~(1<<11);
#define SPI_DDF_16SET(); SPI1->CR1 |= 1<<11;

void SPI_write8(u8 d8)
{
	SPI1->CR1 &= ~( 1<<15 ) ;  //˫������ģʽʹ��
	SPI1->CR1 &= ~( 1<<10 ) ;  //����Ϊֻ����ģʽ����ȫ˫��ģʽ������ͬ
	//SPI_DDF_8SET();
	DMA1_Channel3->CNDTR &= 0x0000   ;          //���������Ĵ�������
	DMA1_Channel3->CNDTR = buffersize ;         //������������Ϊ
	DMA1->IFCR = 0xF00 ;
	//if(SPI1->DR);//�����ȶ�ȡ���ջ����������������������
	while((SPI1->SR& 0x02) == 0);//�ȴ����ͻ���Ϊ��
	DMA1_Channel3->CCR |= 1<<0;//����DMAͨ��3
	while( ( DMA1->ISR & 0x200 ) == 0 ) beep(200,200);		 //�ȴ�DMA�������

	while( ( SPI1->SR & 0x02 ) == 0 );   		 //�ȴ�TXEΪ1

	while( ( SPI1->SR & 0x80 ) == 1 );  		 //�ȴ�æ��־Ϊ0

	DMA1_Channel3->CCR &= ~( 1 << 0 ) ;          //�ر�DMAͨ��3
	
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
	while((SPI2->SR&1<<0) == 1); //�ȴ����ջ���ǿ���λ
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
