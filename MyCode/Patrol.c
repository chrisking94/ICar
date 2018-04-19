#include "Patrol.h"
#include "TCR5000.h"
#include "L298N.h"

#ifdef COMPILE_PATROL_

#define kP 11
#define kdI 200 //nI除数 kI=1/kdI
#define kD 120
#define kdPID 28//PID除数
#define D_INTERVAL 1200
#define I_INTERVAL 15

#define ONLINE_MINVAL 25
#define POSVAL_MAX SENSER_MAXVAL*2//位置量值域(-POSVAL_MAX,POSVAL_MAX)

#define SPEED_MIN 1700
#define SPEED_NORMAL 1950
#define SPEED_MAX 2000

#define CHECK_WAIT_TIMEOUT 100000

extern u16 senVals[3];

s32 PID;

s32 nP=0;
s32 nI=0;
s32 nD=0;

s32 nLastP=0;

u32 nDC=D_INTERVAL;
u32 nIC=I_INTERVAL;

s32 carSpeed=SPEED_NORMAL;

u32 checkWait=0;
u8 adjustMod=100;

u16 mod=1;
void Patrol_work(void)
{
	Senser_getsv(0);
	Senser_getsv(1);
	Senser_getsv(2);

	/* ------------P------------ */
	/* 计算位置 */
	if (senVals[1]>ONLINE_MINVAL)
	{
		//线在中间传感器下
		if (carSpeed<SPEED_MAX)
		{
			//加速
			carSpeed++;
		}

		if (senVals[0]>ONLINE_MINVAL)
		{
			//中间偏左
			nP=POSVAL_MAX/2-senVals[1];
		}
		else
		{
			//中间偏右
			nP=senVals[1]-POSVAL_MAX/2;
		}
	}
	else if (senVals[0]>ONLINE_MINVAL)
	{
		//线在左边传感器偏左
		nP=POSVAL_MAX-senVals[0];
	}
	else if (senVals[2]>ONLINE_MINVAL)
	{
		//线在右边传感器偏右
		nP=senVals[2]-POSVAL_MAX;
	}
	else
	{
		//线离开传感器检测范围
		if (carSpeed>SPEED_MIN)
		{
			//减速
			carSpeed--;
		}
		if (nP<0)
		{
			nP=-POSVAL_MAX;
		}
		else
		{
			nP=POSVAL_MAX;
		}
	}
	//nP/=4;

	/* ------------I------------ */
	if (nIC)
	{
		nIC--;
	}
	else
	{
		nIC=I_INTERVAL;

		if (nP>ONLINE_MINVAL||nP<-ONLINE_MINVAL)
		{
			nI+=nP/20;//nI=9*nI/10;
		}
		else
		{
			nI=0;
		}
	}

	/* ------------D------------ */
	if (nDC)
	{
		nDC--;
	}
	else
	{
		nDC=D_INTERVAL;

		nD=nP-nLastP;
		nLastP=nP;
	}

	/* PID */
	PID=nP*kP + nI/kdI + nD*kD;
	PID/=kdPID;

	L298N_setSpeed(carSpeed,PID);

	/*
	printDebug("nP=",nP*kP);
	printDebug("nI=",nI/kdI);
	printDebug("nD=",nD*kD);
	printDebug("PID=",PID);
	printChar('\n',USART2);

	printDebug("LS=",TIM4->CCR3);
	printDebug("RS=",TIM4->CCR4);
	printChar('\n',USART2);

	printDebug("senVals[0]=",senVals[0]);
	printDebug("senVals[1]=",senVals[1]);
	printDebug("senVals[2]=",senVals[2]);
	printChar('\n',USART2);
	//delayms(1000);
	// */
}

void Patrol_init(void)
{
	delayms(120);
}

#endif
