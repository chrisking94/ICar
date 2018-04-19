#include "CellPhoneControl.h"
#include "usart.h"

#ifdef COMPILE_CELLPHONE_CONTROL_

extern uchar ser_dat[USART_RECV_BUFF_SIZE];
extern bool bProcessCompleted;//判断一帧数据是否处理完成，当一帧数据接收完毕，则该变量被置为false
void CPC_work(void) /* 手机通过蓝牙控制小车 */
{
	int MOD;
	static int lastMod;
	
    if (!bProcessCompleted)//接收到新数据帧
    {
        MOD=ser_dat[8];
        MOD<<=8;
        MOD|=ser_dat[9];
		
		MOD-=1000;
		
		//!当int转换为char时，如果int为负数，则转换出来的char为256-int（正数），原因不明
		
		if(MOD!=lastMod)
		{
			SG90_turn(161*MOD/1000);
			lastMod=MOD;
			
			printNum(SRF05_measure(),USART1);
			printStr("\n",USART1);
		}
        
        bProcessCompleted=true;
		USART2->CR1 |= 1<<13;//!启动Usart2
    }
}

#endif
