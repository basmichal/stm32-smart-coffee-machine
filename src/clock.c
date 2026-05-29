#include "clock.h"
#include "gc9a01.h"
#include "rtc.h"

#include <math.h>

#define CENTER_X 120
#define CENTER_Y 120

#define PI 3.1415926f

static uint8_t old_sec  = 255;
static uint8_t old_min  = 255;
static uint8_t old_hour = 255;

static float old_sec_angle  = 0;
static float old_min_angle  = 0;
static float old_hour_angle = 0;

static void draw_hand(float angle_deg,
                      int length,
                      uint16_t color)
{
    float rad =
        (angle_deg - 90.0f) *
        PI / 180.0f;

    int x =
        CENTER_X +
        cosf(rad) * length;

    int y =
        CENTER_Y +
        sinf(rad) * length;

    gc9a01_draw_line(
        CENTER_X,
        CENTER_Y,
        x,
        y,
        color
    );
}

void init_clock_face(void)
{
     gc9a01_draw_circle(
     120,
     120,
     110,
     0xFFF0
    );

    draw_ticks();
}

void draw_ticks(void)
{
    for(int i = 0; i < 60; i++)
    {
        float angle =
            (i * 6.0f - 90.0f) *
            PI / 180.0f;

        int outer_x =
            CENTER_X +
            cosf(angle) * 109;

        int outer_y =
            CENTER_Y +
            sinf(angle) * 109;

        int inner_radius;

        /* =====================================
           LONG MARKS EVERY 15 MIN
        ===================================== */

        if(i % 15 == 0)
        {
            inner_radius = 85;
        }

        /* =====================================
           NORMAL MARKS EVERY 5 MIN
        ===================================== */

        else if(i % 5 == 0)
        {
            inner_radius = 95;
        }

        else
        {
            continue;
        }

        int inner_x =
            CENTER_X +
            cosf(angle) * inner_radius;

        int inner_y =
            CENTER_Y +
            sinf(angle) * inner_radius;

        gc9a01_draw_line(
            inner_x,
            inner_y,
            outer_x,
            outer_y,
            0xFFFF
        );
    }
}

void clock_draw(void)

{
    float sec_angle =
        rtc_time.sec * 6.0f;

    float min_angle =
        rtc_time.min * 6.0f;

    float hour_angle =
        (rtc_time.hour % 12) * 30.0f +
        rtc_time.min * 0.5f;

   if(rtc_time.sec != old_sec)
{
    draw_hand(old_sec_angle, 85, 0x0000);

    draw_hand(sec_angle, 85, 0xF800);
    draw_hand(min_angle, 75, 0x07E0);
    draw_hand(hour_angle, 45, 0xFFFF);

    old_sec_angle = sec_angle;

    old_sec = rtc_time.sec;
}

    if(rtc_time.min != old_min)
{
    draw_hand(old_min_angle, 75, 0x0000);

    draw_hand(min_angle, 75, 0x07E0);

    old_min_angle = min_angle;

    old_min = rtc_time.min;

    draw_hand(old_hour_angle, 45, 0x0000);

    draw_hand(hour_angle, 45, 0xFFFF);

    old_hour_angle = hour_angle;

    old_hour = rtc_time.hour;
}
}

/*
void clock_draw(void)

{
    float sec_angle =
        rtc_time.sec * 6.0f;

    float min_angle =
        rtc_time.min * 6.0f;

    float hour_angle =
        (rtc_time.hour % 12) * 30.0f +
        rtc_time.min * 0.5f;

   if(rtc_time.sec != old_sec)
{
    draw_hand(old_sec_angle, 85, 0x0000);

    draw_hand(sec_angle, 85, 0xF800);
    draw_hand(min_angle, 75, 0x07E0);
    draw_hand(hour_angle, 45, 0xFFFF);

    old_sec_angle = sec_angle;

    old_sec = rtc_time.sec;
}

    if(rtc_time.min != old_min)
{
    draw_hand(old_min_angle, 75, 0x0000);

    draw_hand(min_angle, 75, 0x07E0);

    old_min_angle = min_angle;

    old_min = rtc_time.min;
}

   if(rtc_time.hour != old_hour ||
   rtc_time.min  != old_min)
{
    draw_hand(old_hour_angle, 45, 0x0000);

    draw_hand(hour_angle, 45, 0xFFFF);

    old_hour_angle = hour_angle;

    old_hour = rtc_time.hour;
}
}*/