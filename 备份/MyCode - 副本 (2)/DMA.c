#include "DMA.h"

vu8 tbuf[buffersize]="ABCDEFG";
vu8 rbuf[buffersize]={0};
void DMA_new(DMA_Channel_TypeDef* CHx,u16 Priority,bool IsReadFromMemery
			,u32 PeriphAddr,vu8* Buffer,u32 BufferSize)//Priority:0~3，值越小越高
{
	CHx->CCR |= 0//!(DMA_CCRx)
		|IsReadFromMemery<<4//从存储器（外设）读
		|0<<5//不执行循环操作
		|0<<6//不执行外设地址增量模式
		|1<<7//存储器增量模式
		|0<<8//PSIZE:外设数据宽度8bit
		|0<<10//MSIZE:存储器数据宽度8bit
		|(3-Priority)<<12//通道优先级
		|0<<14//非存储器到存储器模式
		;//!END
	
	CHx->CNDTR = BufferSize;//设置传输数量
	
	CHx->CPAR = PeriphAddr;//设置外设地址，注意PSize
	CHx->CMAR = (u32)Buffer;
}
void DMA_init(void)
{
	DMA_new(DMA1_Channel3,3,true,SPI1_BASE+0x0C,tbuf,8);//0x0C：->DR偏移地址
}
