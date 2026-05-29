#include "stm32f1xx.h"

/* =========================================================
   FLASH
   72 MHz
========================================================= */

// flash page for stm32f103 64kb: 0x0800 F000 - 0x0801 0000
// flash last page 1kb: 0x0800 FC00 - 0x0801 0000
//#define FLASH_PAGE_SIZE 1024
//#define FLASH_START_ADDRESS 0x08000000
//#define FLASH_END_ADDRESS 0x08010000
//#define FLASH_PAGE_COUNT ((FLASH_END_ADDRESS - FLASH_START_ADDRESS) / FLASH_PAGE_SIZE)

void flash_init(void)
{
    FLASH->ACR |= FLASH_ACR_PRFTBE; // Enable flash prefetch buffer
    FLASH->ACR |= FLASH_ACR_LATENCY_2; // Set flash latency to 2 wait states (for 72MHz operation)
}

void flash_unlock(void)
{
    if(FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

void flash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

void flash_erase_page(uint32_t page_address)
{
    while(FLASH->SR & FLASH_SR_BSY); // Wait for any ongoing operation to complete

    FLASH->CR |= FLASH_CR_PER; // Enable page erase
    FLASH->AR = page_address; // Set page address
    FLASH->CR |= FLASH_CR_STRT; // Start erase operation

    while(FLASH->SR & FLASH_SR_BSY); // Wait for erase to complete

    FLASH->CR &= ~FLASH_CR_PER; // Disable page erase
}

void flash_write_halfword(uint32_t address, uint16_t data)
{
    while(FLASH->SR & FLASH_SR_BSY); // Wait for any ongoing operation to complete

    FLASH->CR |= FLASH_CR_PG; // Enable programming
    *(__IO uint16_t*)address = data; // Write half-word

    while(FLASH->SR & FLASH_SR_BSY); // Wait for programming to complete

    FLASH->CR &= ~FLASH_CR_PG; // Disable programming
}

void flash_write_word(uint32_t address, uint32_t data)
{
    while(FLASH->SR & FLASH_SR_BSY); // Wait for any ongoing operation to complete

    FLASH->CR |= FLASH_CR_PG; // Enable programming
    *(__IO uint32_t*)address = data; // Write word

    while(FLASH->SR & FLASH_SR_BSY); // Wait for programming to complete

    FLASH->CR &= ~FLASH_CR_PG; // Disable programming
}

void flash_read_halfword(uint32_t address, uint16_t* data)
{
    *data = *(__IO uint16_t*)address; // Read half-word from flash
}

void flash_read_word(uint32_t address, uint32_t* data)
{
    *data = *(__IO uint32_t*)address; // Read word from flash
}