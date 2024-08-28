# Introduction :
The `ARM926EJ-S` is a widely used processor core in embedded systems, known for its balance of performance, power efficiency, and versatility. It belongs to the ARM9 family and supports a variety of features that make it suitable for embedded applications, including support for the ARM and Thumb instruction sets, memory management, and DSP extensions.
### **Send data via UART0** :
To send data,write to the UART0 `UARTDR` address : `0x101f1000` offset : `0x0`.

## [startup](startup.s)
## **Purpose**:  
`startup.s`  Frist code to be executed by the processor where pc points to after power-on, It's job is to initialize the stack pointer then jump to main function.

## **Structure**:  
### **reset section** : 
    
    ldr sp,= stack_top                                  #initialize the stack pointer with stack top value
    bl main                                             #branch to main 
    

- `stack_top` value initialized in [linker_script](linker_script.ld) file. 
- main is a symbol which linker will resolve later. 

### **stop section** :
    
    stop : b stop                                        #hold processor in infinite loop after return form main 
     
## [linker script](linker_script.ld)

## Purpose
`linker_script.ld` define memory regions and allocate diffrent sections to their locations.

## Content:
 ### **Entry**
```c
ENTRY(reset) 
```
-  Entry keyword is used to define the entry point of application (in our case reset section from startup.s) by storing this info in the header of .elf file. 
-  Whether this keyword is used or not processor have entry address of execution defined in processor specs so this feature is optional but it is requried for debugging using gdb. 

 ### **Memory**
 ```c
 MEMORY
{
    Mem (rwx) : ORIGIN =0x00000000, LENGTH = 64M
}
 ```
 - define target memory map.
 - `(rwx)` : define memory acess permissions as read/write/execute. 
 - `ORIGIN`: define the start address of the region in physical memory. 

 ### **Sections** :  Defines the sections in the program, such as code, data, and bss sections, and specifies how they should be mapped to memory regions.
 ```c
  . = 0x10000 ;
 ```
- `.` is a special symbol represents the current memory address being processed by the linker, we can use it to specify specific address for specific section .
- `0x10000` is the processor entry address where we want to put startup code.

```c
  .startup  :
    {
      startup.o(.text)
    }> Mem
```
- startup section definition which include the startup.o text section and defile VMA and LMA in our case VMA=LMA for simplicity.

```c
 .text :
    {
        *(.text) *(.rodata)
    }> Mem
```
- text section definition which include all object files text section and read only data sections.
- rodata holds global const variables.


```c
 .data :
    {
        *(.data)
    }> Mem
```
- data section definition which include all object files data sections.
- data section holds global initialized, global static initialized and local static initialized variables.


```c
.bss :
    {
        *(.bss) *(COMMON)
    }> Mem
```
- bss section definition which include all object files bss sections
- bss section holds Global uninitialized, Global static uninitialized and Local static uninitialized variables.


```c
 . = . + 0x1000;                                /* allocate 4kB of memory to stack */
    stack_top = . ;
```
- stack top symbol is used by startup.s to initialize the stack pointer.
- stack region holds local initialized, local uninitialized and local const variables. 


## [Uart](uart.c)
## Purpose
- `uart.c` code implementation that transmit string using uart.
- `uart.h` header file to import uart functionality to our app.

## **Content** 

### uart.h :
```c
#ifndef _UART_H_
#define _UART_H_

void Uart_Send_String(unsigned char * P_TX_String);                 //function prototype tells the compiler about the function existence

#endif
```

- `#ifndef` , `#endif` are a preprocessor macros which prevent the inclusion of file more than once.


### uart.c :
```c
#define UART0DR *((volatile unsigned int * const )((unsigned int *)0x101f1000))
```
- define const pointer with address of uart register which responsible of transmission in uart 0.

```c
void Uart_Send_String(unsigned char* P_TX_String )
{
}
```
- definition of `Uart_Send_String` fuction which takes array of char

