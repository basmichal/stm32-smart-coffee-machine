#include "stm32f1xx.h"

uint8_t spi1_transfer(uint8_t data)
{
    while(!(SPI1->SR & SPI_SR_TXE));

    *((volatile uint8_t*)&SPI1->DR) = data;

    while(!(SPI1->SR & SPI_SR_RXNE));

    return *((volatile uint8_t*)&SPI1->DR);
}