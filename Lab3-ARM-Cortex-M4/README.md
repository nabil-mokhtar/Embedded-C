# Introduction

In this lab we will practice using `TM4C123` microcontroller :

- modify vectors section from last lab and use bss section to reserve stack region. 
- use OpenOCD to debug.


## Startup

**Recap** 
- from lab 2 we used vectors array to define `.vectors` section, which each handler have entry in array.
- `stack_top` is a sambol defined in linker script.
```c
uint32_t vectors[] __attribute__((section(".vectors"))) = 
{
(uint32_t) stack_top, 
(uint32_t) &Rest_Handler, 
(uint32_t) &NMI_Handler 
} 
```
**Changes**

using uninitialized global array to define stack top automatic in bss section. 
```c
static unsigned long stack_top[256];
```
- 256*4 = 1024 which is 1KB stack size. 

lets change the array of unsigned int32 to array of constant pointers to void functions, and cast `stack_top` (address) to pointer to void function `(void (*)())`

```c
void (* const vector_functions[]) () __attribute__((section(".vectors"))) =
{
(void (*)()) ((unsigned long)stack_top+sizeof(stack_top)),
&Rest_Handler, 
&NMI_Handler,  
&H_fault_Handler
};
```






