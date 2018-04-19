#ifndef TACHOMETER_H_
#define TACHOMETER_H_

#include "Common.h"

	#ifdef COMPILE_FC03_

#define SpeedMeasureCycle 200//测速周期单位ms

extern u16 TMLSpeed;
extern u16 TMRSpeed;

void FC03_init(void);

	#endif

#endif
