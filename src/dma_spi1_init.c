#include "stm32f1xx.h"

/* =========================================================
   SPI1 DMA
   SPI1_TX -> DMA1_CH3 without interrupt
========================================================= */

void dma_spi1_init(void)
{
      RCC->AHBENR |= RCC_AHBENR_DMA1EN;

      SPI1->CR2 |= SPI_CR2_TXDMAEN;
   
      DMA1_Channel3->CCR &= ~DMA_CCR_EN;
   
      DMA1_Channel3->CCR =  DMA_CCR_DIR 
      | DMA_CCR_MINC | DMA_CCR_PL_1;
 //     | DMA_CCR_PSIZE_0 // 8-bit peripheral size (SPI1->DR is accessed as 8-bit)
 //     | DMA_CCR_MSIZE_0; // 16-bit data size (if using DFF = 1 in SPI_CR1)
//      | DMA_CCR_TCIE; // Enable transfer complete interrupt if needed
   
}