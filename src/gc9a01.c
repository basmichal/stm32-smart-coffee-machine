#include "stm32f1xx.h"
#include "gc9a01.h"
#include "dma_spi1_init.h"



static void delay(volatile uint32_t t)
{
    while(t--);
}

static void gc9a01_spi_send(uint8_t data)
{
    while(!(SPI1->SR & SPI_SR_TXE));
    *((volatile uint8_t*)&SPI1->DR) = data;

    while(SPI1->SR & SPI_SR_BSY);
}

void gc9a01_write_cmd(uint8_t cmd)
{
    TFT_DC_LOW();
    TFT_CS_LOW();

    gc9a01_spi_send(cmd);

    TFT_CS_HIGH();
}

void gc9a01_write_data(uint8_t data)
{
    TFT_DC_HIGH();
    TFT_CS_LOW();

    gc9a01_spi_send(data);

    TFT_CS_HIGH();
}

static void gc9a01_set_window(uint16_t xs,
                       uint16_t ys,
                       uint16_t xe,
                       uint16_t ye)
{
    gc9a01_write_cmd(0x2A);

    gc9a01_write_data(xs >> 8);
    gc9a01_write_data(xs);

    gc9a01_write_data(xe >> 8);
    gc9a01_write_data(xe);

    gc9a01_write_cmd(0x2B);

    gc9a01_write_data(ys >> 8);
    gc9a01_write_data(ys);

    gc9a01_write_data(ye >> 8);
    gc9a01_write_data(ye);

    gc9a01_write_cmd(0x2C);
}

void gc9a01_draw_pixel(int x, int y, uint16_t color)
{
    if(x < 0 || y < 0 || x >= 240 || y >= 240)
        return;

    gc9a01_set_window(x, y, x, y);

    TFT_DC_HIGH();
    TFT_CS_LOW();

    gc9a01_spi_send(color >> 8);
    gc9a01_spi_send(color);

    TFT_CS_HIGH();
}

void gc9a01_fill(uint16_t color)
{
    gc9a01_set_window(0,0,239,239);

    TFT_DC_HIGH();
    TFT_CS_LOW();

    for(uint32_t i=0;i<240UL*240UL;i++)
    {
        gc9a01_spi_send(color >> 8);
        gc9a01_spi_send(color);
    }

    TFT_CS_HIGH();
}

static void gc9a01_fill_dma_page(uint8_t* buffer,
                     uint32_t size)
{
    

    DMA1_Channel3->CCR &= ~DMA_CCR_EN; // Disable DMA channel before configuration

    DMA1->IFCR |= DMA_IFCR_CTCIF3; // Clear any existing transfer complete flag

    DMA1_Channel3->CMAR =
        (uint32_t)buffer; // Memory address (source)

    DMA1_Channel3->CPAR =
        (uint32_t)&SPI1->DR; // Peripheral address (destination)

    DMA1_Channel3->CNDTR = size; // Number of data items to transfer

    DMA1_Channel3->CCR |= DMA_CCR_EN; // Enable DMA channel to start transfer

    while(!(DMA1->ISR & DMA_ISR_TCIF3)); // Wait for transfer complete flag

    DMA1->IFCR |= DMA_IFCR_CTCIF3; // Clear transfer complete flag

    while(!(SPI1->SR & SPI_SR_TXE)); // Wait until SPI data register is empty (all data has been moved to shift register)

    while(SPI1->SR & SPI_SR_BSY); // Wait until SPI is not busy (all data has been transmitted)

}

void gc9a01_fill_dma(uint8_t* buffer,
                     uint32_t size)
{
    gc9a01_set_window(0,0,239,239);

    TFT_DC_HIGH();
    TFT_CS_LOW();

    for(int y = 0; y < 240; y++)
    {
        gc9a01_fill_dma_page((uint8_t*) buffer, size);
    }

    TFT_CS_HIGH();
}

