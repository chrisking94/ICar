#include "usart.h"
#include "common.h"
#include "L298N.h"
#include "I2C.h"
#include "UltrasonicSystem.h"
#include "ADC.h"
#include "Patrol.h"
#include "NVIC.h"
#include "ir1838.h"
#include "MasterControl.h"

//extern I2CTransmitter I2CT;

int main()
{
    common_init();
	NVIC_init();
    usart_init();
	//L298N_init();
	//ADC_init();
    //I2C_init();
    //UltrasonicSystem_init();
	IR1838_init();
	
	beep(900,300);
	delayms(10);
	beep(900,300);

    while (1)
    {
			//delayms(1000);
			//beep(100,200);
			/*
			printStr("CH8_Val=",USART2);
			printNum(ADC_getVal(8),USART2);
			printChar('\n',USART2);
			printStr("CH9_Val=",USART2);
			printNum(ADC_getVal(9),USART2);
			printChar('\n',USART2);
*/
		//Patrol_work();
		Ircordpro();
      //usart_work();
		
		//se_turn(0);

        //DS3231_read();

        //while (1)
        //{
        //ledp=!ledp;
        /*
        delayms(1000);

        I2CT.iaddr = hmc5883;
        I2CT.idata [0]=0x03;
        I2CT.iwl = 1;
        I2CT.imod = I2CRead;
        I2CT.irl =6;

        I2C_go();

        delayms(1000);


        */
        //USART_SendData(USART2, 'L');
#ifdef DEBUG
        //printStr("HelloWord!");
        //printNum(USART2_IRQn);
        //printNum(USART2->BRR);
#endif
    }
}
