ENTRY(vectors) 

MEMORY 
{ 
    flash(RX)  : ORIGIN = 0x08000000, LENGTH = 128K 
    sram(RWX) : ORIGIN = 0x20000000, LENGTH = 20K 
}

SECTIONS 
{ 
.text : 
{ 
    *(.vectors*) 
    *(.text*) 
    E_text = . ;

} > flash 
.data :
{   
    S_data = . ;
    *(.data)
    . = ALIGN(4) ;
    E_data = . ;

}>sram AT> flash
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
} 