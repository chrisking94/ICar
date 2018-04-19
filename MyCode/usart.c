#include "usart.h"
#include "stdarg.h"
//串口

/***私有函数声明***/
u32 calcBRRDiv(u32 BaudRate,u32 Pclk);

//------------------------------------------------------------------
//函数名：void usart_init(void)
//输入参数：无
//返回参数：无
//说明：初始化串口寄存器
//备注：DEBUG模式下将会初始化USART1
//------------------------------------------------------------------
void usart_init(void)
{
    //Usart2 配置
    USART2->BRR = calcBRRDiv(9600,Pclk1_36MHz);//（USART_BRR）
    USART2->GTPR = USART_PSC/2;//（USART_GTPR）
    USART2->CR1 = 0x0000202C;//（USART_CR1)0x000020EC
    USART2->CR2 = 0;				 //（USART_CR2)
    USART2->CR3 = 0;				 //（USART_CR3)
#ifdef DEBUG
//Usart1 配置
    USART1->BRR = calcBRRDiv(9600,Pclk2_72MHz);//（USART_BRR）
    USART1->GTPR = USART_PSC/2;//（USART_GTPR）
    USART1->CR1 = 0x0000202C;//（USART_CR1)
#endif
}

void printChar(char ch,USART_TypeDef* Usart)
{
	while (!(Usart->SR & 1<<7));//!等待上次传输结束
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
//函数名：void USART2_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：串口接收中断服务
//------------------------------------------------------------------
const uchar BTCtrl_DataFrameHeader[5]={0xa5,0x5a,0x11,0xf5,0};//蓝牙控制数据帧头
uchar ser_dat[USART_RECV_BUFF_SIZE];//数据缓冲区
bool bProcessCompleted=true;//判断一帧数据是否处理完成，当一帧数据接收完毕，则该变量被置为false

enum BTProcessingFlag/*蓝牙数据接收模式*/
{
    CheckingFrameHeader,//检测数据帧头
    ReceivingData//接收数据包
};
void USART2_IRQHandler(void)
{
    static uchar ser_x=0;
    static uchar mat_x=0;//match x
    static uchar sum=0;
    static enum BTProcessingFlag dflag=CheckingFrameHeader;
		
    if (bProcessCompleted)//!上一帧数据处理完成之后
    {
        if (USART2->SR & 1<<5)//!判断读寄存器是否非空
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
                if (BTCtrl_DataFrameHeader[mat_x]==0)//!数据头结束，更换模式准备接收数据
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
                if (ser_x==USART_RECV_BUFF_SIZE-1)//!只接收USART_RECV_BUFF_SIZE字节
                {
                    sum+=BTCtrl_DataFrameHeader[2];
                    sum+=BTCtrl_DataFrameHeader[3];
                    dflag=CheckingFrameHeader;//!切换接收模式
                    if (ser_dat[USART_RECV_BUFF_SIZE-1]==sum)//!数据检验
                    {
                        bProcessCompleted=false;//!数据正确，置接收完成标志
						USART2->CR1 &= ~(1<<13);//!停止usart2
                    }
                }
                sum+=ser_dat[ser_x];//!数据验证
				ser_x++;
            }
        }
    }
}

//------------------------------------------------------------------
//函数名：u32 calcBRRDiv(u32 BaudRate,u32 Pclk)
//输入参数：BaudRate=波特率,Pclk=RCC时钟频率
//返回参数：应置入寄存器的值
//说明：计算相应波特率以及时钟频率对应的BRRDiv寄存器中应该填入的值
//备注：计算方法参照（分数波特率的产生）
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
    div_frac+=(frac-div_frac)>=0.5?1:0;//四舍五入

    if (div_frac==USART_PSC)//满USART_PSC进位
    {
        div_frac=0;
        div_mant++;
    }
    div_mant<<=4;
    div_mant|=div_frac;
    return div_mant;
}

