#include "stm32f1xx.h"
#include "rtc_init.h"
#include "flash.h"

/* =========================================================
   RTC
   LSI ~40 kHz
========================================================= */

void rtc_init(void)
{

    uint16_t prll;

    flash_init();

    flash_read_halfword(0x0800FC00, &prll);

      if(prll == 0xFFFF)
      {
          prll = 39666;
      } else
      {
          prll = 45999;
      }

   RCC->APB1ENR |= RCC_APB1ENR_PWREN |
                    RCC_APB1ENR_BKPEN;

    PWR->CR |= PWR_CR_DBP;

    /* =========================================
       ENABLE LSI
    ========================================= */

    RCC->CSR |= RCC_CSR_LSION;

    while(!(RCC->CSR & RCC_CSR_LSIRDY));

    /* =========================================
       RTC CLOCK = LSI
    ========================================= */

    RCC->BDCR &= ~RCC_BDCR_RTCSEL;

    RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;

    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* =========================================
       CONFIG MODE
    ========================================= */

    RTC->CRL |= RTC_CRL_CNF;

    /* =========================================
       PRESCALER
       40000 - 1
    ========================================= */

    RTC->PRLH = 0;
    RTC->PRLL = prll;

//    RTC->CNTH = 0; // 00:00:00
//    RTC->CNTL = 0; // 00:00:00

    RTC->CRL &= ~RTC_CRL_CNF;

    while(!(RTC->CRL & RTC_CRL_RTOFF));
}

void rtc_prll(uint16_t prll)
{
    RTC->CRL |= RTC_CRL_CNF;

    RTC->PRLL = prll;

    RTC->CRL &= ~RTC_CRL_CNF;

    while(!(RTC->CRL & RTC_CRL_RTOFF));
}