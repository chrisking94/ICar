#include "ADC.h"

#ifdef DEBUG
	#include "usart.h"
#endif

//使用ADC1通道1、8、9   PA1,PB0,PB1

void ADC_init(void)
{
	//adc上电，执行校准（校准前adon=0，即关闭adc至少2个adc周期）
	//等待Tstab时常
	//14个时钟周期后 EOC 被置位，DR中存入转换结果
	//adc转换时间=采样时间（例：1.5周期）+12.5个adc时钟周期=14周期（若时钟频率为14MHz，则为1us）
	
	RCC->APB2RSTR |=   1<<9;//!(RCC_APB2RSTR)复位ADC1
	RCC->APB2RSTR &= ~(1<<9);//!ADC1复位结束
	
	RCC->CFGR &= ~(3<<14);//!(RCC_CFGR)清除ADC预分频因子
	RCC->CFGR |=   2<<14;//!设定分频因子
	
	ADC1->CR1 = 0//!(ADC_CR1)
		|0<<16//独立工作模式
		|1<<8//扫描模式
		;//!END
	
	ADC1->CR2 = 0//!(ADC_CR2)
		|1<<1//连续转换
		|0<<11//设置对齐模式为右对齐
		|7<<17//设定规则通道启动事件为软件启动：SWSTART
		|1<<8//启用DMA
		|1<<20//使用外部事件启动转换 *
		;//!END
	
	ADC1->SQR1 = 2<<20;//!(ADC_SQR1)规则序列数量为3（一个序列号可以对应任意一个通道）

	ADC1 -> SQR3 = 0
		|1<<0//!1号通道，序列中第一个转换
		|8<<5//!8号通道，序列中第二个转换
		|9<<10//!9号通道，序列中第三个转换
		;
	
	//!提高采样时间可以提高采样精度
	ADC1->SMPR2 = 0//!(ADC_SMPR2)
		|7<<24//通道8采样时间(7.5s=1)239.5周期
		|7<<27//通道9采样时间(7.5s=1)239.5周期
		|7<<3//通道1采样时间(7.5s=1)239.5周期
		;//!END
	
	ADC1->CR2 |= 1<<0;//启动ADC1
	ADC1->CR2 |= 1<<3;//使能复位校准，由硬件清零 
	while((ADC1->CR2)&(1<<3));//等待校准结束
	ADC1->CR2 |= 1<<2;//开始AD校准，由硬件清零 
	while((ADC1->CR2)&(1<<2));//等待校准结束
	
	ADC_DMA_start();//DMA模式
}

u16 ADC_getVal(u8 CHx)
{
	ADC1 -> SQR3 &= 0xFFFFFFE0;          //(ADC_SQR3)清除通道选择
    ADC1 -> SQR3 |= CHx;                //选择通道
    ADC1 -> CR2  |= 1<<22;             //开启AD转换
    while(!(ADC1 -> SR & 1<<1));           //等待转换结束
	
    return ADC1->DR;
}

void ADC_DMA_start(void)
{
	ADC1 -> CR2  |= 1<<22;             //开启AD转换
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
	avg/=ADC_DMA_BUFFER_LENGTH/3;//求均值
	
	return avg;
}
