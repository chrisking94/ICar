#include "Common.h"

void common_init(void)
{
    //ȫ�ֹ���
	
	//++++++++++++++++++++RCCʱ������
    //!�õ�Remapʱ�����ȴ�AFIOʱ��
    RCC->APB1ENR  = //!��RCC_APB1ENR��STM32���Ĳο��ֲᣩ
		 1<<0//TIM2
		|1<<1//TIM3,IR1838
		|1<<2//TIM4��PWM
		|1<<17//USART2
		|1<<21//I2C1
		;//!END
	
	RCC->APB2ENR  = //!��RCC_APB2ENR��;
		 1<<0//AFIO
		|1<<2//GPIOA
		|1<<3//GPIOB
		|1<<4//GPIOC
		|1<<9//ADC1
		|1<<12//SPI1
		;//!END
		
	AFIO->MAPR    = 4<<24;//!�رյ��Թ��ܣ�SWJ_CFG[2:0]=100B����AFIO_MAPR��

    //++++++++++++++++++++GPIO���ã��Ĵ�������GPIOx_CRL)
	//--------------------------------------------------------
    //PAL 0~7       76543210
    GPIOA->CRL  = 0x00004B07;
	//--------------------------------------------------------
    //PAH 8~15      FEDCBA98
    GPIOA->CRH  = 0x07700003;
	//--------------------------------------------------------PB4,5�����ͨ�ÿ�©	PB0,1ģ������
    //PBL 0~7       76543210
    GPIOB->CRL  = 0xFF334000;
	//--------------------------------------------------------PB8,9��������ÿ�©	PB14,15��������
    //PBH 8~15      FEDCBA98
    GPIOB->CRH  = 0x440047BB;
	//--------------------------------------------------------
	//PCL 0~7       76543210
	GPIOC->CRL  = 0x00000000;
	//--------------------------------------------------------

    /*IO���ö��գ�4bitһ������(y)
    	��2λ��CNFy����
    ����  00=ģ�⣬01=���գ�10=��/������11=����
    ���        [����] [��©]
           [ͨ��]  00     01
    	   [����]  10     11

    	��2λ��MODEy����
    	����  00
    	���  01=10MHz��10=2MHz��11=50MHz
    */

#ifdef DEBUG
    //USART1ʱ�Ӽ�IO������
    RCC->APB2ENR |= 1<<14;//!ʹ��USART1ʱ��
    //              FEDCBA98
    GPIOA->CRH &= 0xFFFFF00F;//!���֮ǰ����
    GPIOA->CRH |= 0x000004B0;//!����USART1��Rx=PA10��Tx=PA9��            
#endif

    ledp=1;					//!��ʼ������Ϊ0
}

void delayus(u16 us)
{
	if(us==0) return;
	
    SysTick->LOAD=(u32)us*36;//ʱ����أ�������0ʱ��������װ�ص�ֵ(24bit)
    SysTick->VAL =0;           //��ռ�����
    SysTick->CTRL |=  5;          //��ʼ�������ڲ�36MHzʱ��Ƶ��
    while (!(SysTick->CTRL&(1<<16)));//�ȴ�������0��CountFlag������0ʱ��λΪ1
    SysTick->CTRL &= ~1;       //�رռ�����
}

void delayms(u16 ms)//����ʹ��u32
{
	if(ms==0) return;
    SysTick->VAL =0;//��ռ�����
    SysTick->CTRL |=  5;//����������
    while (ms--)
    {
        SysTick->LOAD=36000;//ʱ����أ�������0ʱ��������װ�ص�ֵ(24bit)���ڲ�ʱ��36MHz��36000/36MHz=1ms
        while (!(SysTick->CTRL&(1<<16)));//�ȴ�������0��CountFlag������0ʱ��λΪ1

    }
    SysTick->CTRL &= ~1;       //�رռ�����
}

void delays(u32 s)
{
	if(s==0) return;
    while (s--)
    {
        delayms(1000);
    }
}

void beep(u16 time ,u16 interval)
{
    for (;time>0;time--)
    {
        obeep=!obeep;
        delayus(interval);
    }
    obeep=1;
}

void startTiming(void)
{
	SysTick->LOAD=0x00FFFFFF;//ʱ����أ�������0ʱ��������װ�ص�ֵ(24bit)
    SysTick->VAL =0;           //��ռ�����
    SysTick->CTRL |=  5;          //��ʼ�������ڲ�36MHzʱ��Ƶ�� 
}

u32 readTimingms(void)
{
	u32 ms;
	SysTick->CTRL &= ~1;       //�رռ�����
	ms=(SysTick->LOAD-SysTick->VAL)/72000;
	return ms;
}
