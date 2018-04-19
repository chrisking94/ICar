#include "I2C.h"
//I2C总线，I2C1

/***私有函数声明***/
void I2C_start(void);
u16 calcCRRValue(u32 ClockSpeed);
u16 calcTRISEValue(u32 ClockSpeed);

I2CTransmitter I2CT;

//------------------------------------------------------------------
//函数名：I2C_init(void)
//输入参数：null
//返回参数：null
//说明：I2C初始化
//备注：I2C1
//------------------------------------------------------------------
void I2C_init(void)
{
    /* I2C 配置 */
    I2C1->CR2 = 0x0700|(u16)(I2C_PCLK/1000000);//16进制最后两位为频率，2~36MHz，8MHz（I2C_CR2）
    I2C1->OAR1 |= 0x00;//7bit地址（I2C_OAR1）
    I2C1->OAR2  = 0;
    I2C1->CCR = calcCRRValue(I2C_Speed);//（I2C_CCR）
    I2C1->TRISE = calcTRISEValue(I2C_Speed);//（I2C_TRISE）
    I2C1->CR1 = 0x0401;//bit15为1则使I2C处于复位状态（I2C_CR1）
}

//------------------------------------------------------------------
//函数名：void I2C_go(void)
//输入参数：null
//返回参数：null
//说明：I2C开始根据参数进行通讯
//备注：I2C1
//------------------------------------------------------------------
void I2C_go(void)
{
    uchar x=9;
    while (x--)//产生9个脉冲以解锁（可能存在的总线锁死状态）
    {
        I2C_SCL=0;
        delayus(5);
        I2C_SCL=1;
        delayus(5);
    }
    I2C_start();
}

void I2C_start()
{
    I2C1->CR1 |=   1<<8;
}

void I2C_stop()
{
    I2C1 -> CR1 |=   1<<9;
}

void I2C_end()
{
    I2C1 -> CR1 &=   ~1;//PE置0
}

void I2C_ack()//允许ACK
{
    I2C1->CR1 |= 0x0400;
}

void I2C_nack()//无ACK
{
    I2C1->CR1 &= 0xFBFF;
}

//------------------------------------------------------------------
//函数名：void I2C1_EV_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：I2C中断服务
//备注：I2C1
//------------------------------------------------------------------
void I2C1_EV_IRQHandler(void)
{
    static u32 x;//!指针
    static u16 mod;

    if (I2C1->SR1 & 1)//!已发送启始条件
    {
        I2C1 -> DR = I2CT.iaddr | mod ;//!写从机（主机）地址
    }
    else if (I2C1->SR1 & 1<<1)//!已发送从机地址☆
    {
        x=0;

        if (I2C1->SR2) {}//!读SR2以清除SR1 bit1↑

        I2C_ack();//!启用ack

        if (mod==I2CWrite)//!模式为读的时候不进行这一步,即地址发送后直接进行读取
        {
            if (I2CT.iwl>0)
            {
                I2C1 -> DR = I2CT.idata[x++];//!发送第一个字节
            }
        }
        else
        {
            if (I2CT.irl==1)
            {
                I2C_nack();//!关闭ack
            }
        }
    }
    else if (I2C1 -> SR1 & 1<<6)//!接收到新数据且还未被读取
    {
        if (I2CT.irl==1)
        {
            I2C_stop();
        }
        else
        {
            if (x==I2CT.irl-2)
            {
                //!在倒数第二个字节收到后清除应答位，以保证最后一个字节后没有应答位（NAck）
                I2C_nack();
            }
            else if (x==I2CT.irl-1)
            {
                //!在最后一个字节收到后产生一个停止位，停止总线（Stop）
                I2C_stop();
            }
        }

        I2CT.idata[x++] = I2C1 -> DR;//!将收入寄存器的值写入缓存数组
    }
    else if (I2C1 -> SR1 & 1<<7)//!新数据将被发送且数据寄存器未写入新数据
    {
        if (x<I2CT.iwl)
        {
            //!连续向总线写数据
            I2C1 -> DR = I2CT.idata[x++];
        }
        else
        {
            if (I2CT.imod == I2CWrite)
            {
                //!写模式的所有数据已写出，停止中线
                I2C_stop();
            }
            else//!I2CT.imod == I2CRead
            {
                //!读模式的写命令操作已经完成
                //!开始Read模式的读操作
                mod=I2CRead;
                I2C_start();
            }
        }
    }
    else
    {
        printStr(" SR1="   ,USART2);
        printNum(I2C1->SR1   ,USART2);
        //!printStr(" "   ,USART2);
    }
}

//------------------------------------------------------------------
//函数名：void I2C1_ER_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：I2C错误中断服务
//备注：I2C1
//------------------------------------------------------------------
void I2C1_ER_IRQHandler(void)       //I2C1 Error Interrupt
{
#ifdef DEBUG
    printStr("I2C Error! I2C->SR1=",USART2);
    printNum(I2C1->SR1 ,USART2);
    printStr("\n",USART2);
#endif
    if (I2C1->SR1 & 1<<10)         //应答失败
    {
        I2C1->SR1 &=~(1<<10);      //清除中断
    }
    if (I2C1->SR1 & 1<<14)         //超时
    {
        I2C1->SR1 &=~(1<<14);      //清除中断
    }
    if (I2C1->SR1 & 1<<11)         //过载/欠载
    {
        I2C1->SR1 &=~(1<<11);      //清除中断
    }
    if (I2C1->SR1 & 1<<9)          //仲裁丢失
    {
        I2C1->SR1 &=~(1<<9);       //清除中断
    }
    if (I2C1->SR1 & 1<<8)          //总线出错
    {
        I2C1->SR1 &=~(1<<8);       //清除中断
    }
}

u16 calcCRRValue(u32 ClockSpeed)
{
    u16 result=0x04;
    if (ClockSpeed <= 100000)
    {
        /* 计算标准模式下速度 */
        result = I2C_PCLK / (ClockSpeed << 1);
        /* 检测是否小于0x4（允许的最小值）*/
        if (result < 0x04)
        {
            result = 0x04;
        }
        /* Maximum Rise Time for standard mode */
    }
    else /*(ClockSpeed <= 400000)*/
    {
        /* 计算快速模式下速度 */
        if (I2C_DutyCycle == I2C_DutyCycle2)
        {
            /* Fast mode speed calculate: Tlow/Thigh = 2 */
            result = (uint16_t)(I2C_PCLK / (ClockSpeed * 3));
        }
        else
        {
            /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
            result = (uint16_t)(I2C_PCLK / (ClockSpeed * 25));
            /* Set DUTY bit */
            result |= I2C_DutyCycle16_9;
        }

        /* 检测result中CCR是否为零*/
        if ((result & 0x0FFF) == 0)
        {
            /* 设置最小允许值 */
            result |= (uint16_t)0x0001;
        }
        /* 置位F/S */
        result |=0x8000;
    }
    return result;
}

/* Calc Maximum Rise Time for fast mode */
u16 calcTRISEValue(u32 ClockSpeed)
{
    u16 value;
    value=(u16)(I2C_PCLK / 1000000);
    if (ClockSpeed > 100000)
    {
        value = value*300/1000;
    }
    value++;
    return value;
}
