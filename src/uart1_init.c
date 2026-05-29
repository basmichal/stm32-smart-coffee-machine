#include "stm32f1xx.h"

/* =========================================================
   UART1
   115200
========================================================= */

void uart1_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* =========================================
       PA9 TX
       AF PUSH PULL
    ========================================= */

    GPIOA->CRH &= ~(GPIO_CRH_MODE9 |
                    GPIO_CRH_CNF9);

    GPIOA->CRH |= GPIO_CRH_MODE9_0 |
                  GPIO_CRH_MODE9_1;

    GPIOA->CRH |= GPIO_CRH_CNF9_1;

    /* =========================================
       PA10 RX
       INPUT FLOATING
    ========================================= */

    GPIOA->CRH &= ~(GPIO_CRH_MODE10 |
                    GPIO_CRH_CNF10);

    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    /* =========================================
       BAUD
       8 MHz
       115200
       USARTDIV = 4.34
    ========================================= */

    USART1->BRR = 0x45;

    USART1->CR1 |= USART_CR1_RE |
                   USART_CR1_TE |
                   USART_CR1_RXNEIE;

    USART1->CR1 |= USART_CR1_UE;

    NVIC_EnableIRQ(USART1_IRQn);
}