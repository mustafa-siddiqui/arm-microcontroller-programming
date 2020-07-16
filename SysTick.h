/** @file   SysTick.h
 *  @brief  Header file for functions related to the SysTick timer present
 *          on Cortex M microcontrollers.
 *  @author Mustafa Siddiqui
 *  @date   07/15/2020
 */

#ifndef SYSTICK_H
#define SYSTICK_H

/* Define registers */
#define NVIC_ST_CTRL_R      (*((volatile unsigned long*)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long*)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long*)0xE000E018))

/** @fn     SysTick_init(void)
 *  @brief  Initializes SysTick to be used with the system clock.
 *  @param  NULL
 *  @return NULL
 */
void SysTick_init(void);

/** @fn     SysTick_Wait(unsigned long)
 *  @brief  Given that the system is running on an 80 MHz clock, this function
 *          causes a delay of 12.5 ns.
 *  @param  Number of bus cycles to wait.
 *  @return NULL
 */
void SysTick_Wait(unsigned long delay);

/** @fn     SysTick_Wait10ms(unsigned long)
 *  @brief  This function causes a delay of 10 ms. It makes use of SysTick_Wait().
 *  @param  The number of '10 ms' to be delayed.
 *  @return NULL
 */
void SysTick_Wait10ms(unsigned long delay);

#endif