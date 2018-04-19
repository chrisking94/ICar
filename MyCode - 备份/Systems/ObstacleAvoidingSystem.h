#ifndef _OBSTACLE_AVOIDING_SYSTEM_H_
#define _OBSTACLE_AVOIDING_SYSTEM_H_

#include "Common.h"
#include "SG90.h"
#include "L298N.h"
#include "SRF05.h"

	#ifdef COMPILE_OBSTACLE_AVOIDING_SYSTEM_

static __INLINE void OAS_run(void)
{
	//u16 maxDis=0;
	//u16 minDis=65535;
	u16 curDis;
	/*
	u16 angle;
	u8 i;
	s8 maxI,minI;
	//测16个角度
	for(i=0,angle=1;i<18;i++)
	{
		SG90_turn(angle);
		curDis=SRF05_measure();
		if(curDis>maxDis)
		{
			maxDis=curDis;
			maxI=i;
		}
		if(curDis<minDis)
		{
			minDis=curDis;
			minI=i;
		}
		angle+=10;
	}
	SG90_turn(80);//回到中间
	//maxI在中间说明最大距离在中间
	L298N_setLS(500+(8-maxI)*4);
	L298N_setRS(500+(maxI-8)*4);
	
	if(maxI>8)
	{
		delayms((maxI-8)*160);
	}
	else
	{
		delayms((8-maxI)*160);
	}
	
	L298N_setLS(600);
	L298N_setRS(600);//前进
*/
	curDis=SRF05_measure();
	if(curDis<750)
	{
		L298N_setLS(650);
		L298N_setLS(550);
	}
	//printNum(curDis,USART2);
	do
	{
		curDis=SRF05_measure();
	}while(curDis<2000&&curDis!=0);
	L298N_setLS(550);
	L298N_setRS(550);
}

	#endif

#endif
