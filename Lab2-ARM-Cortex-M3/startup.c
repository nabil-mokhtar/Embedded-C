#include <stdint.h> 

extern unsigned char stack_top[]; 
extern unsigned char E_data[];
extern unsigned char S_data[];
extern unsigned char E_text[];
extern unsigned char E_bss[];
extern unsigned char S_bss[];

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
(uint32_t) stack_top, 
(uint32_t) &Rest_Handler, 
(uint32_t) &NMI_Handler ,  
(uint32_t) &H_fault_Handler , 
(uint32_t) &MM_Fault_Handler , 
(uint32_t) &Bus_Fault , 
(uint32_t) &Usage_Fault_Handler  
}; 


void Rest_Handler()
{

    unsigned int DATA_size= E_data - S_data ;
    unsigned char* P_src  = E_text;
    unsigned char* P_dest = S_data;

    for (int i=0 ; i<DATA_size;i++)
    {
        *(P_dest++)=*(P_src++);
    }


    unsigned int BSS_size = E_bss -  S_bss;
    P_dest= S_bss;

    for (int i=0 ; i<BSS_size;i++)
    {
        *( P_dest ) = (unsigned char) 0 ;
    }


    main();
}