#include "stm32f1xx.h"

/* =========================================================
   SPI1
   MASTER
   72 MHz / 2 = 36 MHz
========================================================= */

void spi1_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI1 clock

        SPI1->CR1 = 0;
        SPI1->CR1 |= SPI_CR1_MSTR; // Master mode
        /*
         * SPI1 is on APB2. On STM32F1 with typical configuration:
         * - If APB2 prescaler = 1, PCLK2 = SYSCLK = 72 MHz (when SYSCLK = 72 MHz)
         * - SPI baud rate is PCLK2 divided by prescaler set by BR bits.
         * To get the fastest possible SPI1 clock, set BR = 0 (fPCLK/2).
         * This results in SPI clock = PCLK2/2 = 36 MHz in the common case.
         * If you need higher SPI speed, you must change the APB2 prescaler so PCLK2 equals SYSCLK,
         * if the MCU clock tree allows it.
         */
        SPI1->CR1 &= ~SPI_CR1_BR; // BR = 0 (highest SPI1 rate: fPCLK2/2)
        SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Software slave management
//        SPI1->CR1 |= SPI_CR1_DFF; // 16-bit data frame format
        SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI
}