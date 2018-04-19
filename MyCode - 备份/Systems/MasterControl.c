#include "MasterControl.h"

#ifdef COMPILE_MASTER_CONTROL_

extern uchar SubSystemSwitch;

void MC_init(void)
{
	
}

void MC_run(void)
{
	
}

void HighPriorityInterrupt(void)//高优先级任务入口，在使用delay函数时可以执行
{
	//进程中不能使用delayms(),waitWhile(),delays()；否则会陷入死循环
	//Ircordpro();
}

#endif
