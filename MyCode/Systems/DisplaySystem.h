#ifndef _DISPLAYSYSTEM_H_
#define _DISPLAYSYSTEM_H_

#include "common.h"
#include "LCD1.8.h"
#include "Bmp.h"
#include "Font.h"

	#ifdef COMPILE_DISPLAY_SYSTEM_
	
extern TextField tfCmd;

void DisplaySystem_init(void);
void dprintf(char* format,...);
void DS_printTime(void);
void DS_printDate(void);

	#endif

#endif
