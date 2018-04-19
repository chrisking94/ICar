#ifndef HMC5883_H_
#define HMC5883_H_

#include "Common.h"

	#ifdef COMPILE_HMC5883_

typedef struct 
{
	u16 x;
	u16 y;
	u16 z;
}VPoint;
	
void HMC5883_init(void);
void HMC5883_readVPoint(VPoint* pVPoint);
	
	#endif
		
#endif
