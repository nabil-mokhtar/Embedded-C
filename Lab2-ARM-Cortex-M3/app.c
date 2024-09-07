typedef volatile unsigned int vuint32_t ; 
#include <stdint.h> 
// register address 
#define RCC_BASE      0x40021000 
#define GPIOA_BASE    0x40010800 
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18) 
#define GPIOA_CRH     *(volatile uint32_t *)(GPIOA_BASE + 0x04) 
#define GPIOA_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x0C) 
// bit fields 
#define RCC_IOPAEN   (1<<2) 
#define GPIOA13      (1UL<<10) 


extern void NMI_Handler(void)  // overrides NMI_Handler that have weak attribute on startup 
{ 
} 

 
extern void Bus_Fault(void) 
{ 
} 

typedef union  { 
    vuint32_t     all_fields; 
    struct { 
        vuint32_t    reserved:10 ; 
        vuint32_t    P_10:1 ; 
    } Pin;
} R_ODR_t;

volatile R_ODR_t*  R_ODR =   (volatile R_ODR_t*)(GPIOA_BASE + 0x0C) ;
unsigned char g_variable[3] = {1,2,3}; 



int main(void) 
{ 
RCC_APB2ENR |= RCC_IOPAEN; 
GPIOA_CRH   &= 0xFFFFF0FF; 
GPIOA_CRH   |= 0x00000200; 
while(1) 
{
    R_ODR->Pin.P_10 =  1; 
    for (int i = 0; i < 500000; i++); // arbitrary delay 
    R_ODR->Pin.P_10 = 0; 
    for (int i = 0; i < 500000; i++); // arbitrary delay  
} 
} 
