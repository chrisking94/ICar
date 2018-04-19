#include "DMA.h"

vu8 tbuf[buffersize]="ABCDEFG";
vu8 rbuf[buffersize]={0};
void DMA_new(DMA_Channel_TypeDef* CHx,u16 Priority,bool IsReadFromMemery
			,u32 PeriphAddr,vu8* Buffer,u32 BufferSize)//Priority:0~3��ֵԽСԽ��
{
	CHx->CCR |= 0//!(DMA_CCRx)
		|IsReadFromMemery<<4//�Ӵ洢�������裩��
		|0<<5//��ִ��ѭ������
		|0<<6//��ִ�������ַ����ģʽ
		|1<<7//�洢������ģʽ
		|0<<8//PSIZE:�������ݿ��8bit
		|0<<10//MSIZE:�洢�����ݿ��8bit
		|(3-Priority)<<12//ͨ�����ȼ�
		|0<<14//�Ǵ洢�����洢��ģʽ
		;//!END
	
	CHx->CNDTR = BufferSize;//���ô�������
	
	CHx->CPAR = PeriphAddr;//���������ַ��ע��PSize
	CHx->CMAR = (u32)Buffer;
}
void DMA_init(void)
{
	DMA_new(DMA1_Channel3,3,true,SPI1_BASE+0x0C,tbuf,8);//0x0C��->DRƫ�Ƶ�ַ
}
