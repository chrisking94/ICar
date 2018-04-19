#include "MPU6050.h"
#include "I2C.h"
#include "DisplaySystem.h"

#ifdef COMPILE_MPU6050_

//**************************************
//初始化MPU6050
//**************************************
void MPU6050_init(void)
{
	dprintf("\nMPU6050...");
	I2CT.iaddr = mpu6050;
	
	I2C_SingleWrite(PWR_MGMT_1, 0x00);	//解除休眠状态
	I2C_SingleWrite(SMPLRT_DIV, 0x07);
	I2C_SingleWrite(CONFIG, 0x06);
	I2C_SingleWrite(GYRO_CONFIG, 0x18);
	I2C_SingleWrite(ACCEL_CONFIG, 0x01);
	dprintf("OK!");
}

//**************************************
//合成数据
//**************************************
u16 MPU6050_getData(uchar REG_Address)
{
	u16 H,L;
	
	I2CT.iaddr = mpu6050;
	H=I2C_SingleRead(REG_Address);
	L=I2C_SingleRead(REG_Address+1);
	
	return (H<<8)+L;   //合成数据
}

#endif
