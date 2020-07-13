/** @file   main.c
 *  @brief  This program is flashes the LED on the TM4C123 Launchpad
 *          at 10 Hz when either of the two switches are pressed. The
 *          main purpose of writing this program was to learn functional
 *          debugging and learn how to record data. The program dumps 
 *          recorded I/O data into a buffer. The data includes the time
 *          between when the switches are pressed or when the LED turns
 *          on/off. The time elapsed is calculated by using the SysTick
 *          timer present in all Cortex M microcontrollers.
 *          NOTE: Functions for Port F and SysTick initialization were
 *          provided as part of the lab.
 *  @author Mustafa Siddiqui, and Jon Valvano & Ramesh Yerraballi (instructors)
 *  @date   07/12/2020
 */

/* Global Variables */
// first data point is wrong, the other 49 will be correct
unsigned long Time[50];
unsigned long Data[50];
unsigned long Led;

/* Define ports */
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long*) 0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long*) 0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long*) 0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long*) 0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long*) 0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long*) 0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long*) 0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long*) 0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long*) 0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long*) 0x400FE108))

/* Define SysTick registers */
#define NVIC_ST_CTRL_R          (*((volatile unsigned long*) 0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long*) 0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long*) 0xE000E018))

/** @fn     PortF_Init(void)
 *  @brief  This functions initializes Port F on the launchpad such that
 *          PF4 & PF0 are inputs (switches) and PF3-1 are outputs (LED).
 *  @return NULL
 */
void PortF_Init(void);

/** @fn     SysTick_Init(void)
 *  @brief  This function initializes SysTick with busy wait running
 *          at bus clock.
 *  @return NULL
 */
void SysTick_Init(void);

/** @fn     Delay(void)
 *  @brief  A function which causes a delay of 0.05 sec.
 *          51 ms on the Kiel µVision IDE simulator/debugger to be exact.
 *  @return NULL
 */
void Delay(void);

/** @fn     main(void)
 *  @brief  This is the main function of the program. It initializes Port F
 *          and SysTick by calling the functions declared above. In an
 *          infinite loop, it checks if either or both the switches are pressed
 *          (negative logic) and flashes the LED at 10 Hz - meaning it turns on
 *          for 0.05 seconds and turns off for 0.05 seconds. 
 *          Recording Debugging Data:
 *          If there is a difference in the previous and current values of PF0, 
 *          PF1, or PF4 - meaning if either of the switches are pressed/released
 *          and the LED turned on/off - the time and value of this bits will be
 *          recorded in the Time and Data arrays. Time lapsed is calculated using
 *          the SysTick Timer present in Cortex M microcontrollers.
 *  @return An integer when successfully run.
 */
int main(void) {  
	unsigned long i,last,now;
	unsigned long SW1, SW2;
	unsigned long prevGPIO_PORTF_DATA_R;
	
  PortF_Init();   											    // initialize PF1 to output
  SysTick_Init(); 											    // initialize SysTick, runs at 16 MHz
	
  i = 0;          											    // array index
  last = NVIC_ST_CURRENT_R;
  
	while(1) {
		SW1 = (GPIO_PORTF_DATA_R & 0x10) >> 4;	// PF4
		SW2 = (GPIO_PORTF_DATA_R & 0x1);		    // PF0
		prevGPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R & 0x13;		// store PF0, PF1, and PF4
		Led = GPIO_PORTF_DATA_R;   				      // read previous
		if ((SW1 & SW2) == 0x0) {					      // if either of the switches are pressed (negative logic)
			Led = Led^0x02;            			      // toggle red LED
		}
		else {
			Led = 0x0;
		}
    GPIO_PORTF_DATA_R = Led;   				      // output
		
		// check for change in PF0, PF1, and PF4
		if (prevGPIO_PORTF_DATA_R != (GPIO_PORTF_DATA_R & 0x13)) {
			if(i < 50) {
				now = NVIC_ST_CURRENT_R;
				Time[i] = (last-now) & 0x00FFFFFF;  // 24-bit time difference
				Data[i] = GPIO_PORTF_DATA_R & 0x13; // record PF4, PF1, and PF0
				last = now;
				i++;
			}
		}
    Delay();
  }
}

/* Port F initialization */
void PortF_Init(void) {
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;       // activate clock for Port F
  delay = SYSCTL_RCGC2_R;             // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;     // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;             // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;          // disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;     // PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;            // PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;          // disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;            // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;            // enable digital I/O on PF4-0
}

/* SysTick Timer initialization */
void SysTick_Init(void) {
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value (24 bits)
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

/* Delay of ~0.05 sec */
void Delay(void) {
  unsigned long volatile time;
  //time = 160000;  // 0.1sec
	time = 75000;			// 0.05 sec
  while (time) {
    time--;
  }
}