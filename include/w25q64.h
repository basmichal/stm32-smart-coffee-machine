#ifndef W25Q64_H
#define W25Q64_H

#include "stm32f1xx.h"
#include <stdint.h>

#define FLASH_CS_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_1

#define FLASH_CS_LOW()  FLASH_CS_GPIO_Port->BRR  = FLASH_CS_Pin
#define FLASH_CS_HIGH() FLASH_CS_GPIO_Port->BSRR = FLASH_CS_Pin

void w25q64_write_enable(void);

void w25q64_wait_until_not_busy(void);

void w25q64_read_data( uint32_t address,  uint8_t* buffer, uint32_t length);

void w25q64_write_page( uint32_t address,  uint8_t* data, uint32_t length);

void w25q64_erase_sector( uint32_t address);

void w25q64_erase_chip(void);

void w25q64_read_id( uint8_t* manufacturer_id,  uint8_t* device_id);

void w25q64_read_jedec_id( uint8_t* manufacturer_id,  uint8_t* memory_type,  uint8_t* capacity);

void w25q64_read_unique_id( uint8_t* unique_id);


#endif