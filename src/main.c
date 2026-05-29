#include "stm32f1xx.h"
#include "gc9a01.h"
#include "rtc.h"
#include "clock.h"
#include "flash.h"
#include "uart.h"
#include "rtc_init.h"
#include "dma_spi1_init.h"
#include "w25q64.h"

void clock_init(void);
void gpio_init(void);
void spi1_init(void);
void uart1_init(void);
void rtc_init(void);
void init_cpu_freq(void);

uint8_t ram_buffer[480];
uint8_t jedec_id[3];

static void delay(volatile uint32_t t)
{
    while(t--);
}

int main(void)
{
    init_cpu_freq();

    clock_init();

    gpio_init();

    spi1_init();

    dma_spi1_init();

    uart1_init();

    rtc_init();

    gc9a01_init();

    for(int i=0;i<480;i+=2)
    {
    ram_buffer[i]   = 0x00;
    ram_buffer[i+1] = 0x00;
    }

    gc9a01_fill_dma(ram_buffer, 240 * 2);

    init_clock_face();

    rtc_set_time(12,0,0);

    w25q64_read_jedec_id(&jedec_id[0], &jedec_id[1], &jedec_id[2]);

    if (jedec_id[0] == 28){
              gc9a01_write_cmd(0x29); // Display ON command
    }
    else{
              gc9a01_write_cmd(0x28); // Display OFF command
    }
    
    while(1)
    {

        rtc_update_from_hw();

        clock_draw();

        if (save_request == 1)
         {
            flash_unlock();
            flash_erase_page(0x0800FC00);
            flash_write_halfword(0x0800FC00, prll_u16);
            flash_lock();
            rtc_prll(prll_u16);

            save_request = 0;
        }
       
        delay(100);
    }
}