```c
while (*P_TX_String !='\0')
{
    UART0DR=(unsigned int) (*P_TX_String);

    P_TX_String++;
}
```
- using while to loop on `P_TX_String` array until null char index. 
- set the value of `UART0DR` data register of the char from our array.
- then increment the pointer value ( array index ).


## [App](app.c)
## Purpose :
simple app to demonstrate the build process.
```c
#include "uart.h"
unsigned char string_buffer[100]="Learn-in-depth:<nabil says hi !>";
void main(void)
{
Uart_Send_String(string_buffer);
}
```
- include uart header file. 
- define global `string_buffer` array of char and initialize it's value.
- define main fuction and pass `string_buffer` value to `Uart_Send_String` fuction.



## [Makefile](Makefile)
## Purpose
- Automate build process and make it more generic.
- specify the dependencies between source code files, object files, and executable files, as 
well as the commands to build and link the program. 

### **Content**
```
CC=arm-none-eabi-
CFLAGS=-mcpu=arm926ej-s -g
INCS= -I .
LIBS=
TARGET=lab1
```
- `CC` : specifies the compiler to use.
- `CFLAGS` : specifies the compiler flags to use.
- `INCS` :  specifies directories to include. 
- `LIBS` : specifies libs to include in build.
- `TARGET` : specifies the name of the executable file to be built. 

```c
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
As=$(wildcard *.s)
AsOBJ=$(As:.s=.o)
```

- `SRC`: specifies the c source code files for the program.
- `OBJ`: specifies the object files to be generated from the c source code files.
- `As`: specifies the assembly source code files for the program.
- `OBJ`: specifies the object files to be generated from the assembly source code files.

```c
all: $(TARGET).bin
		@echo "========== Build is Done =========="
```
- `all` : This rule specifies that the default target is debending on `$(TARGET).bin`.
- `make` command with no arguments executes the first rule in the file which is `all`.
 

```c
%.o: %.s
		$(CC)as.exe  $(CFLAGS)  $< -o $@
```
-  specifies the command to assemble the source code file into the object file. 
- `%.o` : This rule specifies that each object file depends on the corresponding source code file. 
- `$<`: represents the first prerequisite. 
- `$@` : represents the name of target object.
- `%` : act like placeholder in a for-each loop.
```c
%.o: %.c
		$(CC)gcc.exe $(CFLAGS) $(INCS) -c $< -o $@
```
-  specifies the command to compile the source code file into the object file. 

```c
$(TARGET).elf: $(OBJ) $(AsOBJ)
		$(CC)ld.exe -T linker_script.ld $(LIBS) $(OBJ) $(AsOBJ) -o $@ -Map=Map_file.map
```
- specifies the command to link the object files into the elf target file.
- `-Map` : is linker option to generate map file that details the memory map of the project when it is loaded onto the MCU. 

```c
$(TARGET).bin: $(TARGET).elf
		$(CC)objcopy.exe -O binary $< $@
```
- generate a raw binary file by using an output target of `-O binary`

```c
clean:
		del /F /Q *.o *~ $(TARGET).elf $(TARGET).bin Map_file.map
		@echo "========== Everything clean =========="
```	
- `clean`: This rule specifies the command to remove all object files and the executable file.

## Simulation :
- `qemu-system-arm` is the software that emulates a VersatilePB platform
```
qemu-system-arm -M versatilepb -m 128M -nographic  -kernel lab1.bin 
```

## Report :
### Sections of object files :
`arm-none-eabi-objdump.exe -h app.o`
```
Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         0000001c  00000000  00000000  00000034  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000064  00000000  00000000  00000050  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  00000000  00000000  000000b4  2**0
                  ALLOC
  3 .comment      00000046  00000000  00000000  000000b4  2**0
                  CONTENTS, READONLY
  4 .ARM.attributes 0000002c  00000000  00000000  000000fa  2**0
                  CONTENTS, READONLY
```
- ` File off` : The offset in the binary file where this section starts.
- `Algn` : The alignment requirement for the section `2**2` (4-byte alignment).
- `.comment` : hold comments about the generated ELF (details such as compiler version and execution platform) 
- `.ARM.attributes` section stores various attributes related to the ARM processor, such as the architecture version, instruction set, and other specific features or extensions that the code is designed to use. It also contains information about the compiler and tools used to build the code

