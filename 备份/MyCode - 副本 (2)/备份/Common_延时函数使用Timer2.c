#include "Common.h"

void common_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;       //NVIC初始化结构体声明
    //全局共用
    //!用到Remap时必须先打开AFIO时钟
    RCC->APB1ENR  = 0x00220005;//使能小车所用到的外设时钟（RCC_APB1ENR，搜索参考手册）
    RCC->APB2ENR  = 0x0000560D;//使能端口复用时钟(AFIOEN=1)，使能PA,PB，及其他使用到的外设。（RCC_APB2ENR）;
    AFIO->MAPR  = 0x04000000;//关闭调试功能（SWJ_CFG//关闭调试功能）（AFIO_MAPR）

    //GPIO统一配置（寄存器）（GPIOx_CRL)
    //              76543210↓
    GPIOA->CRL  = 0x00004B00;
    //              FEDCBA98↓
    GPIOA->CRH  = 0x00000000;
    //              76543210↓
    GPIOB->CRL  = 0xFF007000;
    //              FEDCBA98↓
    GPIOB->CRH  = 0x00000000;

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
    RCC->APB2ENR |= 0x00004000;//使能USART1时钟
    //              FEDCBA98
    GPIOA->CRH &= 0xFFFFF00F;//清除之前配置
    GPIOA->CRH |= 0x000004B0;//配置USART1（Rx=PA10，Tx=PA9）
#endif

    //------------Timer-------------//
		//TIM1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    //TIM2->CR2 = 0;
    //TIM2->SMCR = 0;
    TIM2->DIER = 0x0001;
    TIM2->CNT = 0x0000;//计数器初值（1ms)
    TIM2->PSC = 36000 - 1;//预分频，计数器时钟频率CK_CNT=fck_psc/(PSC+1)
    TIM2->EGR = 0x0041;
    TIM2->ARR = 2000-1;//AutoReloadRegister，设置计数溢出大小，计数器计数从0~ARR，ARR时产生溢出（中断）

    TIM2->CR1 = 0x0284;//无ARR预装载缓冲器 0x0205

    TIM2->SR &= ~1;//清除更新中断位
		
		SystemCoreClock=1;

    ledp=1;					//初始化设置为0
}


void delay(u32 t)
{
    u16 i;
    for (;t>0;t--)
        for (i=125;i>0;i--);
}

#define T2DE(); TIM2->CR1 &= ~1;//关闭T2
#define T2EN(); TIM2->CR1 |=  1;//使能T2

void delayus(u16 us)
{
	T2EN();
	TIM2->PSC = 18-1;//周期刚好为 1us ，72MHz/(CKD)/18  CKD=4（TIMx_CR1）
	TIM2->ARR = us;
	while(!(TIM2->SR & 1));//等待溢出
	TIM2->SR &= ~1;//清除更新中断位
	T2DE();
}

/*
void delayms(u16 ms)
{
	T2EN();
	TIM2->PSC = 18000-1;//周期刚好为 1us ，72MHz/(CKD)/18000  CKD=4（TIMx_CR1）
	TIM2->ARR = ms;
	while(!(TIM2->SR & 1));//等待溢出
	TIM2->SR &= ~1;//清除更新中断位
	T2DE();
}
*/

void delayms(u16 ms)//Systick
{
	SysTick->LOAD=(u32)ms*72000;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0;           //清空计数器
	SysTick->CTRL |=  1;          //开始倒数
	while(!(SysTick->CTRL&(1<<16)));//等待倒数至0，CountFlag，数到0时该位为1
	SysTick->CTRL &= ~1;       //关闭计数器
	SysTick->VAL =0;       //清空计数器	
}
/*********************************************
**********************************************
                    中断
**********************************************
*********************************************/

//定时器
void TIM2_IRQHandler(void)
{
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}
