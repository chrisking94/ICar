#include "Common.h"

void common_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;       //NVIC��ʼ���ṹ������
    //ȫ�ֹ���
    //!�õ�Remapʱ�����ȴ�AFIOʱ��
    RCC->APB1ENR  = 0x00220005;//ʹ��С�����õ�������ʱ�ӣ�RCC_APB1ENR�������ο��ֲᣩ
    RCC->APB2ENR  = 0x0000560D;//ʹ�ܶ˿ڸ���ʱ��(AFIOEN=1)��ʹ��PA,PB��������ʹ�õ������衣��RCC_APB2ENR��;
    AFIO->MAPR  = 0x04000000;//�رյ��Թ��ܣ�SWJ_CFG//�رյ��Թ��ܣ���AFIO_MAPR��

    //GPIOͳһ���ã��Ĵ�������GPIOx_CRL)
    //              76543210��
    GPIOA->CRL  = 0x00004B00;
    //              FEDCBA98��
    GPIOA->CRH  = 0x00000000;
    //              76543210��
    GPIOB->CRL  = 0xFF007000;
    //              FEDCBA98��
    GPIOB->CRH  = 0x00000000;

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
    RCC->APB2ENR |= 0x00004000;//ʹ��USART1ʱ��
    //              FEDCBA98
    GPIOA->CRH &= 0xFFFFF00F;//���֮ǰ����
    GPIOA->CRH |= 0x000004B0;//����USART1��Rx=PA10��Tx=PA9��
#endif

    //------------Timer-------------//
		//TIM1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    //TIM2->CR2 = 0;
    //TIM2->SMCR = 0;
    TIM2->DIER = 0x0001;
    TIM2->CNT = 0x0000;//��������ֵ��1ms)
    TIM2->PSC = 36000 - 1;//Ԥ��Ƶ��������ʱ��Ƶ��CK_CNT=fck_psc/(PSC+1)
    TIM2->EGR = 0x0041;
    TIM2->ARR = 2000-1;//AutoReloadRegister�����ü��������С��������������0~ARR��ARRʱ����������жϣ�

    TIM2->CR1 = 0x0284;//��ARRԤװ�ػ����� 0x0205

    TIM2->SR &= ~1;//��������ж�λ
		
		SystemCoreClock=1;

    ledp=1;					//��ʼ������Ϊ0
}


void delay(u32 t)
{
    u16 i;
    for (;t>0;t--)
        for (i=125;i>0;i--);
}

#define T2DE(); TIM2->CR1 &= ~1;//�ر�T2
#define T2EN(); TIM2->CR1 |=  1;//ʹ��T2

void delayus(u16 us)
{
	T2EN();
	TIM2->PSC = 18-1;//���ڸպ�Ϊ 1us ��72MHz/(CKD)/18  CKD=4��TIMx_CR1��
	TIM2->ARR = us;
	while(!(TIM2->SR & 1));//�ȴ����
	TIM2->SR &= ~1;//��������ж�λ
	T2DE();
}

/*
void delayms(u16 ms)
{
	T2EN();
	TIM2->PSC = 18000-1;//���ڸպ�Ϊ 1us ��72MHz/(CKD)/18000  CKD=4��TIMx_CR1��
	TIM2->ARR = ms;
	while(!(TIM2->SR & 1));//�ȴ����
	TIM2->SR &= ~1;//��������ж�λ
	T2DE();
}
*/

void delayms(u16 ms)//Systick
{
	SysTick->LOAD=(u32)ms*72000;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0;           //��ռ�����
	SysTick->CTRL |=  1;          //��ʼ����
	while(!(SysTick->CTRL&(1<<16)));//�ȴ�������0��CountFlag������0ʱ��λΪ1
	SysTick->CTRL &= ~1;       //�رռ�����
	SysTick->VAL =0;       //��ռ�����	
}
/*********************************************
**********************************************
                    �ж�
**********************************************
*********************************************/

//��ʱ��
void TIM2_IRQHandler(void)
{
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}
