MEMORY
{
    flash(rwx) : ORIGIN = 0x00000000 , LENGTH = 512M
    sram(rwx) : ORIGIN  = 0x20000000 , LENGTH = 512M
}

SECTIONS
{

    .text :
    {
        *(.vectors*)
        *(.text*)
        *(.rodata)
        E_text = . ;

    }>flash

    .data :
    {
        S_data = . ;
        *(.data) 
        . = ALIGN(4);
        E_data = . ;
    }>sram AT> flash

    .bss :
    {
        S_bss = . ;
        *(.bss)
        . = ALIGN(4) ;
        E_bss = . ;

    }>sram

}