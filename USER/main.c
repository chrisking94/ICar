#include "usart.h"
#include "common.h"
#include "L298N.h"
#include "I2C.h"
#include "SPI.h"
#include "ObstacleAvoidingSystem.h"
#include "FC03.h"
#include "ADC.h"
#include "Patrol.h"
#include "NVIC.h"
#include "hmc5883.h"
#include "ir1838.h"
#include "MasterControl.h"
#include "PowerSystem.h"
#include "DMA.h"
#include "LCD1.8.h"
#include "DisplaySystem.h"
#include "ds3231.h"
#include "at24c32.h"
#include "rtc.h"
#include "MPU6050.h"
#include "SRF05.h"
#include "SG90.h"
#include "at24c32.h"

uchar SubSystemSwitch;//��ϵͳ����


int main()
{
	VPoint CompassVPoint;
	
    common_init();
	NVIC_init();
	DMA_init();
    usart_init();
	
	SPI_init();
	L298N_init();
	//ADC_init();
    I2C_init();
	
	//##��ʼ����ʾϵͳ
	//beep(700,300);//����ʹ��beep��lcd18_init()�Ż�ɹ���ԭ���ݲ���
	//ԭ����Ϊ�������ĵ��LCD�������ģ����Է��������ʱ�������LCD��ѹ
	//���µ�ѹ�ͣ�LCD���������ֳ�ʼ��ʧ�ܵļ���
	//���������������GND�ӵ���ԴGND���˺��ѹ5V���ң����ᱻ����̫��
	lcd18_init();
	DisplaySystem_init();
	
	dprintf("Booting...\n");

	HMC5883_init();
	MPU6050_init();
	IR1838_init();
	SG90_init();
	FC03_init();
	//RTC_init();
	
	//SelectUsart(1);
	dprintf("\n#USART=1");
	
	beep(700,300);
	delayms(10);
	beep(700,300);

	dprintf("");
	dprintf("\n\n#Alpha is online#\n");
	DS_printDate();
	DS_printTime();
	L298N_setLS(1800);
	L298N_setRS(1800);
    while (1)
    {
		//usart_work();
		//OAS_run();
		//PS_calibrate(500);
		//Ircordpro();,
		ledp=!ledp;
		//delayms(1000);
		//dprintf("%d\n",SRF05_measure());
		//DS3231_read();
		//DS_printTime();
		
		//AT24C32_test();
		//HMC5883_readVPoint(&CompassVPoint);
		//delayms(1000);
		//printChar('i',2);
		//printStr("I'm King",2);
		//SG90_turn(1);SG90_turn(80);SG90_turn(160);SG90_turn(80);
		//printf("Distance=%d\r\n",SRF05_measure());
		//printf("%d:%d:%d\n",DS3231_readData(DS3231_HOUR),DS3231_readData(DS3231_MINUTE),DS3231_readData(DS3231_SECOND));
		//dprintf("accel��x=%d,y=%d,z=%d\n",MPU6050_getData(ACCEL_XOUT_H),MPU6050_getData(ACCEL_YOUT_H),MPU6050_getData(ACCEL_ZOUT_H));
		//dprintf("jjsd:x=%d,y=%d,z=%d\n",MPU6050_getData(GYRO_XOUT_H),MPU6050_getData(GYRO_YOUT_H),MPU6050_getData(GYRO_ZOUT_H));
		//printf("Right!%d.\r\n",DS3231_readData(DS3231_SECOND));
		//printf("%d:%d:%d\r\n",DS3231_readData(DS3231_HOUR),DS3231_readData(DS3231_MINUTE),DS3231_readData(DS3231_SECOND));
		if(SubSystemSwitch&1<<0)
		{
			
		} 
    }
}
