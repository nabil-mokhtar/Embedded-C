# [startup](startup.s)

## **Purpose**:  
`startup.s`  Frist code to be executed by the processor where pc points to after power-on, It's job is to initialize the stack pointer then jump to main function.

## **Structure**:  
### **reset section** : 
    ```assmebly
    ldr sp,= stack_top                                  #initialize the stack pointer with stack top value
    bl main                                             #branch to main 
    ``` 
    - `stack_top` value initialized in [linker_script](linker_script.ld) file. 
    - main is a symbol which linker will resolve later. 

### **stop section** :
    ```assembly     
    stop : b stop                                        #hold processor in infinite loop after return form main 
    ``` 
# [linker script](linker_script.ld)

## Purpose
`linker_script.ld` define memory regions and allocate diffrent sections to their locations.

## Content:
 ### **Entry**
```
ENTRY(reset) 
```
-  Entry keyword is used to define the entry point of application (in our case reset section from startup.s) by storing this info in the header of .elf file. 
-  Whether this keyword is used or not processor have entry address of execution defined in processor specs so this feature is optional but it is requried for debugging using gdb. 

 ### **Memory**
 ```
 MEMORY
{
    Mem (rwx) : ORIGIN =0x00000000, LENGTH = 64M
}
 ```
 - define target memory map.
 - `(rwx)` : define memory acess permissions as read/write/execute. 
 - `ORIGIN`: define the start address of the region in physical memory. 

 ### **Sections** :  Defines the sections in the program, such as code, data, and bss sections, and specifies how they should be mapped to memory regions.
 ```
  . = 0x10000 ;
 ```
- `.` is a special symbol represents the current memory address being processed by the linker, we can use it to specify specific address for specific section .
- `0x10000` is the processor entry address where we want to put startup code.

```
  .startup  :
    {
      startup.o(.text)
    }> Mem
```
- startup section definition which include the startup.o text section and defile VMA and LMA in our case VMA=LMA for simplicity.

```
 .text :
    {
        *(.text) *(.rodata)
    }> Mem
```
- text section definition which include all object files text section and read only data sections.
- rodata holds global const variables.


```
 .data :
    {
        *(.data)
    }> Mem
```
- data section definition which include all object files data sections.
- data section holds global initialized, global static initialized and local static initialized variables.


```
.bss :
    {
        *(.bss) *(COMMON)
    }> Mem
```
- bss section definition which include all object files bss sections
- bss section holds Global uninitialized, Global static uninitialized and Local static uninitialized variables.


```
 . = . + 0x1000;  /* allocate 4kB of memory to stack */
    stack_top = . ;
```
- stack top symbol is used by startup.s to initialize the stack pointer.
- stack region holds local initialized, local uninitialized and local const variables. 

