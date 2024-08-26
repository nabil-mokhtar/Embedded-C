#include "uart.h"

#define UART0DR *((volatile unsigned int * const )((unsigned int *)0x101f1000)) //const pointer to address of uart 0 register 


void Uart_Send_String(unsigned char* P_TX_String )
{

while (*P_TX_String !='\0')
{
    UART0DR=(unsigned int) (*P_TX_String);

    P_TX_String++;
}


}