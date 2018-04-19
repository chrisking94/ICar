#include "TCR5000.h"
#include "adc.h"

#ifdef COMPILE_TCR5000_

#ifdef DEBUG
	#include "usart.h"
#endif

//成员
u16 maxvs[3]={2000,2000,2000},minvs[3]={1000,1000,1000},widths[3]={1000,1000,1000};
u16 senVals[3];

void Senser_getsv(u8 sen)
{
	senVals[sen]=ADC_DMA_getVal(sen);
	
	/*
	printDebug("--------Sen=",sen);
	printDebug("   v=",senVals[sen]);
	printDebug("minv=",minvs[sen]);
	printDebug("maxv=",maxvs[sen]);
	printDebug("-------------\n",0);
	delayms(3200);
	// */
	
	//校准
	if (senVals[sen]>maxvs[sen])
	{
		maxvs[sen]=senVals[sen];
		widths[sen]=maxvs[sen]-minvs[sen];
	}
	else if (senVals[sen]<minvs[sen])
	{
		minvs[sen]=senVals[sen];
		widths[sen]=maxvs[sen]-minvs[sen];
	}
	
	senVals[sen]=SENSER_MAXVAL*(senVals[sen]-minvs[sen])/widths[sen];//返回值域为0~SENSER_MAXVAL
}

void Senser_reset(void)
{
	u32 i;
	for(i=0;i<3;i++)
	{
		maxvs[i]=2000;
		minvs[i]=1000;
		widths[i]=1000;
	}
}

#endif
