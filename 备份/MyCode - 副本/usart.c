#include "usart.h"
//����

/***˽�к�������***/
u32 calcBRRDiv(u32 BaudRate,u32 Pclk);

//------------------------------------------------------------------
//��������void usart_init(void)
//�����������
//���ز�������
//˵������ʼ�����ڼĴ���
//��ע��DEBUGģʽ�½����ʼ��USART1
//------------------------------------------------------------------
void usart_init(void)
{
    //Usart2 ����
    USART2->BRR = calcBRRDiv(9600,Pclk1_36MHz);//��USART_BRR��
    USART2->GTPR = USART_PSC/2;//��USART_GTPR��
    USART2->CR1 = 0x0000202C;//��USART_CR1)0x000020EC
    USART2->CR2 = 0;				 //��USART_CR2)
    USART2->CR3 = 0;				 //��USART_CR3)
#ifdef DEBUG
//Usart1 ����
    USART1->BRR = calcBRRDiv(9600,Pclk2_72MHz);//��USART_BRR��
    USART1->GTPR = USART_PSC/2;//��USART_GTPR��
    USART1->CR1 = 0x0000202C;//��USART_CR1)
#endif
}

void printChar(char ch,USART_TypeDef* Usart)
{
	while (!(Usart->SR & 1<<7));//!�ȴ��ϴδ������
    Usart->DR = (u16)ch;
}

void printStr(char * str,USART_TypeDef* Usart)
{
    while (*str)
    {
        printChar(*str++,Usart);
    }
}

void printNum(s32 num,USART_TypeDef* Usart)
{
    uchar buf[10],i=0;
    if (num<0)
    {
        num=~num;
        num+=1;
        printChar('-',Usart);
    }
    do
    {
        buf[i++]=(uchar)(num%10);
    } while (num/=10);
    while (i--)
    {
        printChar(buf[i]+0x30,Usart);
    }
}

//------------------------------------------------------------------
//��������void usart_work(void)
//���������null
//���ز�����null
//˵�������ڽ������ݴ���
//------------------------------------------------------------------
uchar BT_CtrlWl[5]={0xa5,0x5a,0x11,0xf5,0};//������������֡ͷ
#define SER_BUF_SIZE 15
uchar ser_dat[SER_BUF_SIZE];
bool bProcessCompleted=true;//�ж�һ֡�����Ƿ�����ɣ���һ֡���ݽ�����ϣ���ñ�������Ϊfalse

void usart_work(void)
{
	int MOD;
	
    if (!bProcessCompleted)//���յ�������֡
    {
        MOD=ser_dat[8];
        MOD<<=8;
        MOD|=ser_dat[9];
		
		MOD-=1000;
		
		//!��intת��Ϊcharʱ�����intΪ��������ת��������charΪ256-int����������ԭ����
		
		se_turn(161*MOD/1000);
        
        bProcessCompleted=true;
		USART2->CR1 |= 1<<13;//!����Usart2
    }
}

//------------------------------------------------------------------
//��������void USART2_IRQHandler(void)
//���������null
//���ز�����null
//˵�������ڽ����жϷ���
//------------------------------------------------------------------
enum BTProcessingFlag/*�������ݽ���ģʽ*/
{
    CheckingFrameHeader,//�������֡ͷ
    ReceivingData//�������ݰ�
};
void USART2_IRQHandler(void)
{
    static uchar ser_x=0;
    static uchar mat_x=0;//match x
    static uchar sum=0;
    static enum BTProcessingFlag dflag=CheckingFrameHeader;
		
    if (bProcessCompleted)//!��һ֡���ݴ������֮��
    {
        if (USART2->SR & 1<<5)//!�ж϶��Ĵ����Ƿ�ǿ�
        {
            if (dflag==CheckingFrameHeader)
            {
                if (USART2->DR==BT_CtrlWl[mat_x])
                {
                    mat_x++;
                }
                else
                {
                    mat_x=0;
                }
                if (BT_CtrlWl[mat_x]==0)//!����ͷ����������ģʽ׼����������
                {
                    dflag=ReceivingData;
                    ser_x=0;
                    mat_x=0;
                    sum=0;
                }
            }
            else if (dflag==ReceivingData)
            {
                ser_dat[ser_x]=USART2->DR;
                if (ser_x==SER_BUF_SIZE-1)//!ֻ����SER_BUF_SIZE�ֽ�
                {
                    sum+=BT_CtrlWl[2];
                    sum+=BT_CtrlWl[3];
                    dflag=CheckingFrameHeader;//!�л�����ģʽ
                    if (ser_dat[SER_BUF_SIZE-1]==sum)//!���ݼ���
                    {
                        bProcessCompleted=false;//!������ȷ���ý�����ɱ�־
						USART2->CR1 &= ~(1<<13);//!ֹͣusart2
                    }
                }
                sum+=ser_dat[ser_x];//!������֤
				ser_x++;
            }
        }
    }
}

//------------------------------------------------------------------
//��������u32 calcBRRDiv(u32 BaudRate,u32 Pclk)
//���������BaudRate=������,Pclk=RCCʱ��Ƶ��
//���ز�����Ӧ����Ĵ�����ֵ
//˵����������Ӧ�������Լ�ʱ��Ƶ�ʶ�Ӧ��BRRDiv�Ĵ�����Ӧ�������ֵ
//��ע�����㷽�����գ����������ʵĲ�����
//------------------------------------------------------------------
u32 calcBRRDiv(u32 BaudRate,u32 Pclk)
{
    u32 div_mant;
    u32 div_frac;
    float frac;
    div_mant=Pclk/(USART_PSC*BaudRate);
    frac=(float)Pclk/(USART_PSC*BaudRate);
    frac-=div_mant;
    frac*=USART_PSC;
    div_frac=(u32)frac;
    div_frac+=(frac-div_frac)>=0.5?1:0;//��������

    if (div_frac==USART_PSC)//��USART_PSC��λ
    {
        div_frac=0;
        div_mant++;
    }
    div_mant<<=4;
    div_mant|=div_frac;
    return div_mant;
}

