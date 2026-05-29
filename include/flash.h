#ifndef FLASH_H
#define FLASH_H
#include <stdint.h>

void flash_init(void);
void flash_unlock(void);
void flash_lock(void);
void flash_erase_page(uint32_t page_address);
void flash_write_halfword(uint32_t address, uint16_t data);
void flash_write_word(uint32_t address, uint32_t data);
void flash_read_halfword(uint32_t address, uint16_t* data);
void flash_read_word(uint32_t address, uint32_t* data);

#endif // FLASH_H