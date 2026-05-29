#include "stm32f1xx.h"

void init_cpu_freq() {

    FLASH->ACR |= FLASH_ACR_PRFTBE; // Enable flash prefetch buffer
    FLASH->ACR |= FLASH_ACR_LATENCY_2; // Set flash latency to 2 wait states (for 72MHz operation)
    
    RCC->CR |= RCC_CR_HSEON; // Enable HSE
    while (!(RCC->CR & RCC_CR_HSERDY)); // Wait for HSE to be ready

    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // Set APB1 prescaler to 2 (max 36MHz)

    RCC->CFGR &= ~RCC_CFGR_PLLMULL; // Clear PLL multiplier bits
    RCC->CFGR |= RCC_CFGR_PLLSRC; // Set PLL source to HSE
    RCC->CFGR |= (RCC_CFGR_PLLMULL9); // Set PLL multiplier to 9 (8MHz * 9 = 72MHz)

    RCC->CR |= RCC_CR_PLLON; // Enable PLL
    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait for PLL to be ready

    RCC->CFGR &= ~RCC_CFGR_SW; // Clear system clock switch bits
    RCC->CFGR |= RCC_CFGR_SW_PLL; // Select PLL as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Wait for PLL to be used as system clock
}