`arm-none-eabi-objdump.exe -h uart.o`
```
Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         00000058  00000000  00000000  00000034  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000000  00000000  00000000  0000008c  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  00000000  00000000  0000008c  2**0
                  ALLOC
  3 .comment      00000046  00000000  00000000  0000008c  2**0
                  CONTENTS, READONLY
  4 .ARM.attributes 0000002c  00000000  00000000  000000d2  2**0
                  CONTENTS, READONLY
```
- The `.text` section has CONTENTS, ALLOC, LOAD, READONLY, and CODE flags : 
  - It contains executable code `CONTENTS`.
  - It must be allocated space in memory `ALLOC`.
  - It will be loaded from the file into memory `LOAD`.
  - It is read-only at runtime `READONLY`.
  - It contains executable code `CODE`.

`arm-none-eabi-objdump.exe -h startup.o`
```
Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         00000010  00000000  00000000  00000034  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000000  00000000  00000000  00000044  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  00000000  00000000  00000044  2**0
                  ALLOC
  3 .ARM.attributes 00000022  00000000  00000000  00000044  2**0
                  CONTENTS, READONLY
```
- The .bss section has only the ALLOC flag:
- It requires allocated memory space, but since it’s uninitialized data, it does not have CONTENTS or LOAD flags (it’s initialized to zero at runtime).
## elf file 

### ELF File Structure
1. ELF Header : The ELF header provides key information about the binary, such as its format, target architecture, entry point, and locations of various tables within the file.
    ```cc
    Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00   
    Class:                             ELF32
    Data:                              2's complement, little endian
    Version:                           1 (current)
    OS/ABI:                            UNIX - System V
    ABI Version:                       0
    Type:                              EXEC (Executable file)
    Machine:                           ARM
    Version:                           0x1
    Entry point address:               0x10000
    Start of program headers:          52 (bytes into file)
    Start of section headers:          4956 (bytes into file)
    Flags:                             0x5000200, Version5 EABI, soft-float ABI
    Size of this header:               52 (bytes)
    Size of program headers:           32 (bytes)
    Number of program headers:         1
    Size of section headers:           40 (bytes)
    Number of section headers:         9
    Section header string table index: 8
    ```

2. Program Header Table : Describes segments to be loaded into memory.
   ```
   Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
   LOAD           0x001000 0x00010000 0x00010000 0x000e8 0x000e8 RWE 0x1000
   ```
3. Section Headers : Contains entries for each section in the ELF file.
   ```
    Section Headers:
    [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
    [ 0]                   NULL            00000000 000000 000000 00      0   0  0
    [ 1] .startup          PROGBITS        00010000 001000 000010 00  AX  0   0  4
    [ 2] .text             PROGBITS        00010010 001010 000074 00  AX  0   0  4
    [ 3] .data             PROGBITS        00010084 001084 000064 00  WA  0   0  4
    [ 4] .ARM.attributes   ARM_ATTRIBUTES  00000000 0010e8 00002e 00      0   0  1
    [ 5] .comment          PROGBITS        00000000 001116 000045 01  MS  0   0  1
    [ 6] .symtab           SYMTAB          00000000 00115c 000160 10      7  17  4
    [ 7] .strtab           STRTAB          00000000 0012bc 000057 00      0   0  1
    [ 8] .shstrtab         STRTAB          00000000 001313 000049 00      0   0  1
   ```
   - `.data` size = 64 byte wich 100 in decimal , do you relate to `char string_buffer[100]` in our [app.c](app.c)
4. Section to Segment Mapping : describes how sections from the ELF file are mapped to segments defined in the program header table.
   ```
   Segment Sections...
   00     .startup .text .data
   ```
   - this mapping indicates that Segment 00 includes the sections .startup, .text, and .data.
   
