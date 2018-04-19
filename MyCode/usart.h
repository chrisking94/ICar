/*********************************************
USART2{TX=PA2,Rx=PA3}，USART1{TX=PA9，Rx=PA10}
*********************************************/
#ifndef USART_H_
#define USART_H_

#include "Common.h"
#include "I2C.h"
#include "SG90.h"
#include "SRF05.h"

#define USART_RECV_BUFF_SIZE 15

#define Pclk1_36MHz 36000000 //其他USART
#define Pclk2_72MHz 72000000 //USART1
#define USART_PSC 16				 //Prescaler Value 时钟预分频

//#define CrLf {0x0D,0x0A,0x00}

void usart_init(void);

#ifdef DEBUG
void printChar(char ch,USART_TypeDef* Usart);
void printStr(char * str,USART_TypeDef* Usart);
u32 calcBRRDiv(u32 BaudRate,u32 Pclk);
void printNum(s32 num,USART_TypeDef* Usart);
void printDebug(char *str,s32 num);
void printf(char* s,...);
#endif

#endif
