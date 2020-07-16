#include "SysTick.h"

/* Initialize SysTick */
void SysTick_init(void) {
    NVIC_ST_CTRL_R = 0;             // disable SysTick during setup
    NVIC_ST_CTRL_R = 0x00000005;    // enable SysTick with core clock
}

/* The delay parameter is in units of the 80 MHz core clock. (12.5 ns) */
void SysTick_Wait(unsigned long delay) {
    NVIC_ST_RELOAD_R = delay - 1;                   // number of counts to wait
    NVIC_ST_CURRENT_R = 0;                          // any value written to the register clears it
    while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}   // wait for count flag (bit #4)
}

/* 800000 * 12.5 ns = 10 ms */
void SysTick_Wait10ms(unsigned long delay) {
    for (unsigned long i = 0; i < delay; i++) {
        SysTick_Wait(800000);       // wait 10 ms
    }
}