#include "Common.h"

void common_init(void)
{
    //全局共用
	
	//++++++++++++++++++++RCC时钟配置
    //!用到Remap时必须先打开AFIO时钟
    RCC->APB1ENR  = //!（RCC_APB1ENR，STM32中文参考手册）
		 1<<0//TIM2
		|1<<1//TIM3,IR1838
		|1<<2//TIM4，PWM
		|1<<17//USART2
		|1<<21//I2C1
		;//!END
	
	RCC->APB2ENR  = //!（RCC_APB2ENR）;
		 1<<0//AFIO
		|1<<2//GPIOA
		|1<<3//GPIOB
		|1<<4//GPIOC
		|1<<9//ADC1
		|1<<12//SPI1
		;//!END
		
	AFIO->MAPR    = 4<<24;//!关闭调试功能（SWJ_CFG[2:0]=100B）（AFIO_MAPR）

    //++++++++++++++++++++GPIO配置（寄存器）（GPIOx_CRL)
	//--------------------------------------------------------
    //PAL 0~7       76543210
    GPIOA->CRL  = 0x00004B07;
	//--------------------------------------------------------
    //PAH 8~15      FEDCBA98
    GPIOA->CRH  = 0x07700003;
	//--------------------------------------------------------PB4,5电机，通用开漏	PB0,1模拟输入
    //PBL 0~7       76543210
    GPIOB->CRL  = 0xFF334000;
	//--------------------------------------------------------PB8,9电机，复用开漏	PB14,15浮空输入
    //PBH 8~15      FEDCBA98
    GPIOB->CRH  = 0x440047BB;
	//--------------------------------------------------------
	//PCL 0~7       76543210
	GPIOC->CRL  = 0x00000000;
	//--------------------------------------------------------

    /*IO配置对照，4bit一个引脚(y)
    	高2位（CNFy）：
    输入  00=模拟，01=浮空，10=上/下拉，11=保留
    输出        [推挽] [开漏]
           [通用]  00     01
    	   [复用]  10     11

    	低2位（MODEy）：
    	输入  00
    	输出  01=10MHz，10=2MHz，11=50MHz
    */

#ifdef DEBUG
    //USART1时钟及IO口配置
    RCC->APB2ENR |= 1<<14;//!使能USART1时钟
    //              FEDCBA98
    GPIOA->CRH &= 0xFFFFF00F;//!清除之前配置
    GPIOA->CRH |= 0x000004B0;//!配置USART1（Rx=PA10，Tx=PA9）            
#endif

    ledp=1;					//!初始化设置为0
}

void delayus(u16 us)
{
	if(us==0) return;
	
    SysTick->LOAD=(u32)us*36;//时间加载，倒数至0时，将被重装载的值(24bit)
    SysTick->VAL =0;           //清空计数器
    SysTick->CTRL |=  5;          //开始倒数，内部36MHz时钟频率
    while (!(SysTick->CTRL&(1<<16)));//等待倒数至0，CountFlag，数到0时该位为1
    SysTick->CTRL &= ~1;       //关闭计数器
}

void delayms(u16 ms)//可以使用u32
{
	if(ms==0) return;
    SysTick->VAL =0;//清空计数器
    SysTick->CTRL |=  5;//开启计数器
    while (ms--)
    {
        SysTick->LOAD=36000;//时间加载，倒数至0时，将被重装载的值(24bit)，内部时钟36MHz，36000/36MHz=1ms
        while (!(SysTick->CTRL&(1<<16)));//等待倒数至0，CountFlag，数到0时该位为1

    }
    SysTick->CTRL &= ~1;       //关闭计数器
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
	SysTick->LOAD=0x00FFFFFF;//时间加载，倒数至0时，将被重装载的值(24bit)
    SysTick->VAL =0;           //清空计数器
    SysTick->CTRL |=  5;          //开始倒数，内部36MHz时钟频率 
}

u32 readTimingms(void)
{
	u32 ms;
	SysTick->CTRL &= ~1;       //关闭计数器
	ms=(SysTick->LOAD-SysTick->VAL)/72000;
	return ms;
}
