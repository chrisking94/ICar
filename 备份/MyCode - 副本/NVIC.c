#include "NVIC.h"

void NVIC_PriorityGroupConfig(void)
{
	u32 tmp,tmp1;
	
	tmp1=(~MyNVICPriorityGroup)&0x07;//ȡ����λ
	tmp1<<=8;
	tmp=SCB->AIRCR;//��ȡ��ǰ������
	//!SCB->AIRCR��16λ�Ĵ���
	tmp&=0x0000F8FF;//�����ǰ����
	//!SCB->AIRCR ���޸���Ҫͨ���ڸ�16 λд��0X05FA �����Կ�����޸ĵ�
	tmp|=0x05FA0000;//д��Կ��
	tmp|=tmp1;
	SCB->AIRCR=tmp;
}

//!ICER[2]���жϳ��ܼĴ�����Interrupt Clear-Enable Registers������ӦISER[2]NVIC_EnableIRQ(Channel);
//!ISER[2]���ж�ʹ�ܼĴ�����Interrupt Set-Enable Registers������ӦICER[2]
//!ISPR[2]��ȫ���ǣ�Interrupt Set-Pending Registers����һ���жϹ�����ƼĴ����顣ÿ��λ,NVIC_SetPendingIRQ(Channel);
//!ICPR[2]��ȫ���ǣ�Interrupt Clear-Pending Registers����һ���жϽ�ҿ��ƼĴ�����
//!IABR[2]��ȫ���ǣ�Active Bit Registers����һ���жϼ����־λ�Ĵ�����(ROR:read only register)
//!IPR[15](IP)��ȫ���ǣ�Interrupt Priority Registers����һ���ж����ȼ����ƵļĴ�����
//!IPR[0]��[31~24]��[23~16]��[15~8]��[7~0]�ֱ��Ӧ���ж�3~0��
//!�������ƣ��ܹ���Ӧ60 ���ⲿ�жϡ���ÿ���������ж�ռ�õ�8bit ��û��ȫ��ʹ�ã�����ֻ
//!���˸�4 λ����4 λ���ַ�Ϊ��ռ���ȼ��������ȼ�����ռ���ȼ���ǰ�������ȼ��ں󡣶���
//!�������ȼ���ռ����λ��Ҫ����SCB->AIRCR ���жϷ����������������
//!STM32 ���жϷ��飺STM32 ���жϷ�Ϊ5 ���飬��0~4���÷������
//!������SCB->AIRCR �Ĵ�����bit10~8 ������ġ�����ķ����ϵ���±���ʾ��

//!��	AIRCR[10:8]	��4λ[7:4]�������	��ռ���ȼ�/λ		��Ӧ���ȼ�/λ
//!0		111			0:4					0				4
//!1		110			1:3					1				3
//!2		101			2:2					2				2
//!3		100			3:1					3				1
//!4		011			4:0					4				0

//!��������жϵ���Ӧ���ȼ�����Ӧ���ȼ�����һ���Ļ������ĸ��ж��ȷ�������ִ�С�
//!�����ȼ�����ռ���ȼ��ǿ��Դ�����ڽ��еĵ���ռ���ȼ��жϵġ�����ռ���ȼ���ͬ��
//!�жϣ������ȼ�����Ӧ���ȼ������Դ�ϵ���Ӧ���ȼ����жϡ�

void NVIC_new(IRQn_Type Channel,u8 PreemptionPriority,u8 SubPriority)
{
	u32 tmp;
	
	u8 IPRADDR = Channel/4;//�õ��飨ÿ��4������ַx
	u8 IPROFFSET = Channel%4;//�õ�����ƫ��
	IPROFFSET=IPROFFSET*8+4;//�õ�ƫ��ȷ��λ�ã�8λһ��Channel����ֻʹ�ø�4bit��
	//!���ȼ����ù���4λ����λ(��xλ)Ϊ��ռ���ȼ�����λ��yλ��Ϊ��Ӧ���ȼ���x y��SCB->AIRCR��أ��ο�ǰ��˵��
	tmp=PreemptionPriority<<(4-MyNVICPriorityGroup);//������ռ���ȼ�
	tmp|=SubPriority&(0x0f>>MyNVICPriorityGroup);//������Ӧ���ȼ�
	tmp&=0x0f;//ȡ����λ
	
	NVIC_EnableIRQ(Channel);
	//NVIC_DisableIRQ(Channel);
	
	//!IP���ж����ȼ����ƵļĴ����飨Interrupt Priority Registers��
	NVIC->IP[IPRADDR]|=tmp<<IPROFFSET;//������Ӧ���ȼ����������ȼ� 
}

void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

void NVIC_init(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);//NVIC_VectTab_RAM
	NVIC_PriorityGroupConfig();//���÷���

	NVIC_new(USART2_IRQn,1,3);//USART2�ж�
	//NVIC_new(I2C1_EV_IRQn,1,1);//I2C1�¼�
	//NVIC_new(I2C1_ER_IRQn,0,1);//I2C1����
	//NVIC_new(EXTI15_10_IRQn,1,1);//�ⲿ�ж�10~15��������
	NVIC_new(TIM3_IRQn,1,3);//TIM3�ж�
	NVIC_new(EXTI3_IRQn,1,4);//����ң��
}
