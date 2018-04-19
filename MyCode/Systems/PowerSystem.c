#include "PowerSystem.h"

#ifdef COMPILE_POWER_SYSTEM_

void TIM4_IRQHandler(void)//TIM4ÅäÖÃÔÚL298NÖÐ
{
	
}

u16 ls=500;
u16 rs=500;
void PS_calibrate(s16 speed)
{
	if(TMRSpeed==TMLSpeed) return;
	
	if(TMRSpeed>TMLSpeed)
	{
		rs--;
	}
	else
	{
		rs++;
	}
	L298N_setLS(ls);
	L298N_setRS(rs);
	TMLSpeed=0;
	TMRSpeed=0;
}

#endif