void gc9a01_draw_line(int x0,
                      int y0,
                      int x1,
                      int y1,
                      uint16_t color)
{
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -((y1 > y0) ? (y1 - y0) : (y0 - y1));
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;

    while(1)
    {
        gc9a01_draw_pixel(x0, y0, color);

        if(x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if(e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if(e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void gc9a01_draw_circle(int xc,
                        int yc,
                        int r,
                        uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while(y >= x)
    {
        gc9a01_draw_pixel(xc+x, yc+y, color);
        gc9a01_draw_pixel(xc-x, yc+y, color);

        gc9a01_draw_pixel(xc+x, yc-y, color);
        gc9a01_draw_pixel(xc-x, yc-y, color);

        gc9a01_draw_pixel(xc+y, yc+x, color);
        gc9a01_draw_pixel(xc-y, yc+x, color);

        gc9a01_draw_pixel(xc+y, yc-x, color);
        gc9a01_draw_pixel(xc-y, yc-x, color);

        x++;

        if(d > 0)
        {
            y--;
            d += 4 * (x - y) + 10;
        }
        else
        {
            d += 4 * x + 6;
        }
    }
}

void gc9a01_init(void)
{
    TFT_RST_LOW();
    delay(50000);

    TFT_RST_HIGH();
    delay(50000);

    gc9a01_write_cmd(0xEF);

    gc9a01_write_cmd(0xEB);
    gc9a01_write_data(0x14);

    gc9a01_write_cmd(0xFE);
    gc9a01_write_cmd(0xEF);

    gc9a01_write_cmd(0xEB);
    gc9a01_write_data(0x14);

    gc9a01_write_cmd(0x84);
    gc9a01_write_data(0x40);

    gc9a01_write_cmd(0x85);
    gc9a01_write_data(0xFF);

    gc9a01_write_cmd(0x86);
    gc9a01_write_data(0xFF);

    gc9a01_write_cmd(0x87);
    gc9a01_write_data(0xFF);

    gc9a01_write_cmd(0x88);
    gc9a01_write_data(0x0A);

    gc9a01_write_cmd(0x89);
    gc9a01_write_data(0x21);

    gc9a01_write_cmd(0x8A);
    gc9a01_write_data(0x00);

    gc9a01_write_cmd(0x8B);
    gc9a01_write_data(0x80);

    gc9a01_write_cmd(0x8C);
    gc9a01_write_data(0x01);

    gc9a01_write_cmd(0x8D);
    gc9a01_write_data(0x01);

    gc9a01_write_cmd(0x8E);
    gc9a01_write_data(0xFF);

    gc9a01_write_cmd(0x8F);
    gc9a01_write_data(0xFF);

    gc9a01_write_cmd(0xB6);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x20);

    gc9a01_write_cmd(0x36);
    gc9a01_write_data(0x08);

    gc9a01_write_cmd(0x3A);
    gc9a01_write_data(0x05);

    gc9a01_write_cmd(0x90);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x08);

    gc9a01_write_cmd(0xBD);
    gc9a01_write_data(0x06);

    gc9a01_write_cmd(0xBC);
    gc9a01_write_data(0x00);

    gc9a01_write_cmd(0xFF);
    gc9a01_write_data(0x60);
    gc9a01_write_data(0x01);
    gc9a01_write_data(0x04);

    gc9a01_write_cmd(0xC3);
    gc9a01_write_data(0x13);

    gc9a01_write_cmd(0xC4);
    gc9a01_write_data(0x13);

    gc9a01_write_cmd(0xC9);
    gc9a01_write_data(0x22);

    gc9a01_write_cmd(0xBE);
    gc9a01_write_data(0x11);

    gc9a01_write_cmd(0xE1);
    gc9a01_write_data(0x10);
    gc9a01_write_data(0x0E);

    gc9a01_write_cmd(0xDF);
    gc9a01_write_data(0x21);
    gc9a01_write_data(0x0C);
    gc9a01_write_data(0x02);

    gc9a01_write_cmd(0xF0);
    gc9a01_write_data(0x45);
    gc9a01_write_data(0x09);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x26);
    gc9a01_write_data(0x2A);

    gc9a01_write_cmd(0xF1);
    gc9a01_write_data(0x43);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x72);
    gc9a01_write_data(0x36);
    gc9a01_write_data(0x37);
    gc9a01_write_data(0x6F);

    gc9a01_write_cmd(0xF2);
    gc9a01_write_data(0x45);
    gc9a01_write_data(0x09);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x26);
    gc9a01_write_data(0x2A);

    // Gamma settings
    gc9a01_write_cmd(0xF3);
    gc9a01_write_data(0x43);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x72);
    gc9a01_write_data(0x36);
    gc9a01_write_data(0x37);
    gc9a01_write_data(0x6F);

    //
    gc9a01_write_cmd(0xED);
    gc9a01_write_data(0x1B);
    gc9a01_write_data(0x0B);

    //
    gc9a01_write_cmd(0xAE);
    gc9a01_write_data(0x77);

    // Power setting for normal mode
    gc9a01_write_cmd(0xCD);
    gc9a01_write_data(0x63);

    // Power setting for normal mode
    gc9a01_write_cmd(0x70);
    gc9a01_write_data(0x07);
    gc9a01_write_data(0x07);
    gc9a01_write_data(0x04);
    gc9a01_write_data(0x0E);
    gc9a01_write_data(0x0F);
    gc9a01_write_data(0x09);
    gc9a01_write_data(0x07);
    gc9a01_write_data(0x08);
    gc9a01_write_data(0x03);

    // Power setting for normal mode
    gc9a01_write_cmd(0xE8);
    gc9a01_write_data(0x34);

    // Power setting for normal mode
    gc9a01_write_cmd(0x62);
    gc9a01_write_data(0x18);
    gc9a01_write_data(0x0D);
    gc9a01_write_data(0x71);
    gc9a01_write_data(0xED);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x18);
    gc9a01_write_data(0x0F);
    gc9a01_write_data(0x71);
    gc9a01_write_data(0xEF);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x70);

    // Power setting for normal mode
    gc9a01_write_cmd(0x63);
    gc9a01_write_data(0x18);
    gc9a01_write_data(0x11);
    gc9a01_write_data(0x71);
    gc9a01_write_data(0xF1);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x18);
    gc9a01_write_data(0x13);
    gc9a01_write_data(0x71);
    gc9a01_write_data(0xF3);
    gc9a01_write_data(0x70);
    gc9a01_write_data(0x70);

    // Partial display update settings
    gc9a01_write_cmd(0x64);
    gc9a01_write_data(0x28);
    gc9a01_write_data(0x29);
    gc9a01_write_data(0xF1);
    gc9a01_write_data(0x01);
    gc9a01_write_data(0xF1);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x07);

    // VCOM settings
    gc9a01_write_cmd(0x66);
    gc9a01_write_data(0x3C);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0xCD);
    gc9a01_write_data(0x67);
    gc9a01_write_data(0x45);
    gc9a01_write_data(0x45);
    gc9a01_write_data(0x10);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x00);

    // Power settings
    gc9a01_write_cmd(0x67);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x3C);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x01);
    gc9a01_write_data(0x54);
    gc9a01_write_data(0x10);
    gc9a01_write_data(0x32);
    gc9a01_write_data(0x98);


    // Gamma settings
    gc9a01_write_cmd(0x74);
    gc9a01_write_data(0x10);
    gc9a01_write_data(0x85);
    gc9a01_write_data(0x80);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x00);
    gc9a01_write_data(0x4E);
    gc9a01_write_data(0x00);

    // Gamma settings
    gc9a01_write_cmd(0x98);
    gc9a01_write_data(0x3E);
    gc9a01_write_data(0x07);

    // Exit sleep mode
    gc9a01_write_cmd(0x35);

    // Turn on display
    gc9a01_write_cmd(0x21);

    //  Delay after sleep out and display on commands is necessary for the display to be ready for data
    gc9a01_write_cmd(0x11);

    delay(500);

    // Enable tearing effect line (optional, can be used for synchronization)
 //   gc9a01_write_cmd(0x29);

    delay(500);

 //   gc9a01_fill(0x0000);
}
