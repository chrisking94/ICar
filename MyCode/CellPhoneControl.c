#include "CellPhoneControl.h"
#include "usart.h"

#ifdef COMPILE_CELLPHONE_CONTROL_

extern uchar ser_dat[USART_RECV_BUFF_SIZE];
extern bool bProcessCompleted;//�ж�һ֡�����Ƿ�����ɣ���һ֡���ݽ�����ϣ���ñ�������Ϊfalse
void CPC_work(void) /* �ֻ�ͨ����������С�� */
{
	int MOD;
	static int lastMod;
	
    if (!bProcessCompleted)//���յ�������֡
    {
        MOD=ser_dat[8];
        MOD<<=8;
        MOD|=ser_dat[9];
		
		MOD-=1000;
		
		//!��intת��Ϊcharʱ�����intΪ��������ת��������charΪ256-int����������ԭ����
		
		if(MOD!=lastMod)
		{
			SG90_turn(161*MOD/1000);
			lastMod=MOD;
			
			printNum(SRF05_measure(),USART1);
			printStr("\n",USART1);
		}
        
        bProcessCompleted=true;
		USART2->CR1 |= 1<<13;//!����Usart2
    }
}

#endif
