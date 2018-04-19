#include "lcd1.8.h"

#define	DP		P0//�������ݿ�
#define BUFSIZE	32//��ַ���

void Parallel_init()//���ڳ�ʼ��
{
	//�ⲿ�ж�0
	IT0=1;//�½��ش���
	EX0=1;
	EA=1;
	//�ȴ�����
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
	//���ڽ���ʱ������
	//�½��ش�������
	static	uchar mod=0;
	
	switch(mod)
	{
	case 0://�ȴ�����״̬���յ��ĵ�һ���ֽ���Ϊģʽ��
		mod=DP;
		break;
	case 1://��ʾģʽ
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
