#include "w25q64.h"
#include "gc9a01.h"
#include "stm32f1xx.h"
#include <stdint.h>
#include "spi1.h"

void w25q64_write_enable(void)
{
    while(SPI1->SR & SPI_SR_BSY);

    FLASH_CS_LOW();

    *((uint8_t*)&SPI1->DR) = 0x06; // Write Enable command

    while(SPI1->SR & SPI_SR_BSY);

    FLASH_CS_HIGH();
}

void w25q64_wait_until_not_busy(void)
{
    FLASH_CS_LOW();

    *((uint8_t*)&SPI1->DR) = 0x05; // Read Status Register command

    while(SPI1->SR & SPI_SR_BSY);

    uint8_t status = *((uint8_t*)&SPI1->DR);

    FLASH_CS_HIGH();

    while(status & 0x01) // Check if the busy bit is set
    {
        FLASH_CS_LOW();

        *((uint8_t*)&SPI1->DR) = 0x05; // Read Status Register command

        while(SPI1->SR & SPI_SR_BSY);

        status = *((uint8_t*)&SPI1->DR);

        FLASH_CS_HIGH();
    }
}

void w25q64_erase_sector( uint32_t address)
{
    w25q64_write_enable();

    FLASH_CS_LOW();

    *((uint8_t*)&SPI1->DR) = 0x20; // Sector Erase command

    while(SPI1->SR & SPI_SR_BSY);

    // Send 24-bit address
    *((uint8_t*)&SPI1->DR) = (address >> 16) & 0xFF;
    while(SPI1->SR & SPI_SR_BSY);
    *((uint8_t*)&SPI1->DR) = (address >> 8) & 0xFF;
    while(SPI1->SR & SPI_SR_BSY);
    *((uint8_t*)&SPI1->DR) = address & 0xFF;

    while(SPI1->SR & SPI_SR_BSY);

    FLASH_CS_HIGH();

    w25q64_wait_until_not_busy();
}

void w25q64_write_page( uint32_t address, uint8_t* data, uint32_t length)
{
    w25q64_write_enable();

    FLASH_CS_LOW();

    *((uint8_t*)&SPI1->DR) = 0x02; // Page Program command

    while(SPI1->SR & SPI_SR_BSY);

    // Send 24-bit address
    *((uint8_t*)&SPI1->DR) = (address >> 16) & 0xFF;
    while(SPI1->SR & SPI_SR_BSY);
    *((uint8_t*)&SPI1->DR) = (address >> 8) & 0xFF;
    while(SPI1->SR & SPI_SR_BSY);
    *((uint8_t*)&SPI1->DR) = address & 0xFF;

    while(SPI1->SR & SPI_SR_BSY);

    // Send data bytes
    for(uint32_t i=0; i<length; i++)
    {
        *((uint8_t*)&SPI1->DR) = data[i];
        while(SPI1->SR & SPI_SR_BSY);
    }

    FLASH_CS_HIGH();

    w25q64_wait_until_not_busy();
}

void w25q64_read_data(uint32_t address, uint8_t* buffer, uint32_t length)
{
    FLASH_CS_LOW();

    *((uint8_t*)&SPI1->DR) = 0x03; // Read Data command

    while(SPI1->SR & SPI_SR_BSY);

    // Send 24-bit address
    *((uint8_t*)&SPI1->DR) = (address >> 16) & 0xFF;
    while(SPI1->SR & SPI_SR_BSY);
    *(( uint8_t*)&SPI1->DR) = (address >> 8) & 0xFF;
    while(SPI1->SR & SPI_SR_BSY);
    *(( uint8_t*)&SPI1->DR) = address & 0xFF;

    while(SPI1->SR & SPI_SR_BSY);

    // Read data bytes
    for(uint32_t i=0; i<length; i++)
    {
        while(!(SPI1->SR & SPI_SR_RXNE)); // Wait until data is received
        buffer[i] = *(( uint8_t*)&SPI1->DR);
    }

    FLASH_CS_HIGH();
}

void w25q64_read_id( uint8_t* manufacturer_id,  uint8_t* device_id)
{
    FLASH_CS_LOW();

    *(( uint8_t*)&SPI1->DR) = 0x90; // Read ID command

    while(SPI1->SR & SPI_SR_BSY);

    // Send dummy bytes
    *(( uint8_t*)&SPI1->DR) = 0x00;
    while(SPI1->SR & SPI_SR_BSY);
    *(( uint8_t*)&SPI1->DR) = 0x00;
    while(SPI1->SR & SPI_SR_BSY);
    *(( uint8_t*)&SPI1->DR) = 0x00;
    while(SPI1->SR & SPI_SR_BSY);

    // Read manufacturer and device ID
    while(!(SPI1->SR & SPI_SR_RXNE));
    *manufacturer_id = *(( uint8_t*)&SPI1->DR);
    while(!(SPI1->SR & SPI_SR_RXNE));
    *device_id = *(( uint8_t*)&SPI1->DR);

    FLASH_CS_HIGH();
}

void w25q64_read_jedec_id(uint8_t* manufacturer_id,
                          uint8_t* memory_type,
                          uint8_t* capacity)
{
    TFT_CS_HIGH();
    FLASH_CS_HIGH();
    FLASH_CS_LOW();

    spi1_transfer(0x9F);

    *manufacturer_id = spi1_transfer(0xFF);
    *memory_type     = spi1_transfer(0xFF);
    *capacity        = spi1_transfer(0xFF);

    FLASH_CS_HIGH();
}

void w25q64_read_unique_id( uint8_t* unique_id)
{
    FLASH_CS_LOW();

    *(( uint8_t*)&SPI1->DR) = 0x4B; // Read Unique ID command

    while(SPI1->SR & SPI_SR_BSY);

    // Send dummy bytes
    for(int i=0; i<4; i++)
    {
        *(( uint8_t*)&SPI1->DR) = 0x00;
        while(SPI1->SR & SPI_SR_BSY);
    }

    // Read 8-byte unique ID
    for(int i=0; i<8; i++)
    {
        while(!(SPI1->SR & SPI_SR_RXNE));
        unique_id[i] = *(( uint8_t*)&SPI1->DR);
    }

    FLASH_CS_HIGH();
}