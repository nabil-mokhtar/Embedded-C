<div align="center">
  <div>&nbsp;</div>
  <img src="assets\DALL·E.jpg" width="600" /> 
</div>



# Embedded C : Build Process, Startup, Linker, and more..

 This is hands-on training repository that includes solutions to assignments from the [Learn in Depth](https://www.learn-in-depth-store.com/) course by Eng. Keroles, a comprehensive program designed to deepen your understanding of embedded systems.

## Repository Structure
The repository is organized into a series of labs, each corresponding to different assignments from the "Learn in Depth" course. These labs build upon one another, introducing key concepts in embedded systems step-by-step.


- ## Lab 1: [ARM VersatilePB](Lab1-ARM-VersatilePB) 
    **Objectives** : 
    - Understand the build process, Develop bare-metal application that execute directly on hardware.
    - Create starup and linker script files. 
    - Learn how to use arm cross-toolchain.
    - Automate the build using make. 
    

- ## Lab 2: [ARM Cortex M3 - STM32F103C8T6](Lab2-ARM-Cortex-M3) 
    **Objectives** : 
    - Define Interrupt vectors Section. 
    - Copy Data from ROM to RAM.
    - Initialize Data Area. 
    - Initialize Stack. 
    - Create a reset section and Call main.
        

- ## Lab 3: [ARM Cortex M4 - TM4C123](Lab3-ARM-Cortex-M4) 
    **Objectives** : 
    - write vectors section in professional way.
    - initialize the stack using tricky way. 



## Tools :
  - **compile**  : `arm-none-eabi-gcc` toolchain for compiling source code into object files.
  - **assemble** : `arm-none-eabi-as` to assemble assembly language files into object code.
  - **linking**  : `arm-none-eabi-ld` or `arm-none-eabi-gcc` for linking object files to produce executable binaries. 
  - **analysis** : `arm-none-eabi-objdump` for examining the binary output.
  - **debugging** :
    - server : [OpenOCD](https://openocd.org/) 
    - client : `arm-none-eabi-gdb`
  
    