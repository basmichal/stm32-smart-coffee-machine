# STM32 Smart Coffee Machine Controller

Bare metal embedded smart coffee machine controller based on STM32F103.

The project focuses on low-level embedded development without HAL libraries.

## Features

* GC9A01 round LCD support
* SPI1 DMA accelerated rendering
* RTC analog clock
* UART communication
* External W25Q64 SPI Flash support
* Bluetooth SPP communication
* Coffee machine automation

## Hardware

* STM32F103C8T6
* GC9A01 round TFT LCD
* W25Q64 SPI Flash
* Bluetooth UART module
* Relay modules

## Current Progress

## Current Progress

### Stable / Working

* [x] STM32F103 bare metal project
* [x] SPI1 driver
* [x] SPI1 DMA accelerated LCD fill
* [x] GC9A01 round LCD support
* [x] RTC support
* [x] Analog clock rendering
* [x] UART command parser
* [x] Bluetooth SPP communication (UART 115200)
* [x] RTC calibration
* [x] Shared SPI bus architecture
* [x] W25Q64 JEDEC ID read
* [x] W25Q64 status register read
* [x] Shared SPI bus communication verified (GC9A01 + W25Q64)

### Work In Progress

* [ ] W25Q64 SPI Flash driver
* [ ] DMA accelerated graphics primitives
* [ ] DMA driven bitmap rendering
* [ ] Partial screen DMA updates
* [ ] Flash sector erase
* [ ] Flash page program
* [ ] Bitmap storage system
* [ ] Bitmap streaming to LCD
* [ ] Coffee machine relay control
* [ ] Android application
* [ ] GUI animations
* [ ] ESP32 connectivity module
* [ ] WiFi / NTP synchronization


## Technical Topics

* Bare metal STM32 programming
* DMA driven SPI rendering
* RTC calibration
* Shared SPI bus
* External SPI Flash memory
* Embedded graphics pipeline
* Register-level programming
* Hardware debugging

## Architecture

STM32 handles:

* realtime control
* LCD rendering
* DMA transfers
* relay control
* RTC

ESP32 (planned) handles:

* Bluetooth
* WiFi
* NTP synchronization
* Android communication

## License

MIT License
