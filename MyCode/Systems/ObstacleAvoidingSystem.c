#include "ObstacleAvoidingSystem.h"
#include "DisplaySystem.h"

#ifdef COMPILE_OBSTACLE_AVOIDING_SYSTEM_

#include "SRF05.h"
#include "SG90.h"
#include "L298N.h"

#define OS_MinDisAllowed 200
void OAS_run(void)
{
	u16 maxDis=0;
	u16 minDis=65535;
	u16 curDis;

	u16 angle;
	u8 i;
	u8 maxI,minI;
	//���ϰ�ֱ��
	L298N_setSpeed(550,0);
	//��16���Ƕ�
	for (i=0,angle=SG90_LEFT;i<16;i++)
	{
		SG90_turn(angle);
		curDis=SRF05_measure();
		dprintf("%d\n",curDis);
		if (curDis>maxDis)
		{
			maxDis=curDis;
			maxI=i;
		}
		if (curDis<minDis)
		{
			minDis=curDis;
			minI=i;
		}

		//��С����̫С������
		if (0&&minDis<OS_MinDisAllowed)
		{
			L298N_setSpeed(-300,0);
			//ֱ���������OS_MinDisAllowed
			while (minDis<OS_MinDisAllowed);
			L298N_setSpeed(0,0);
			//���¼��
			i=0;
			angle=SG90_LEFT;
		}
		angle+=(SG90_RIGHT-SG90_LEFT)/16;
	}
	SG90_turn(SG90_MID);//�ص��м�
return;
	//��ʼת������
	L298N_setSpeed(500,(maxI-8)*4);
	//ֱ��curDis=maxDis;
	do
	{
		curDis=SRF05_measure();
	}while (curDis<maxDis-200);//200Ϊ���
}

#endif
