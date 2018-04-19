#include "hmc5883.h"
#include "I2C.h"

#ifdef COMPILE_HMC5883_

void HMC5883_init(void)
{
	I2CT.iaddr = hmc5883;//at24c32;//ds3231;
    I2CT.idata [0]=0x02;
    I2CT.idata [1]=0x00;
    I2CT.iwl = 2;
    I2CT.imod=I2CWrite;
    I2C_go();
	I2C_wait();
	I2CT.idata[0]=0x01;
	I2CT.idata[1]=0xe0;
	I2C_go();
}

void HMC5883_readVPoint(VPoint* pVPoint)
{
    I2CT.iaddr = hmc5883;//at24c32;//ds3231;
    I2CT.idata [0]=0x03;
    I2CT.iwl = 1;
    I2CT.irl = 6;
    I2CT.imod=I2CRead;
    I2C_go();
	
    I2C_wait();
	
	pVPoint->x=((u16)I2CT.idata[0])<<8|I2CT.idata[1];
	pVPoint->y=((u16)I2CT.idata[2])<<8|I2CT.idata[3];
	pVPoint->z=((u16)I2CT.idata[4])<<8|I2CT.idata[5];
	
    printDebug("x=",pVPoint->x);
	printDebug("y=",pVPoint->y);
	printDebug("z=",pVPoint->z);
	printDebug("\n",0);
}

#endif
