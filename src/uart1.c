#include "uart.h"
#include "rtc.h"
#include "rtc_init.h"
#include "flash.h"

#include <stdio.h>

char uart_buffer[64];
volatile uint8_t uart_index = 0;
volatile uint8_t save_request = 0;
volatile uint16_t prll_u16 = 0;

void USART1_IRQHandler(void)
{
    if(USART1->SR & USART_SR_RXNE)
    {
        char c = USART1->DR;

        if(c == ';')
{
    uart_buffer[uart_index] = 0;

    int d,mo,y,h,m,s;

    if(sscanf(uart_buffer,
        "TIME %d.%d.%d %d:%d:%d",
        &d,&mo,&y,
        &h,&m,&s) == 6)
    {
        rtc_set_time(h,m,s);
    }

    int prll;

    if(sscanf(uart_buffer,
        "PRLL %d",
        &prll) == 1)
    {
        save_request = 1;
        prll_u16 = (uint16_t)prll;
    }

    uart_index = 0;
}
else
{
    uart_buffer[uart_index++] = c;
}
    }
}