#include "at24c32.h"
#include "I2C.h"

#ifdef COMPILE_AT24C32_

void AT24C32_test(void)
{
	I2CT.iaddr = at24c32;
	I2CT.imod = I2CWrite;
	I2CT.iwl = 3;
	I2CT.idata[0] = 0;
	I2CT.idata[1] = 10;
	I2CT.idata[3] = 25;
	I2C_go();
	I2C_wait();
	
	I2CT.imod=I2CRead;
	I2CT.iwl=2;
	I2CT.irl=1;
	I2CT.idata[0] = 0;
	I2CT.idata[1] = 10;
	I2C_go();
	I2C_wait();
	
	printDebug("AT24C32: ",I2CT.idata[0]);
}

#endif
