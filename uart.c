#include "uart.h"

char RX_buff[RECV_MAX+1];
u8 Recv_p = 0;
u8 Recv_OK = 0;

int putchar( int c ) //重定向printf函数
{
  if (c == '\n') 
  { 
    UCA0TXBUF = '\r';
    while(UCA0STAT & UCBUSY);
  }
  UCA0TXBUF = c;
  while(UCA0STAT & UCBUSY);
  return c;
}

void UART_init()
{
    P1DIR &=~BIT1;
    P1DIR |= BIT2;
    P1SEL  = BIT1 + BIT2;		// P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2;		// P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |=UCSWRST;		        //开始配置
    UCA0CTL1 |= UCSSEL_2;		// SMCLK

    UCA0BR0 = 65;		        //9600波特率
    UCA0BR1 = 3;		        //波特率计算：（3*256+65）*9600=7 996 800 Hz=8MHz
    UCA0MCTL = UCBRS0 + UCBRS1;		//Modulation UCBRSx = 1

    UCA0CTL1&=~UCSWRST;		        //配置结束
    IE2=UCA0RXIE;		        //开接收使能
}

#pragma vector=USCIAB0RX_VECTOR  //UART接受中断
__interrupt void USCI0RX_ISR(void)
{
  while(!(IFG2 & UCA0RXBUF));
  if(Recv_p<=RECV_MAX) //防止越界写数组
  {
     RX_buff[Recv_p]=UCA0RXBUF;   //读取
  }
  if(Recv_p>RECV_MAX || RX_buff[Recv_p]=='\n')    //缓存满 或 读取到回车
  {
    RX_buff[Recv_p]='\0';        //增加字符串结尾
    Recv_p=0;    //接收指针清零
    Recv_OK = 1; //完成标志
  }
  else
    Recv_p++;    //接收指针++
}

