#include "sys.h"

void PWM1_init()
{
  P1DIR |= BIT6;		//P1.6  PWM���
  P1SEL |= BIT6;
  TA0CTL=TASSEL_2+MC_1+ID_3;	//���ö�ʱ��ʱ��SMCLK����8MHz��������,8��Ƶ TA0Ϊ1MHz
  TA0CCTL1=OUTMOD_7;		//�Ƚ����ģʽ�����ģʽ7
  TA0CCR0=1000;
  TA0CCR1=500;
}

void PWM2_init()
{
  P2DIR |= BIT1 + BIT4;		//P2.1  P2.4  PWM���
  P2SEL |= BIT1 + BIT4;
  TA1CTL=TASSEL_2+MC_1+ID_3;	//���ö�ʱ��ʱ��SMCLK����8MHz��������,8��Ƶ TA1Ϊ1MHz
  TA1CCTL1=OUTMOD_7;		//�Ƚ����ģʽ�����ģʽ7
  TA1CCTL2=OUTMOD_7;		//�Ƚ����ģʽ�����ģʽ7
  TA1CCR0=1000;
  TA1CCR1=500;
  TA1CCR2=500;
}
void Timer_Overflow_init()
{
  //����Timer0������������SMCLK����8MHz��8��Ƶ��TA0Ϊ1MHz�����ʱ��Ϊ1*1M*65535=0.065535s=65.535ms
  TA0CTL |= TASSEL_2 + MC_2 + ID_3 + TAIE + TACLR; 
}
void Timer_Compare_init()
{
  //����Timer0����������SMCLK����8MHz��8��Ƶ��TA0Ϊ1MHz
  TA0CTL |= TASSEL_2+MC_1 +ID_3; 
  TA0R=0;		//����
  TA0CCR0 = 10000;      //��TA0CCR0=10000ʱ�����ʱ��Ϊ1/1M*10000=0.01s=10ms��ע�⣺TA0CCR0���Ϊ65535
  TA0CCTL0 |= CCIE;	//ʹ�ܶ�ʱ���Ƚ��ж�
}
/* TimerAʹ�������ж�������������������CCR0���ȼ����ռ��A0_VECTOR;
 * TimerA��������������жϺ���������CCR1 CCR2ռ��A1_VECTOR.
 * ����A1_VECTOR�������ж���ͨ��TAIV�ж������Ĵ��������ĸ������жϣ��������ȼ���
 */
#pragma vector=TIMER0_A0_VECTOR //���Timer_Compare_init()ʹ��
__interrupt void Timer_A_A0 (void)
{
  P1OUT ^= BIT0;
  //printf("ADC:%d\r\n",get_ADC()); //���ADC
}
#pragma vector=TIMER0_A1_VECTOR //���Timer_Overflow_init()ʹ��
__interrupt void Timer_A_A1 (void)
{
  switch(TA0IV) //�ж��Ƿ��Ƕ�ʱ��0����ж�
  {
  case 2: break;
  case 4: break;
  case 10: P1OUT ^= BIT0; break;
  }

}