#include "sys.h"

void PWM1_init()
{
  P1DIR |= BIT6;		//P1.6  PWM输出
  P1SEL |= BIT6;
  TA0CTL=TASSEL_2+MC_1+ID_3;	//设置定时器时钟SMCLK——8MHz，增计数,8分频 TA0为1MHz
  TA0CCTL1=OUTMOD_7;		//比较输出模式，输出模式7
  TA0CCR0=1000;
  TA0CCR1=500;
}

void PWM2_init()
{
  P2DIR |= BIT1 + BIT4;		//P2.1  P2.4  PWM输出
  P2SEL |= BIT1 + BIT4;
  TA1CTL=TASSEL_2+MC_1+ID_3;	//设置定时器时钟SMCLK——8MHz，增计数,8分频 TA1为1MHz
  TA1CCTL1=OUTMOD_7;		//比较输出模式，输出模式7
  TA1CCTL2=OUTMOD_7;		//比较输出模式，输出模式7
  TA1CCR0=1000;
  TA1CCR1=500;
  TA1CCR2=500;
}
void Timer_Overflow_init()
{
  //设置Timer0，连续计数，SMCLK——8MHz，8分频，TA0为1MHz，溢出时间为1*1M*65535=0.065535s=65.535ms
  TA0CTL |= TASSEL_2 + MC_2 + ID_3 + TAIE + TACLR; 
}
void Timer_Compare_init()
{
  //设置Timer0，增计数，SMCLK——8MHz，8分频，TA0为1MHz
  TA0CTL |= TASSEL_2+MC_1 +ID_3; 
  TA0R=0;		//清零
  TA0CCR0 = 10000;      //当TA0CCR0=10000时，溢出时间为1/1M*10000=0.01s=10ms，注意：TA0CCR0最大为65535
  TA0CCTL0 |= CCIE;	//使能定时器比较中断
}
/* TimerA使用两个中断向量，其中增计数的CCR0优先级最高占用A0_VECTOR;
 * TimerA的连续计数溢出中断和增计数的CCR1 CCR2占用A1_VECTOR.
 * 其中A1_VECTOR这三个中断又通过TAIV中断向量寄存器区分哪个产生中断，并有优先级。
 */
#pragma vector=TIMER0_A0_VECTOR //配合Timer_Compare_init()使用
__interrupt void Timer_A_A0 (void)
{
  P1OUT ^= BIT0;
  //printf("ADC:%d\r\n",get_ADC()); //输出ADC
}
#pragma vector=TIMER0_A1_VECTOR //配合Timer_Overflow_init()使用
__interrupt void Timer_A_A1 (void)
{
  switch(TA0IV) //判断是否是定时器0溢出中断
  {
  case 2: break;
  case 4: break;
  case 10: P1OUT ^= BIT0; break;
  }

}