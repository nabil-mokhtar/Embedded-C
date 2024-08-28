# Introduction

The `STM32F103C8T6` is a popular microcontroller from the STM32F1 series, produced by STMicroelectronics. It's widely used in embedded systems due to its balance of features, performance, and cost.
<!-- 
**Toggle led** 
-   Led is connected to GPIO port C13 
-   To make a GPIO toggling in STM32, you need to work with two peripherals: 
    -   RCC (reset and clock control). 
    -   GPIOx (general purpose input/output). -->


**Boot sequence**
- when power is on the pc register holds the address of `0x08000000` which is the enty point of the processor.
- The value in address `0x08000000` is reserved for stack top address which will be coppied to `SP` register.
- the next address pc will point to `0x08000004` expected to be the address of reset handler.

**Memories**
- 64 or 128 Kbytes of Flash memory
- 20 Kbytes of SRAM
