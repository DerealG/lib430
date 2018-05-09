#include "sys.h"
#include "uart.h"
#include "timer.h"

u16 PWM2_CCR0,PWM2_Duty1,PWM2_Duty2;

void main( void )
{
  Clock_init();
  UART_init();
  //PWM1_init();
  PWM2_init();
  IO_init();
  Key_init();
  ADC_init();
  //Timer_Overflow_init(); 
  Timer_Compare_init();
  _EINT(); //ȫ���ж�
  while(1)
  {
    if(Recv_OK) //���һ�ν���
    {
      Recv_OK = 0;
      //���ڵ���
      //��ʽ: PWM2_CCR0,PWM2_Duty1,PWM2_Duty2
      //���磺1000,50,50
      sscanf(RX_buff,"%d,%d,%d",&PWM2_CCR0,&PWM2_Duty1,&PWM2_Duty2);
      TA1CCR0 = PWM2_CCR0;
      TA1CCR1 = (u16)((float)PWM2_CCR0*((float)PWM2_Duty1/100.0));
      TA1CCR2 = (u16)((float)PWM2_CCR0*((float)PWM2_Duty2/100.0));
      printf("Recv:%s\r\n",RX_buff);
    }
  }
}
