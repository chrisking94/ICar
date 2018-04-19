#include "NVIC.h"

void NVIC_PriorityGroupConfig(void)
{
	u32 tmp,tmp1;
	
	tmp1=(~MyNVICPriorityGroup)&0x07;//取后三位
	tmp1<<=8;
	tmp=SCB->AIRCR;//读取先前的设置
	//!SCB->AIRCR：16位寄存器
	tmp&=0x0000F8FF;//清空先前分组
	//!SCB->AIRCR 的修改需要通过在高16 位写入0X05FA 这个密钥才能修改的
	tmp|=0x05FA0000;//写入钥匙
	tmp|=tmp1;
	SCB->AIRCR=tmp;
}

//!ICER[2]：中断除能寄存器（Interrupt Clear-Enable Registers），对应ISER[2]NVIC_EnableIRQ(Channel);
//!ISER[2]：中断使能寄存器（Interrupt Set-Enable Registers），对应ICER[2]
//!ISPR[2]：全称是：Interrupt Set-Pending Registers，是一个中断挂起控制寄存器组。每个位,NVIC_SetPendingIRQ(Channel);
//!ICPR[2]：全称是：Interrupt Clear-Pending Registers，是一个中断解挂控制寄存器组
//!IABR[2]：全称是：Active Bit Registers，是一个中断激活标志位寄存器组(ROR:read only register)
//!IPR[15](IP)：全称是：Interrupt Priority Registers，是一个中断优先级控制的寄存器组
//!IPR[0]的[31~24]，[23~16]，[15~8]，[7~0]分别对应中中断3~0，
//!依次类推，总共对应60 个外部中断。而每个可屏蔽中断占用的8bit 并没有全部使用，而是只
//!用了高4 位。这4 位，又分为抢占优先级和子优先级。抢占优先级在前，子优先级在后。而这
//!两个优先级各占几个位又要根据SCB->AIRCR 中中断分组的设置来决定。
//!STM32 的中断分组：STM32 将中断分为5 个组，组0~4。该分组的设
//!置是由SCB->AIRCR 寄存器的bit10~8 来定义的。具体的分配关系如下表所示：

//!组	AIRCR[10:8]	高4位[7:4]分配情况	抢占优先级/位		响应优先级/位
//!0		111			0:4					0				4
//!1		110			1:3					1				3
//!2		101			2:2					2				2
//!3		100			3:1					3				1
//!4		011			4:0					4				0

//!如果两个中断的响应优先级和响应优先级都是一样的话，则看哪个中断先发生就先执行。
//!高优先级的抢占优先级是可以打断正在进行的低抢占优先级中断的。而抢占优先级相同的
//!中断，高优先级的响应优先级不可以打断低响应优先级的中断。

void NVIC_new(IRQn_Type Channel,u8 PreemptionPriority,u8 SubPriority)
{
	u32 tmp;
	
	u8 IPRADDR = Channel/4;//得到组（每组4个）地址x
	u8 IPROFFSET = Channel%4;//得到组内偏移
	IPROFFSET=IPROFFSET*8+4;//得到偏移确切位置（8位一个Channel，但只使用高4bit）
	//!优先级设置共有4位，高位(共x位)为抢占优先级，低位（y位）为响应优先级，x y与SCB->AIRCR相关，参考前面说明
	tmp=PreemptionPriority<<(4-MyNVICPriorityGroup);//设置抢占优先级
	tmp|=SubPriority&(0x0f>>MyNVICPriorityGroup);//设置相应优先级
	tmp&=0x0f;//取低四位
	
	NVIC_EnableIRQ(Channel);
	//NVIC_DisableIRQ(Channel);
	
	//!IP：中断优先级控制的寄存器组（Interrupt Priority Registers）
	NVIC->IP[IPRADDR]|=tmp<<IPROFFSET;//设置响应优先级和抢断优先级 
}

void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

void NVIC_init(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);//NVIC_VectTab_RAM
	NVIC_PriorityGroupConfig();//设置分组

	NVIC_new(USART2_IRQn,1,3);//USART2中断
	//NVIC_new(I2C1_EV_IRQn,1,1);//I2C1事件
	//NVIC_new(I2C1_ER_IRQn,0,1);//I2C1错误
	//NVIC_new(EXTI15_10_IRQn,1,1);//外部中断10~15，测速器
	NVIC_new(TIM3_IRQn,1,3);//TIM3中断
	NVIC_new(EXTI3_IRQn,1,4);//红外遥控
}
