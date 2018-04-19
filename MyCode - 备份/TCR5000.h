#ifndef TCR5000_H_
#define TCR5000_H_

#include "Common.h"

	#ifdef COMPILE_TCR5000_

#define SENSER_MAXVAL 512

#define SENSER_ORIGIN_MIN 700//未经处理的传感器值最大值

void Senser_getsv(u8 sen);//0:左，1:右
void Senser_reset(void);

	#endif

#endif
