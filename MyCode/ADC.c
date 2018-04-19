#include "ADC.h"

#ifdef DEBUG
	#include "usart.h"
#endif

//ʹ��ADC1ͨ��1��8��9   PA1,PB0,PB1

void ADC_init(void)
{
	//adc�ϵ磬ִ��У׼��У׼ǰadon=0�����ر�adc����2��adc���ڣ�
	//�ȴ�Tstabʱ��
	//14��ʱ�����ں� EOC ����λ��DR�д���ת�����
	//adcת��ʱ��=����ʱ�䣨����1.5���ڣ�+12.5��adcʱ������=14���ڣ���ʱ��Ƶ��Ϊ14MHz����Ϊ1us��
	
	RCC->APB2RSTR |=   1<<9;//!(RCC_APB2RSTR)��λADC1
	RCC->APB2RSTR &= ~(1<<9);//!ADC1��λ����
	
	RCC->CFGR &= ~(3<<14);//!(RCC_CFGR)���ADCԤ��Ƶ����
	RCC->CFGR |=   2<<14;//!�趨��Ƶ����
	
	ADC1->CR1 = 0//!(ADC_CR1)
		|0<<16//��������ģʽ
		|1<<8//ɨ��ģʽ
		;//!END
	
	ADC1->CR2 = 0//!(ADC_CR2)
		|1<<1//����ת��
		|0<<11//���ö���ģʽΪ�Ҷ���
		|7<<17//�趨����ͨ�������¼�Ϊ���������SWSTART
		|1<<8//����DMA
		|1<<20//ʹ���ⲿ�¼�����ת�� *
		;//!END
	
	ADC1->SQR1 = 2<<20;//!(ADC_SQR1)������������Ϊ3��һ�����кſ��Զ�Ӧ����һ��ͨ����

	ADC1 -> SQR3 = 0
		|1<<0//!1��ͨ���������е�һ��ת��
		|8<<5//!8��ͨ���������еڶ���ת��
		|9<<10//!9��ͨ���������е�����ת��
		;
	
	//!��߲���ʱ�������߲�������
	ADC1->SMPR2 = 0//!(ADC_SMPR2)
		|7<<24//ͨ��8����ʱ��(7.5s=1)239.5����
		|7<<27//ͨ��9����ʱ��(7.5s=1)239.5����
		|7<<3//ͨ��1����ʱ��(7.5s=1)239.5����
		;//!END
	
	ADC1->CR2 |= 1<<0;//����ADC1
	ADC1->CR2 |= 1<<3;//ʹ�ܸ�λУ׼����Ӳ������ 
	while((ADC1->CR2)&(1<<3));//�ȴ�У׼����
	ADC1->CR2 |= 1<<2;//��ʼADУ׼����Ӳ������ 
	while((ADC1->CR2)&(1<<2));//�ȴ�У׼����
	
	ADC_DMA_start();//DMAģʽ
}

u16 ADC_getVal(u8 CHx)
{
	ADC1 -> SQR3 &= 0xFFFFFFE0;          //(ADC_SQR3)���ͨ��ѡ��
    ADC1 -> SQR3 |= CHx;                //ѡ��ͨ��
    ADC1 -> CR2  |= 1<<22;             //����ADת��
    while(!(ADC1 -> SR & 1<<1));           //�ȴ�ת������
	
    return ADC1->DR;
}

void ADC_DMA_start(void)
{
	ADC1 -> CR2  |= 1<<22;             //����ADת��
}

u16 ADC_DataBuffer[ADC_DMA_BUFFER_LENGTH];
u16 ADC_DMA_getVal(u8 CHx)
{
	u8 i;
	u16 avg;
	for(i=CHx;i<ADC_DMA_BUFFER_LENGTH;i+=3)
	{
		avg+=ADC_DataBuffer[i];
	}
	avg/=ADC_DMA_BUFFER_LENGTH/3;//���ֵ
	
	return avg;
}
