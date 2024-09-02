//Mastering_Embedded System online diploma 
#include <stdint.h> 
// #define STACK_Start_SP 0x20001000 
extern unsigned int stack_top; 
extern int main(void);
void Rest_Handler (void) ;

void Default_Handler()
{
    Rest_Handler();
}

void NMI_Handler(void) __attribute__((weak,alias("Default_Handler")));
void H_fault_Handler(void) __attribute__((weak,alias("Default_Handler")));
void MM_Fault_Handler(void) __attribute__((weak,alias("Default_Handler")));
void Bus_Fault(void) __attribute__((weak,alias("Default_Handler")));
void Usage_Fault_Handler(void) __attribute__((weak,alias("Default_Handler")));

uint32_t vectors[] __attribute__((section(".vectors")))   = { 
(uint32_t) &stack_top, 
(uint32_t) &Rest_Handler, 
(uint32_t) &NMI_Handler ,  
(uint32_t) &H_fault_Handler , 
(uint32_t) &MM_Fault_Handler , 
(uint32_t) &Bus_Fault , 
(uint32_t) &Usage_Fault_Handler  
}; 

extern unsigned int E_data;
extern unsigned int S_data;
extern unsigned int E_text;
extern unsigned int E_bss;
extern unsigned int S_bss;

void Rest_Handler()
{

    unsigned int DATA_size=(unsigned char*) &E_data - (unsigned char*)&S_data ;
    unsigned char* P_src = (unsigned char*) &E_text;
    unsigned char* P_dest =(unsigned char*) &S_data;

    for (int i=0 ; i<DATA_size;i++)
    {
        *((unsigned char*)P_dest++)=*((unsigned char*)P_dest++);
    }


    unsigned int BSS_size =(unsigned char*) &E_bss - (unsigned char*) &S_bss;
    P_dest=(unsigned char*) &S_bss;

    for (int i=0 ; i<BSS_size;i++)
    {
        *((unsigned char*) P_dest ) = (unsigned char )0 ;
    }


main();
}