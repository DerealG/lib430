#ifndef UART_H_
#define UART_H_

#include "sys.h"

#define RECV_MAX 20
extern char RX_buff[];
extern u8 Recv_OK;

void UART_init();

#endif