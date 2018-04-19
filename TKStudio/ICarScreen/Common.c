#include "common.h"

void delay(uint t)
{
	uint j;
    for (;t>0;t--)
        for (j=5;j>0;j--);
}

void delayms(uint t)//—”≥Ÿi∫¡√Î
{
    unsigned char a,b;
	while(t--)
    for(b=2;b>0;b--)
        for(a=227;a>0;a--);
}

void delayus(uint t)//usº∂—” ±
{
	while(t--);
}
