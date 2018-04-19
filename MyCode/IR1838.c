#include "ir1838.h"
#include "DisplaySystem.h"

#ifdef COMPILE_IR1838_

#define T3DE(); TIM3->CR1 &= ~1;//关闭T3
#define T3EN(); TIM3->CR1 |=  1;//使能T3，在单脉冲模式下，当发生更新事件时，CEN被自动清除!
#define T3CUIF(); TIM3->SR &= ~1;//(Clear Update Interrupt Flag)清除更新中断位

void IR1838_init(void)
{
	dprintf("\nIR1838...");
	//############################ EXTI3 ######################################
	//PB3
	AFIO->EXTICR[0] |= 0//!(AFIO_EXTICR[1~4])
	|1<<12//PB[3]
	;//!END

	EXTI->IMR |= //!(EXTI_IMR)
	    1<<3//开放来自线3上的中断请求
	    ;//!END

	EXTI->EMR |= //!(EXTI_EMR)
	    1<<3//开放来自线3上的时间请求
	    ;//!END

	EXTI->FTSR |= //!(EXTI_RTSR)上升沿，(EXTI->FTSR)下降沿
	    1<<3//允许输入线3上的下降沿触发
	    ;//!END

	//############################ TIM3 ######################################
	TIM3->DIER |= 0//!(TIMx_DIER)：DMA/中断使能寄存器
	    |1<<0//允许更新中断
	    ;//!END

	TIM3->CNT = 0;//计数器初值
	TIM3->PSC = 18000 - 1;//预分频，计数器时钟频率CK_CNT=fck_psc/(PSC+1)（1ms)，16bit寄存器（最大65536-1）
	TIM3->ARR = 2-1;//AutoReloadRegister，(1ms)

	TIM3->CR1 |= 0//!(TIMx_CR1)
	    |0<<0//关闭Tim
	    |0<<3//[非]{单脉冲模式(OPM)：发生下次更新事件（清除CEN），计数器停止}
		|1<<4//向下计数
		|1<<7//ARPE:auto reload preload enable，自动重装预装允许
		|0<<8//CKD[1:0]，Tdts=Tck_int
		;//!END

    T3CUIF();
	dprintf("OK!");
}

uchar irdata[32];
bool keyHolding=false;
bool irok=false;
uchar irtime=0;
bool IRevBegin=false;

void EXTI3_IRQHandler(void)
{
	static u8 i;
	
	if (!irok)
	{
		
		T3DE();

		if (IRevBegin)
		{
			irdata[i]=irtime;//存储每个电平的持续时间，irtime>3?1:0
			if (i==31)//接收完成
			{
				delayus(1);//这里必须使用延迟函数，否则在void Ircordpro(void)；中的irdata会出错，原因暂时不明
				irok=true;
				keyHolding=false;
				IRevBegin=false;
			}
			else
			{
				i++;
			}
		}
		else
		{
			if (irtime==26||irtime==27)//引导码 TC9012的头码，9ms+4.5ms，真实时间=(irtime*0.5)ms，计数周期为0.5ms
			{
				i=0;//数据头
				IRevBegin=true;
			}
			if (irtime==22||irtime==23)//9ms+2.5ms，持续按键
			{
				keyHolding=true;
				irok=true;
			}
		}
		
		irtime=0;
		if (!irok)
		{
			T3EN();
		}
	}
	
	EXTI->PR |= 1<<3;//(EXTI_PR)清除挂起位
}

extern u16 SG90_Offset;
void Ircordpro(void)//红外码值处理函数
{
	uchar i, j, k;
	uchar cord,value;
	uchar IRcord[4];//用户码+用户码+数据码+数据反码
	static uchar key;//可以表示当前按下的键码，或持续按键的键码

	if (irok)
	{
		k=0;
		if (!keyHolding)
		{
			for (i=0;i<4;i++)     //处理4个字节
			{
				for (j=1;j<=8;j++) //处理1个字节8位
				{
					cord=irdata[k++];
					
					if (cord>3)//0：脉宽0.56ms,周期1.12ms	1：脉宽1.68ms,周期2.24ms
					{
						value|=0x80;
					}
					if (j<8)
					{
						value>>=1;
					}
				}
				IRcord[i]=value;
				value=0x00;
			}
			
			IRcord[3]=~IRcord[3];
			if(IRcord[2]==IRcord[3])//数据验证，2为键码，3为2的反码
			{
				key=IRcord[2];
			}
			else
			{
				key=0x00;
			}
		}

		//键值判断
		switch (key)
		{
		case 0x0c:
			//key='1';
			if(keyHolding)
			{
				
			}
			else
			{
				beep(200,200);
			}
			break;//1
		case 0x18:
			//key='2';
			break;//2
		case 0x5e:
			//key='3';
			break;//3
		case 0x08:
			//key='4';
			break;//4
		case 0x1c:
			//key='5';
			break;//5
		case 0x5a:
			//key='6';
			break;//6
		case 0x42:
			//key='7';
			break;//7
		case 0x52:
			//key='8';
			break;//8
		case 0x4a:
			//key='9';
			break;//9
		case 0x45:
			//key='p';//电源
			break;
		case 0x47:
			//key='m';//menu
			break;
		case 0x44:
			//key='t';//Test
			break;
		case 0x40:
			//key='+';//+
			break;
		case 0x19:
			//key='-';//-
			break;
		case 0x43:
			//key='r';//Return
			break;
		case 0x15:
			//key='a';//Play
			break;
		case 0x07:
			//key='l';//left
			break;
		case 0x09:
			//key='i';//Right
			break;
		case 0x16:
			//key='0';//0
			break;
		case 0x0D:
			//key='c';//C
			break;
		case 0xff://持续按键
			//key='f';
			break;
		default://杂波数据
			//key=0x00;
			break;
		}
		
		irok=false;
		keyHolding=false;
	}
}

void TIM3_IRQHandler(void)
{
	T3CUIF();
	
	if(irtime>27)//超时关闭T3
	{
		IRevBegin=false;//非遥控器红外线发出的引导码，停止接收状态
		T3DE();
		return;
	}
	irtime++;
}

#endif
