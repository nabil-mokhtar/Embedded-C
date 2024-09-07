#include <stdint.h> 

extern int main(void);
void Rest_Handler (void) ;

static unsigned long stack_top[256];

void Default_Handler()
{
    Rest_Handler();
}

void NMI_Handler(void) __attribute__((weak,alias("Default_Handler")));
void H_fault_Handler(void) __attribute__((weak,alias("Default_Handler")));

void (* const vector_functions[]) () __attribute__((section(".vectors"))) =
{
(void (*)()) ((unsigned long)stack_top+sizeof(stack_top)),
&Rest_Handler, 
&NMI_Handler,  
&H_fault_Handler
};


extern unsigned char E_data[];
extern unsigned char S_data[];
extern unsigned char E_text[];
extern unsigned char E_bss[];
extern unsigned char S_bss[];

void Rest_Handler()
{

    unsigned int DATA_size= E_data - S_data ;
    unsigned char* P_src =  E_text;
    unsigned char* P_dest = S_data;

    for (int i=0 ; i<DATA_size;i++)
    {
        *(P_dest++)=*(P_dest++);
    }


    unsigned int BSS_size = E_bss - S_bss;
    P_dest= S_bss;

    for (int i=0 ; i<BSS_size;i++)
    {
        *( P_dest ) = (unsigned char ) 0 ;
    }


    main();
}