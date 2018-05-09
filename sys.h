#ifndef SYS_H_
#define SYS_H_

#include "msp430.h"
#include "stdio.h"

typedef unsigned char u8;	      //无符号型
typedef unsigned short int u16;
typedef unsigned long  int u32;
typedef char char8;	              //有符号型
typedef short int int16;
typedef long  int int32;

void Clock_init();
void delay(u16 ms);
void IO_init();
void Key_init();
void ADC_init();
u16 get_ADC();

#endif