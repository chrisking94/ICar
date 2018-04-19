#include "Patrol.h"

const s16 pCorrection=100;
s16 carSpeed=400;

#define kP 8
#define kI 0.04
#define kD 1
void Patrol_work(void)
{
	u16 senVals[2];
	s16 PID;
	static s16 proportion;
	static s16 lastProportion;
	static s16 integral;
	static double differential;
	
	senVals[0]=ADC_getVal(8);
	senVals[1]=ADC_getVal(9);
	
	proportion=senVals[0]-senVals[1]+pCorrection;
	integral+=proportion;
	differential=2*proportion-lastProportion;
	
	PID=kP*proportion+kI*integral+kD*differential;
	
	#ifdef DEBUG
	printStr("PID=",USART2);
	printNum(PID,USART2);
	printChar('\n',USART2);
	#endif
	
	L298N_setLS(carSpeed-PID);
	L298N_setRS(carSpeed+PID);
}
