#include "usart.h"
#include "stdarg.h"
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

#ifdef DEBUG
void printDebug(char *str,s32 num)
{
	printStr(str,USART2);
	printNum(num,USART2);
	printChar('\n',USART2);
}

void printf(char* format,...)
{
	va_list args;
	
	va_start(args,format);
	
	while(*format)
	{
		if(*format=='%')
		{
			switch(*++format)
			{
				case 'c':
					printChar((char)va_arg(args,int),USART2);
					break;
				case 's':
					printStr(va_arg(args,char*),USART2);
					break;
				case 'd':
					//printNUM((s32)va_arg(args,s32),USART2);
					break;
				case '%':
					printChar('%',USART2);
					break;
			}
		}
		else
		{
			printChar(*format,USART2);
		}
		format++;
	}
	
	va_end(args);
}
#endif

//------------------------------------------------------------------
//��������void USART2_IRQHandler(void)
//���������null
//���ز�����null
//˵�������ڽ����жϷ���
//------------------------------------------------------------------
const uchar BTCtrl_DataFrameHeader[5]={0xa5,0x5a,0x11,0xf5,0};//������������֡ͷ
uchar ser_dat[USART_RECV_BUFF_SIZE];//���ݻ�����
bool bProcessCompleted=true;//�ж�һ֡�����Ƿ�����ɣ���һ֡���ݽ�����ϣ���ñ�������Ϊfalse

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
                if (USART2->DR==BTCtrl_DataFrameHeader[mat_x])
                {
                    mat_x++;
                }
                else
                {
                    mat_x=0;
                }
                if (BTCtrl_DataFrameHeader[mat_x]==0)//!����ͷ����������ģʽ׼����������
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
                if (ser_x==USART_RECV_BUFF_SIZE-1)//!ֻ����USART_RECV_BUFF_SIZE�ֽ�
                {
                    sum+=BTCtrl_DataFrameHeader[2];
                    sum+=BTCtrl_DataFrameHeader[3];
                    dflag=CheckingFrameHeader;//!�л�����ģʽ
                    if (ser_dat[USART_RECV_BUFF_SIZE-1]==sum)//!���ݼ���
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

