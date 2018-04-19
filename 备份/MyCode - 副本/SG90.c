#include "SG90.h"

#define SEC PAout(0) //Steering Engine Control

void SG90_turn(uchar angle)//161
{
	uchar i=8;
	static uchar lastAngle;

	if (angle!=lastAngle)
	{
		lastAngle=angle;
		while (i--)
		{
			sec=1;
			delayms(1);
			delayus(450+angle*22);
			sec=0;
			delayms(18);
		}
		Ultrasonic_measure();
	}
	
}
