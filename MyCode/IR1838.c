#include "ir1838.h"
#include "DisplaySystem.h"

#ifdef COMPILE_IR1838_

#define T3DE(); TIM3->CR1 &= ~1;//�ر�T3
#define T3EN(); TIM3->CR1 |=  1;//ʹ��T3���ڵ�����ģʽ�£������������¼�ʱ��CEN���Զ����!
#define T3CUIF(); TIM3->SR &= ~1;//(Clear Update Interrupt Flag)��������ж�λ

void IR1838_init(void)
{
	dprintf("\nIR1838...");
	//############################ EXTI3 ######################################
	//PB3
	AFIO->EXTICR[0] |= 0//!(AFIO_EXTICR[1~4])
	|1<<12//PB[3]
	;//!END

	EXTI->IMR |= //!(EXTI_IMR)
	    1<<3//����������3�ϵ��ж�����
	    ;//!END

	EXTI->EMR |= //!(EXTI_EMR)
	    1<<3//����������3�ϵ�ʱ������
	    ;//!END

	EXTI->FTSR |= //!(EXTI_RTSR)�����أ�(EXTI->FTSR)�½���
	    1<<3//����������3�ϵ��½��ش���
	    ;//!END

	//############################ TIM3 ######################################
	TIM3->DIER |= 0//!(TIMx_DIER)��DMA/�ж�ʹ�ܼĴ���
	    |1<<0//��������ж�
	    ;//!END

	TIM3->CNT = 0;//��������ֵ
	TIM3->PSC = 18000 - 1;//Ԥ��Ƶ��������ʱ��Ƶ��CK_CNT=fck_psc/(PSC+1)��1ms)��16bit�Ĵ��������65536-1��
	TIM3->ARR = 2-1;//AutoReloadRegister��(1ms)

	TIM3->CR1 |= 0//!(TIMx_CR1)
	    |0<<0//�ر�Tim
	    |0<<3//[��]{������ģʽ(OPM)�������´θ����¼������CEN����������ֹͣ}
		|1<<4//���¼���
		|1<<7//ARPE:auto reload preload enable���Զ���װԤװ����
		|0<<8//CKD[1:0]��Tdts=Tck_int
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
			irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬irtime>3?1:0
			if (i==31)//�������
			{
				delayus(1);//�������ʹ���ӳٺ�����������void Ircordpro(void)���е�irdata�����ԭ����ʱ����
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
			if (irtime==26||irtime==27)//������ TC9012��ͷ�룬9ms+4.5ms����ʵʱ��=(irtime*0.5)ms����������Ϊ0.5ms
			{
				i=0;//����ͷ
				IRevBegin=true;
			}
			if (irtime==22||irtime==23)//9ms+2.5ms����������
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
	
	EXTI->PR |= 1<<3;//(EXTI_PR)�������λ
}

extern u16 SG90_Offset;
void Ircordpro(void)//������ֵ������
{
	uchar i, j, k;
	uchar cord,value;
	uchar IRcord[4];//�û���+�û���+������+���ݷ���
	static uchar key;//���Ա�ʾ��ǰ���µļ��룬����������ļ���

	if (irok)
	{
		k=0;
		if (!keyHolding)
		{
			for (i=0;i<4;i++)     //����4���ֽ�
			{
				for (j=1;j<=8;j++) //����1���ֽ�8λ
				{
					cord=irdata[k++];
					
					if (cord>3)//0������0.56ms,����1.12ms	1������1.68ms,����2.24ms
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
			if(IRcord[2]==IRcord[3])//������֤��2Ϊ���룬3Ϊ2�ķ���
			{
				key=IRcord[2];
			}
			else
			{
				key=0x00;
			}
		}

		//��ֵ�ж�
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
			//key='p';//��Դ
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
		case 0xff://��������
			//key='f';
			break;
		default://�Ӳ�����
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
	
	if(irtime>27)//��ʱ�ر�T3
	{
		IRevBegin=false;//��ң���������߷����������룬ֹͣ����״̬
		T3DE();
		return;
	}
	irtime++;
}

#endif
