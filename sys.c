#include "sys.h"

void Clock_init()
{
  WDTCTL = WDTPW | WDTHOLD;		//�رտ��Ź�

  BCSCTL1=CALBC1_16MHZ;		//DCO 16MHZ
  DCOCTL=CALDCO_16MHZ;		//DCO����Ϊ16MHZ
  BCSCTL2 |= SELM_1 + DIVM_0;		//MCLK ���� DCO ���� 16MHZ
  BCSCTL2 &=~SELS;		        //SMCLK ���� DCO
  BCSCTL2 |= DIVS_1;			//����Ƶ ����8MHZ
  BCSCTL1 &= ~XTS;
  BCSCTL3 |= LFXT1S_2;          // ʹ���ڲ�VLOCLK��ΪACLK��25����ԼΪ10kHz
}

void delay(u16 ms)
{
  int i;
  for(i=0;i<ms;i++)
    __delay_cycles(16000);
}

void IO_init()
{
   P1DIR |= BIT0; //LED
}

void Key_init()
{
  P1DIR &=~BIT3;
  P1REN |= BIT3;
  P1IES |= BIT3;
  P1IE |= BIT3;
  P1IFG &=~BIT3;
}

void ADC_init()
{
  ADC10CTL0 = ADC10ON; //����ADC
  //�ο���ѹ
  //SREF_0              Vcc
  //SREF_1              �ڲ��ο���ѹ
  //REFON	        �����ڲ��ο���ѹ��
  //REF2_5V 2.5V	ѡ���ڲ��ο���ѹ��Ĭ�� 1.5V��
  
  //2.5V �ο���ѹ
  //ADC10CTL0 = SREF_1+REF2_5V+REFON+ADC10ON;
  
  //1.5V �ο���ѹ
  //ADC10CTL0 = SREF_1+REFON+ADC10ON;
  ADC10CTL1 = INCH_4; //ͨ��ѡ��
  ADC10AE0 |= BIT4;	//ADC �����ʹ��
  ADC10CTL0|=ENC;     //ʹ��ת��
}
u16 get_ADC()
{
  ADC10CTL0|=ADC10SC; //����һ��ת��
  while((ADC10CTL0&ADC10IFG)==0);//�ȴ�ת�����
  return ADC10MEM;
}

#pragma vector=PORT1_VECTOR //P1�ⲿ�жϷ������
__interrupt void Port_1(void)
{
  delay(50);
  if((P1IN&BIT3)==0)
  {
    P1OUT ^= BIT0;
  }
  P1IFG &= ~BIT3;
}