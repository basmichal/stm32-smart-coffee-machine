#ifndef RTC_H
#define RTC_H

#include <stdint.h>

typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;

} rtc_time_t;

extern rtc_time_t rtc_time;

void rtc_init(void);

void rtc_set_time(uint8_t h,
                  uint8_t m,
                  uint8_t s);

void rtc_update_from_hw(void);


#endif