5. Symbol Table : Contains symbols like functions and variables.
   ```
   Symbol table '.symtab' contains 22 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 00010000     0 SECTION LOCAL  DEFAULT    1 .startup
     2: 00010010     0 SECTION LOCAL  DEFAULT    2 .text
     3: 00010084     0 SECTION LOCAL  DEFAULT    3 .data
     4: 00000000     0 SECTION LOCAL  DEFAULT    4 .ARM.attributes
     5: 00000000     0 SECTION LOCAL  DEFAULT    5 .comment
     6: 00000000     0 FILE    LOCAL  DEFAULT  ABS startup.o
     7: 00010000     0 NOTYPE  LOCAL  DEFAULT    1 $a
     8: 00010008     0 NOTYPE  LOCAL  DEFAULT    1 stop
     9: 0001000c     0 NOTYPE  LOCAL  DEFAULT    1 $d
    10: 00000000     0 FILE    LOCAL  DEFAULT  ABS app.c
    11: 00010084     0 NOTYPE  LOCAL  DEFAULT    3 $d
    12: 00010010     0 NOTYPE  LOCAL  DEFAULT    2 $a
    13: 00010028     0 NOTYPE  LOCAL  DEFAULT    2 $d
    14: 00000000     0 FILE    LOCAL  DEFAULT  ABS uart.c
    15: 0001002c     0 NOTYPE  LOCAL  DEFAULT    2 $a
    16: 00010080     0 NOTYPE  LOCAL  DEFAULT    2 $d
    17: 00010000     0 NOTYPE  GLOBAL DEFAULT    1 reset
    18: 000110e8     0 NOTYPE  GLOBAL DEFAULT    3 stack_top
    19: 0001002c    88 FUNC    GLOBAL DEFAULT    2 Uart_Send_String
    20: 00010010    28 FUNC    GLOBAL DEFAULT    2 main
    21: 00010084   100 OBJECT  GLOBAL DEFAULT    3 string_buffer
   ```
   
  
6. Attribute Section : provides specific information about the target architecture and ABI (Application Binary Interface) used for the ELF file


`arm-none-eabi-objdump.exe -h lab1.elf`
```
Idx Name          Size      VMA       LMA       File off  Algn
  0 .startup      00000010  00010000  00010000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .text         00000074  00010010  00010010  00001010  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  2 .data         00000064  00010084  00010084  00001084  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  3 .ARM.attributes 0000002e  00000000  00000000  000010e8  2**0
                  CONTENTS, READONLY
  4 .comment      00000045  00000000  00000000  00001116  2**0
                  CONTENTS, READONLY
```
- The `.data` section has CONTENTS, ALLOC, LOAD, and DATA flags:

- It contains initialized data `CONTENTS`.
- It must be allocated space in memory `ALLOC`.
- It will be loaded from the file into memory `LOAD`.
- It contains data `DATA`, which may be read-write.

## symbol table 
`arm-none-eabi-nm.exe  startup.o`
```
         U main
00000000 T reset
         U stack_top
00000008 t stop
```
- The address column shows the memory address or offset where the symbol is defined.
- `00000000` means `reset` symbol is at the start of the memory space.
- `00000008` The `stop` symbol is located at this address,
- **Before Linking** : Addresses are relative.
- `U` : The symbol is undefined in this file.
- `T` : The symbol is in the text section and is globally visible, remeber we have defined reset as global `.globl reset`.
- `t` : The symbol is in the text section and local. 


`arm-none-eabi-nm.exe  app.o`
```
00000000 T main
00000000 D string_buffer
         U Uart_Send_String
```
- `D` : The symbol is in the data section.

`arm-none-eabi-nm.exe  uart.o `
```
00000000 T Uart_Send_String
```

`arm-none-eabi-nm.exe  lab1.elf`
```
00010010 T main
00010000 T reset
000110e8 D stack_top
00010008 t stop
00010084 D string_buffer
0001002c T Uart_Send_String
```
- **After Linking**: Addresses become absolute, reflecting their locations in memory.