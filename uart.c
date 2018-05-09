#include "uart.h"

char RX_buff[RECV_MAX+1];
u8 Recv_p = 0;
u8 Recv_OK = 0;

int putchar( int c ) //�ض���printf����
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
    UCA0CTL1 |=UCSWRST;		        //��ʼ����
    UCA0CTL1 |= UCSSEL_2;		// SMCLK

    UCA0BR0 = 65;		        //9600������
    UCA0BR1 = 3;		        //�����ʼ��㣺��3*256+65��*9600=7 996 800 Hz=8MHz
    UCA0MCTL = UCBRS0 + UCBRS1;		//Modulation UCBRSx = 1

    UCA0CTL1&=~UCSWRST;		        //���ý���
    IE2=UCA0RXIE;		        //������ʹ��
}

#pragma vector=USCIAB0RX_VECTOR  //UART�����ж�
__interrupt void USCI0RX_ISR(void)
{
  while(!(IFG2 & UCA0RXBUF));
  if(Recv_p<=RECV_MAX) //��ֹԽ��д����
  {
     RX_buff[Recv_p]=UCA0RXBUF;   //��ȡ
  }
  if(Recv_p>RECV_MAX || RX_buff[Recv_p]=='\n')    //������ �� ��ȡ���س�
  {
    RX_buff[Recv_p]='\0';        //�����ַ�����β
    Recv_p=0;    //����ָ������
    Recv_OK = 1; //��ɱ�־
  }
  else
    Recv_p++;    //����ָ��++
}

