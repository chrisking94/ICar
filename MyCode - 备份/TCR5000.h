#ifndef TCR5000_H_
#define TCR5000_H_

#include "Common.h"

	#ifdef COMPILE_TCR5000_

#define SENSER_MAXVAL 512

#define SENSER_ORIGIN_MIN 700//δ������Ĵ�����ֵ���ֵ

void Senser_getsv(u8 sen);//0:��1:��
void Senser_reset(void);

	#endif

#endif
