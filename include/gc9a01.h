#ifndef GC9A01_H
#define GC9A01_H

#include "stm32f1xx.h"
#include <stdint.h>

#define TFT_WIDTH   240
#define TFT_HEIGHT  240

#define TFT_CS_GPIO_Port GPIOA
#define TFT_CS_Pin GPIO_PIN_4

#define TFT_DC_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_3

#define TFT_RST_GPIO_Port GPIOA
#define TFT_RST_Pin GPIO_PIN_2


#define TFT_CS_LOW()   TFT_CS_GPIO_Port->BRR  = TFT_CS_Pin
#define TFT_CS_HIGH()  TFT_CS_GPIO_Port->BSRR = TFT_CS_Pin

#define TFT_DC_LOW()   TFT_DC_GPIO_Port->BRR  = TFT_DC_Pin
#define TFT_DC_HIGH()  TFT_DC_GPIO_Port->BSRR = TFT_DC_Pin

#define TFT_RST_LOW()  TFT_RST_GPIO_Port->BRR  = TFT_RST_Pin
#define TFT_RST_HIGH() TFT_RST_GPIO_Port->BSRR = TFT_RST_Pin

void gc9a01_write_cmd(uint8_t cmd);

void gc9a01_write_data(uint8_t data);

void gc9a01_init(void);

void gc9a01_fill(uint16_t color);

void gc9a01_fill_dma(uint8_t* buffer, uint32_t size);

void gc9a01_draw_pixel(int x, int y, uint16_t color);

void gc9a01_draw_line(int x0,
                      int y0,
                      int x1,
                      int y1,
                      uint16_t color);

void gc9a01_draw_circle(int xc,
                        int yc,
                        int r,
                        uint16_t color);

#endif