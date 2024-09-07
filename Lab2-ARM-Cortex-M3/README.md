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


## Nitty gritty details for each file

- [Linker Script](#ref2)
- [Startup](#ref1)
- [App](#ref3)


<a name="ref2"></a>
## Linker script

## Content:
### **Entry** : 
`ENTRY(vectors)` define vectors section as the entry point of the application. 
### **Memory** :
```c
MEMORY 
{ 
    flash(RX)  : ORIGIN = 0x08000000, LENGTH = 128K 
    sram(RWX) : ORIGIN = 0x20000000, LENGTH = 20K 
}

```
 - define `flash` size to 128K with start address `0x08000000` and have read and execute permission. 
 - define `sram` size to 20K with start address `0x20000000` and have read, write and execute permissions.

 ### **Sections**
```c
.text : 
{ 
    *(.vectors*) 
    *(.text*) 
    E_text = . ;

} > flash 
```
- text section include vectors and text sections.
- When a symbol is declared two things happen. The first is that the compiler reserves enough space in the program’s memory to hold the value of the symbol. The second is that the compiler creates an entry in the program’s symbol table which holds the symbol’s address. ie the symbol table contains the address of the block of memory holding the symbol’s value.
- the location counter holds end address of text section in flash which saved in ` E_text`. 

```c
.data :
{   
    S_data = . ;
    *(.data)
    . = ALIGN(4) ;
    E_data = . ;

}>sram AT> flash
```
- data section include all data sections from all object files.
- start and end address of data section in `sram` (virtual address) are saved in `S_data` and `E_data`.
- `ALIGN(4)` instruction tells the linker to insert padding bytes until current location becomes aligned on 4-byte boundary: `while ((current_location & 7) != 0)
  *current_location++ = padding_value;`
- define `flash` as load memory address and `sram` as virtual memory address. 

```c
.bss  : 
{   
    S_bss = . ;
    *(.bss*)
    . = ALIGN(4) ; 
    E_bss = . ;
    . = ALIGN(4);
    . = . + 0x1000;
    stack_top = . ;
} > sram 
```
- bss section include all bss sections from all object files.
- start and end address of bss section in `sram` (virtual address) are saved in `S_bss` and `E_bss`.

<a name="ref1"></a>
## startup
### **content** :

```c
extern unsigned int stack_top[]; 
extern unsigned int E_data[];
extern unsigned int S_data[];
```
- symbol `stack_top` holds the address of stack top memory location , but nothing special is stored at `stack_top` address. This means that you cannot access the value of a linker script defined symbol - it has no value - all you can do is access the address of a linker script defined symbol using `&stack_top` or the symbols can be treated as arrays `stack_top[]`.

- what are symbols from [ld source code refrence](https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html)

```c
void NMI_Handler(void) __attribute__((weak,alias("Default_Handler")));
```
- The compiler recognizes `__attribute__` and the attributes that specified with it. These attributes modify how the compiler interprets the surrounding code, affecting how functions, variables, or types are handled.

- The weak attribute allows you to declare a symbol (function or variable) that can be optionally overridden by a stronger symbol during the linking phase.

- It’s commonly used for providing default implementations that can be replaced by stronger definitions elsewhere in our case the default is the `Default_Handler`.
- `alias("Default_Handler")` : Aliases provide a simple way to reference the `Default_Handler` function under multiple names without duplicating code or data.

```c
uint32_t vectors[] __attribute__((section(".vectors"))) = 
{
(uint32_t) stack_top, 
(uint32_t) &Rest_Handler, 
(uint32_t) &NMI_Handler 
} 
```
- `__attribute__((section()))` :  attribute allows you to place functions, variables, or data into specific sections of the output binary, instead of the default sections like .text, .data, or .bss .
- Define vectors array and place it in `vectors` section wich defined as the entry point by linker script.
- `stack_top`: takes index 0 because in arm cortex m3 the value in address `0x08000000` is reserved for stack top address which will be coppied to `SP` register, and vectors section is saved in this address, then the vector handlers.

```c
void Rest_Handler()
{
    unsigned int DATA_size = E_data - S_data ;
    unsigned char* P_src   = E_text;
    unsigned char* P_dest  = S_data;
```
- Calculate `DATA_size` from start and end addresses of data.
- Create source pointer to flash .data start address .
- Create destination pointer to sram .data start address (virtual).

```c
 for (int i=0 ; i<DATA_size;i++)
    {
        *(P_dest++)=*(P_src++);
    }

```
- Copy data from source (flash) to destination (sram).

```c
unsigned int BSS_size = E_bss -  S_bss;
P_dest= S_bss;
```
- Calculate `BSS_size` from start and end addresses of bss (virtual).
- Create destination pointer to sram .bss start address.

```c
 for (int i=0 ; i<BSS_size;i++)
    {
        *( P_dest ) = (unsigned char) 0 ;
    }
```
- Initialize bss section in sram with zeros.


```c
 main();
```
- Finally jump to main.
