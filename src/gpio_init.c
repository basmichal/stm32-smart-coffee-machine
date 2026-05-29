#include "stm32f1xx.h"
#include "gc9a01.h"
#include "w25q64.h"

/* =========================================================
   GPIO
========================================================= */

void gpio_init(void)
{
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | // Enable GPIOA clock
                 RCC_APB2ENR_IOPBEN | // Enable GPIOB clock
                 RCC_APB2ENR_AFIOEN| // Enable AFIO clock (for remapping)
                 RCC_APB2ENR_IOPCEN; // Enable GPIOC clock

    /* =========================================
         PA5 -> SPI1 SCK
       AF PUSH PULL 50 MHz
    ========================================= */

      GPIOA->CRL &= ~(GPIO_CRL_MODE5 |
                     GPIO_CRL_CNF5);

      GPIOA->CRL |= GPIO_CRL_MODE5_0 |
                    GPIO_CRL_MODE5_1;

      GPIOA->CRL |= GPIO_CRL_CNF5_1;
      
      /*===========================================
         PA6 -> SPI1 MISO 
                INPUT FLOATING
      ========================================*/
      
      GPIOA->CRL &= ~(GPIO_CRL_MODE6 |
                     GPIO_CRL_CNF6);
      GPIOA->CRL |= GPIO_CRL_CNF6_0;


    /* =========================================
       PA7 -> SPI1 MOSI
    ========================================= */

    GPIOA->CRL &= ~(GPIO_CRL_MODE7 |
                    GPIO_CRL_CNF7);

    GPIOA->CRL |= GPIO_CRL_MODE7_0 |
                  GPIO_CRL_MODE7_1;

    GPIOA->CRL |= GPIO_CRL_CNF7_1;

    /* =========================================
       TFT CS / DC / RST
       OUTPUT PUSH PULL
    ========================================= */

    GPIOA->CRL &= ~(GPIO_CRL_MODE2 |
                    GPIO_CRL_CNF2
                    | GPIO_CRL_MODE3 |
                    GPIO_CRL_CNF3
                    | GPIO_CRL_MODE4 |
                    GPIO_CRL_CNF4);

    GPIOA->CRL |= GPIO_CRL_MODE2_1 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE4_1;

/*==========================================
       FLASH CS
       OUTPUT PUSH PULL
    ========================================= */

   GPIOA->CRL &= ~(GPIO_CRL_MODE1 |
                GPIO_CRL_CNF1);

   GPIOA->CRL |= GPIO_CRL_MODE1_1;

/*==========================================
       Set CS pins high
    ========================================= */   
   TFT_CS_HIGH();
   FLASH_CS_HIGH();
}