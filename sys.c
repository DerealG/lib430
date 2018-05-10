#include "sys.h"

void Clock_init()
{
  WDTCTL = WDTPW | WDTHOLD;		//关闭看门狗

  BCSCTL1=CALBC1_16MHZ;		//DCO 16MHZ
  DCOCTL=CALDCO_16MHZ;		//DCO设置为16MHZ
  BCSCTL2 |= SELM_1 + DIVM_0;		//MCLK —— DCO —— 16MHZ
  BCSCTL2 &=~SELS;		        //SMCLK —— DCO
  BCSCTL2 |= DIVS_1;			//二分频 ——8MHZ
  BCSCTL1 &= ~XTS;
  BCSCTL3 |= LFXT1S_2;          // 使用内部VLOCLK作为ACLK，25℃下约为10kHz
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
  ADC10CTL0 = ADC10ON; //开启ADC
  //参考电压
  //SREF_0              Vcc
  //SREF_1              内部参考电压
  //REFON	        开启内部参考电压、
  //REF2_5V 2.5V	选择内部参考电压（默认 1.5V）
  
  //2.5V 参考电压
  //ADC10CTL0 = SREF_1+REF2_5V+REFON+ADC10ON;
  
  //1.5V 参考电压
  //ADC10CTL0 = SREF_1+REFON+ADC10ON;
  ADC10CTL1 = INCH_4; //通道选择
  ADC10AE0 |= BIT4;	//ADC 输入口使能
  ADC10CTL0|=ENC;     //使能转换
}
u16 get_ADC()
{
  ADC10CTL0|=ADC10SC; //开启一次转换
  while((ADC10CTL0&ADC10IFG)==0);//等待转换完成
  return ADC10MEM;
}

#pragma vector=PORT1_VECTOR //P1外部中断服务程序
__interrupt void Port_1(void)
{
  delay(50);
  if((P1IN&BIT3)==0)
  {
    P1OUT ^= BIT0;
  }
  P1IFG &= ~BIT3;
}