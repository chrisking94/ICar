#ifndef _POWERSYSTEM_H_
#define _POWERSYSTEM_H_
/*闭环调速*/

#include "common.h"
#include "Tachometer.h"
#include "L298N.h"

	#ifdef COMPILE_POWER_SYSTEM_

#ifdef DEBUG
	#include "usart.h"
#endif

void PS_calibrate(s16 speed);//校准
void PS_setLS(s16 speed);
void PS_setRS(s16 speed);

	#endif

#endif
