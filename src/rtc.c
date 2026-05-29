#include "stm32f1xx.h"
#include "rtc.h"

rtc_time_t rtc_time;

/* =========================================================
   WAIT FOR RTC
========================================================= */

static void rtc_wait(void)
{
    while(!(RTC->CRL & RTC_CRL_RTOFF));
}

/* =========================================================
   SET TIME
========================================================= */

void rtc_set_time(uint8_t h,
                  uint8_t m,
                  uint8_t s)
{
    uint32_t total =
        h * 3600UL +
        m * 60UL +
        s;

    rtc_wait();

    RTC->CRL |= RTC_CRL_CNF;

    RTC->CNTH = total >> 16;
    RTC->CNTL = total & 0xFFFF;

    RTC->CRL &= ~RTC_CRL_CNF;

    rtc_wait();
}

/* =========================================================
   UPDATE FROM HW
========================================================= */

void rtc_update_from_hw(void)
{
    uint32_t cnt;

    cnt = ((uint32_t)RTC->CNTH << 16);
    cnt |= RTC->CNTL;

    cnt %= 86400UL;

    rtc_time.hour = cnt / 3600UL;

    cnt %= 3600UL;

    rtc_time.min = cnt / 60UL;

    rtc_time.sec = cnt % 60UL;
}