<div align="center">
  <div>&nbsp;</div>
  <img src="assets\DALL·E.webp" width="800"/> 
</div>



# Embedded C : Build Process, Startup, Linker, and more..

 This is hands-on training repository that includes solutions to assignments from the `Learn in Depth` course by Eng. Keroles, a comprehensive program designed to deepen your understanding of embedded systems.

## Repository Structure
The repository is organized into a series of labs, each corresponding to different assignments from the "Learn in Depth" course. These labs build upon one another, introducing key concepts in embedded systems step-by-step.


- ## Lab 1: ARM VersatilePB 
    **Objectives** : 
    - Understand the build process, Develop bare-metal application that execute directly on hardware.
    - Create starup and linker script files. 
    - Learn how to use arm cross-toolchain : 
        - **compile**  : `arm-none-eabi-gcc` toolchain for compiling source code into object files.
        - **assemble** : `arm-none-eabi-as` to assemble assembly language files into object code.
        - **linking**  : `arm-none-eabi-ld` or `arm-none-eabi-gcc` for linking object files to produce executable binaries. 
        - **analysis** : tools such as `arm-none-eabi-objdump` and `arm-none-eabi-gdb` for examining and debugging the binary output..
    - Automate the build using make. 
    