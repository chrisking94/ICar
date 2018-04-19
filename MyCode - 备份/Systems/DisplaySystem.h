#ifndef _DISPLAYSYSTEM_H_
#define _DISPLAYSYSTEM_H_

#include "common.h"
#include "LCD1.8.h"
#include "Bmp.h"
#include "Font.h"

	#ifdef COMPILE_DISPLAY_SYSTEM_
	
extern TextField tfCmd;
extern TextField tfClockH;
extern TextField tfClockM;
extern TextField tfClockS;
extern ProgressBar proBar;

void DisplaySystem_init(void);

	#endif

#endif
