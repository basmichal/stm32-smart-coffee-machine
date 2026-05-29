#include "stm32f1xx.h"

/* =========================================================
   SYSTEM CLOCK 8 MHz
   HSI -> SYSCLK
========================================================= */

void clock_init(void)
{
    RCC->CR |= RCC_CR_HSION;

    while(!(RCC->CR & RCC_CR_HSIRDY));

    RCC->CFGR &= ~RCC_CFGR_SW;

    RCC->CFGR |= RCC_CFGR_SW_HSI;

    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    SystemCoreClockUpdate();
}