/* @file  main.c
*  @brief This is an implementation of a Moore finite state machine that
*         simulates the behavior of a traffic light. The traffic lights can
*         be thought of as a system of two traffic lights on two roads going
*         east and north that meet at a junction.
*         The microcontroller is configured as:
*         - east facing red light connected to PB5
*         - east facing yellow light connected to PB4
*         - east facing green light connected to PB3
*         - north facing red light connected to PB2
*         - north facing yellow light connected to PB1
*         - north facing green light connected to PB0
*         - north facing car detector connected to PE1 (1=car present)
*         - east facing car detector connected to PE0 (1=car present)
*  @author Daniel Valvano, Jonathan Valvano (instructors of the course)
*          Mustafa Siddiqui (modularized the code in an attempt to understand it)
*  @date  07/23/2020
*/

#include "PLL.h"
#include "SysTick.h"

/* Define PortB registers
   Note: LIGHT and SENSOR are defined by use of bit-specific addressing
*/
#define LIGHT                   (*((volatile unsigned long*)0x400050FC))
#define GPIO_PORTB_OUT          (*((volatile unsigned long*)0x400050FC)) // bits 5-0
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long*)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long*)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long*)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long*)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long*)0x4000552C))
#define GPIO_PORTE_IN           (*((volatile unsigned long*)0x4002400C)) // bits 1-0
#define SENSOR                  (*((volatile unsigned long*)0x4002400C))

/* Define Port E registers */
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long*)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long*)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long*)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long*)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long*)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long*)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control
#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control

/* MACROs to improve readability */
#define goN   0
#define waitN 1
#define goE   2
#define waitE 3

/* @fn    PortE_Init(void)
*  @brief Function to initialize registers of Port E.
*/
void PortE_Init(void);

/* @fn    PortB_Init(void)
*  @brief Function to initialize register of Port B.
*/
void PortB_Init(void);

/* Linked data structure to store FSM data */
struct State {
  unsigned long Out; 
  unsigned long Time;  
  unsigned long Next[4];
} typedef STyp;

/* Initialize FSM */
STyp FSM[4]={
  {0x21, 3000, {goN, waitN, goN, waitN}}, 
  {0x22, 500, {goE, goE, goE, goE}},
  {0x0C, 3000, {goE, goE, waitE, waitE}},
  {0x14, 500, {goN, goN, goN, goN}}
 };

/* Index to the current state */
unsigned long S;
unsigned long Input; 

int main(void) { 
  volatile unsigned long delay;

  // initialize PLL at 80 Hz
  PLL_Init();
  SysTick_Init();

  // inputs on PortE, outputs on PortB
  PortE_Init();
  PortB_Init();

  // initial state
  S = goN;  

  while(1) {

    // set lights
    LIGHT = FSM[S].Out;
    SysTick_Wait10ms(FSM[S].Time);

    // read sensors
    Input = SENSOR;
    S = FSM[S].Next[Input];  
  }
}

void PortE_Init(void) {
  SYSCTL_RCGC2_R |= 0x12;      
  delay = SYSCTL_RCGC2_R;           // no need to unlock
  GPIO_PORTE_AMSEL_R &= ~0x03;      // disable analog function on PE1-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // enable regular GPIO
  GPIO_PORTE_DIR_R &= ~0x03;        // inputs on PE1-0
  GPIO_PORTE_AFSEL_R &= ~0x03;      // regular function on PE1-0
  GPIO_PORTE_DEN_R |= 0x03;         // enable digital on PE1-0
}

void PortB_Init(void) {
  SYSCTL_RCGC2_R |= 0x12;      
  delay = SYSCTL_RCGC2_R;           // no need to unlock
  GPIO_PORTB_AMSEL_R &= ~0x3F;      // disable analog function on PB5-0
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // enable regular GPIO
  GPIO_PORTB_DIR_R |= 0x3F;         // outputs on PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F;      // regular function on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;         // enable digital on PB5-0
}
