#include "I2C.h"

extern I2CTransmitter I2CT;

/***˽�к�������***/
uchar BCD_HEX(uchar val);
uchar HEX_BCD(uchar val);

void DS3231_read()
{
    uchar x;

    I2CT.iaddr = hmc5883;//at24c32;//ds3231;
    I2CT.idata [0]=0x03;
    I2CT.idata [1]=0x05;
    I2CT.idata [2]=100;
    I2CT.iwl = 1;
    I2CT.irl = 6;
    I2CT.imod=I2CRead;
    I2C_go();
	
    delayms(2000);
	
    //I2CT.idata [0]=BCD_HEX(I2CT.idata [0]);
    x=I2CT.irl;
    while (x--)
    {
        printStr("d",USART2);
        printNum(x,USART2);
        printStr("=",USART2);
        printNum(I2CT.idata[x],USART2);
        printStr(" ",USART2);
    }
    printStr("\n",USART2);
}

uchar BCD_HEX(uchar val)
{
    //BCD���룺4bit��ʾ1��ʮ��������
    uchar tmp;
    tmp=val&0x0f;//����λ����
    val>>=4;
    val&=0x0f;//����λ�Ѿ�Ϊ��?
    val*=10;
    tmp+=val;
    //0xxx xxxx => (0xxx)*10+xxxx

    return tmp;
}

uchar HEX_BCD(uchar val)
{
    uchar i,j,k;
    i=val/10;
    j=val;
    k=j+(i<<4);
    return k;
}
