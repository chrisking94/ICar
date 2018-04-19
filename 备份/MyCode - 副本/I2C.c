#include "I2C.h"
//I2C���ߣ�I2C1

/***˽�к�������***/
void I2C_start(void);
u16 calcCRRValue(u32 ClockSpeed);
u16 calcTRISEValue(u32 ClockSpeed);

I2CTransmitter I2CT;

//------------------------------------------------------------------
//��������I2C_init(void)
//���������null
//���ز�����null
//˵����I2C��ʼ��
//��ע��I2C1
//------------------------------------------------------------------
void I2C_init(void)
{
    /* I2C ���� */
    I2C1->CR2 = 0x0700|(u16)(I2C_PCLK/1000000);//16���������λΪƵ�ʣ�2~36MHz��8MHz��I2C_CR2��
    I2C1->OAR1 |= 0x00;//7bit��ַ��I2C_OAR1��
    I2C1->OAR2  = 0;
    I2C1->CCR = calcCRRValue(I2C_Speed);//��I2C_CCR��
    I2C1->TRISE = calcTRISEValue(I2C_Speed);//��I2C_TRISE��
    I2C1->CR1 = 0x0401;//bit15Ϊ1��ʹI2C���ڸ�λ״̬��I2C_CR1��
}

//------------------------------------------------------------------
//��������void I2C_go(void)
//���������null
//���ز�����null
//˵����I2C��ʼ���ݲ�������ͨѶ
//��ע��I2C1
//------------------------------------------------------------------
void I2C_go(void)
{
    uchar x=9;
    while (x--)//����9�������Խ��������ܴ��ڵ���������״̬��
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
    I2C1 -> CR1 &=   ~1;//PE��0
}

void I2C_ack()//����ACK
{
    I2C1->CR1 |= 0x0400;
}

void I2C_nack()//��ACK
{
    I2C1->CR1 &= 0xFBFF;
}

//------------------------------------------------------------------
//��������void I2C1_EV_IRQHandler(void)
//���������null
//���ز�����null
//˵����I2C�жϷ���
//��ע��I2C1
//------------------------------------------------------------------
void I2C1_EV_IRQHandler(void)
{
    static u32 x;//!ָ��
    static u16 mod;

    if (I2C1->SR1 & 1)//!�ѷ�����ʼ����
    {
        I2C1 -> DR = I2CT.iaddr | mod ;//!д�ӻ�����������ַ
    }
    else if (I2C1->SR1 & 1<<1)//!�ѷ��ʹӻ���ַ��
    {
        x=0;

        if (I2C1->SR2) {}//!��SR2�����SR1 bit1��

        I2C_ack();//!����ack

        if (mod==I2CWrite)//!ģʽΪ����ʱ�򲻽�����һ��,����ַ���ͺ�ֱ�ӽ��ж�ȡ
        {
            if (I2CT.iwl>0)
            {
                I2C1 -> DR = I2CT.idata[x++];//!���͵�һ���ֽ�
            }
        }
        else
        {
            if (I2CT.irl==1)
            {
                I2C_nack();//!�ر�ack
            }
        }
    }
    else if (I2C1 -> SR1 & 1<<6)//!���յ��������һ�δ����ȡ
    {
        if (I2CT.irl==1)
        {
            I2C_stop();
        }
        else
        {
            if (x==I2CT.irl-2)
            {
                //!�ڵ����ڶ����ֽ��յ������Ӧ��λ���Ա�֤���һ���ֽں�û��Ӧ��λ��NAck��
                I2C_nack();
            }
            else if (x==I2CT.irl-1)
            {
                //!�����һ���ֽ��յ������һ��ֹͣλ��ֹͣ���ߣ�Stop��
                I2C_stop();
            }
        }

        I2CT.idata[x++] = I2C1 -> DR;//!������Ĵ�����ֵд�뻺������
    }
    else if (I2C1 -> SR1 & 1<<7)//!�����ݽ������������ݼĴ���δд��������
    {
        if (x<I2CT.iwl)
        {
            //!����������д����
            I2C1 -> DR = I2CT.idata[x++];
        }
        else
        {
            if (I2CT.imod == I2CWrite)
            {
                //!дģʽ������������д����ֹͣ����
                I2C_stop();
            }
            else//!I2CT.imod == I2CRead
            {
                //!��ģʽ��д��������Ѿ����
                //!��ʼReadģʽ�Ķ�����
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
//��������void I2C1_ER_IRQHandler(void)
//���������null
//���ز�����null
//˵����I2C�����жϷ���
//��ע��I2C1
//------------------------------------------------------------------
void I2C1_ER_IRQHandler(void)       //I2C1 Error Interrupt
{
#ifdef DEBUG
    printStr("I2C Error! I2C->SR1=",USART2);
    printNum(I2C1->SR1 ,USART2);
    printStr("\n",USART2);
#endif
    if (I2C1->SR1 & 1<<10)         //Ӧ��ʧ��
    {
        I2C1->SR1 &=~(1<<10);      //����ж�
    }
    if (I2C1->SR1 & 1<<14)         //��ʱ
    {
        I2C1->SR1 &=~(1<<14);      //����ж�
    }
    if (I2C1->SR1 & 1<<11)         //����/Ƿ��
    {
        I2C1->SR1 &=~(1<<11);      //����ж�
    }
    if (I2C1->SR1 & 1<<9)          //�ٲö�ʧ
    {
        I2C1->SR1 &=~(1<<9);       //����ж�
    }
    if (I2C1->SR1 & 1<<8)          //���߳���
    {
        I2C1->SR1 &=~(1<<8);       //����ж�
    }
}

u16 calcCRRValue(u32 ClockSpeed)
{
    u16 result=0x04;
    if (ClockSpeed <= 100000)
    {
        /* �����׼ģʽ���ٶ� */
        result = I2C_PCLK / (ClockSpeed << 1);
        /* ����Ƿ�С��0x4���������Сֵ��*/
        if (result < 0x04)
        {
            result = 0x04;
        }
        /* Maximum Rise Time for standard mode */
    }
    else /*(ClockSpeed <= 400000)*/
    {
        /* �������ģʽ���ٶ� */
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

        /* ���result��CCR�Ƿ�Ϊ��*/
        if ((result & 0x0FFF) == 0)
        {
            /* ������С����ֵ */
            result |= (uint16_t)0x0001;
        }
        /* ��λF/S */
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
