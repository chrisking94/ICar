#include "lcd1.8.h"

#define	DP		P0//并行数据口
#define BUFSIZE	32//最长字符串

void Parallel_init()//并口初始化
{
	//外部中断0
	IT0=1;//下降沿触发
	EX0=1;
	EA=1;
	//等待输入
	DP=0xff;
}

char buffer[BUFSIZE];
uchar *pBuf=buffer;
uchar bNewString=0;

int main()
{
	TextField tf;
	
	lcd18_init();
	Parallel_init();
	lcd18_cls();
	
	tf.backColor=BLACK;
	tf.font=&SystemFont;
	tf.foreColor=WHITE;
	tf.x=0;
	tf.y=0;
	tf.width=LCD18_W;
	tf.height=LCD18_H;
	lcd18_initTextField(&tf);
	lcd18_setTextField(&tf);
	
	for(;;)
	{
		if (bNewString)
		{
			bNewString=0;
			lcd18_printStr(buffer);
		}
	}
}

void EXTI0_IRQHandler(void) interrupt 0
{
	//并口接收时钟输入
	//下降沿触发采样
	static	uchar mod=0;
	
	switch(mod)
	{
	case 0://等待接收状态，收到的第一个字节作为模式字
		mod=DP;
		break;
	case 1://显示模式
		*pBuf=DP;
		if(*pBuf=='\0')
		{
			pBuf=buffer;
			bNewString=1;
			mod=0;
		}else pBuf++;
		break;
	}
}
