/**	@file	main.c
 * 	@brief	This code works as a simulator for a simple heart pacemaker.
 * 			It runs on the ARM TM4C123 LaunchPad and was written using the
 * 			Kiel µVision IDE version 4.74.
 * 			
 * 			The input from switch 1 on the launch pad acts as an atrial sensor (AS)
 * 			on a pacemaker. Output to the green LED on the pad is Ready and is used 
 * 			for debugging and does not exist on an actual pacemaker. Output to the
 * 			red LED acts as a ventricular trigger (VT). 
 * 
 * 			The program begins by setting Ready as high and waiting for the switch 
 * 			to be pressed. When it is pressed, it clears Ready (set as low), and 
 * 			waits for the switch to be released. When it is released, it waits for 
 * 			250 ms (simulates the time between atrial and ventricular contraction)
 * 			and sets VT as high which will pulse the ventricles. It then waits for
 * 			another 250 ms and then clears VT (set as low).
 * 	@author	Mustafa Siddiqui
 * 	@date	06/26/20
 */

/* Define ports */
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

/**	@fn		void PortF_Init(void)
 * 	@brief	Initializes Port F on the microcontroller to allow digital function and sets 
 * 			the inputs (PF4 - SW1) and outputs (PF1 - VT - and PF3 - Ready). 
 * 			Input is SW1 and Outputs are the LEDs on the launchpad.
 */
void PortF_Init(void);

/**	@fn		void Delay1ms(unsigned long)
 * 	@brief	A function to cause delay for approximately some milliseconds. It assumes an 80 MHz clock.
 * 	@param	The number of milliseconds to be delayed for.
 */
void Delay1ms(unsigned long msec);

/**	@fn		void WaitForASLow(void)
 * 	@brief 	This functions reads the AS input and waits for the signal to be low. If it is
 * 			low, it returns. If not, it waits and calls itself (recursive function) until it
 * 			detects the signal to be low.
 */
void WaitForASLow(void);

/**	@fn		void WaitForASHigh(void)
 * 	@brief 	This functions reads the AS input and waits for the signal to be high. If it is
 * 			high, it returns. If not, it waits and calls itself (recursive function) until it
 * 			detects the signal to be high.
 */
void WaitForASHigh(void);

/**	@fn		void SetVT(void)
 * 	@brief	This functions sets VT - PF1 - high. It does not affect the other bits in the port.
 */
void SetVT(void);

/**	@fn		void ClearVT(void)
 * 	@brief	This function clears VT - PF1. It does not affect the other bits in the port.
 */
void ClearVT(void);

/**	@fn		void SetReady(void)
 * 	@brief	This functions sets Ready - PF3 - high. It does not affect the other bits in the port.
 */
void SetReady(void);

/**	@fn		void ClearReady(void)
 * 	@brief	This function clears Ready - PF3. It does not affect the other bits in the port.
 */
void ClearReady(void);

/**	@fn		main()
 * 	@brief 	Main function of the program which simulates the working of a heart pacemaker.
 * 			Sets 'Ready' as high initially and then waits for the input (SW1) to go low
 * 			because of negative logic (switch is being pressed). Delays for 10 ms to allow
 * 			any bouncing in the switch to be eliminated. It then clears 'Ready' and waits 
 * 			for the input(SW1) to be high (switch being released). When it goes high, it
 * 			waits for 250 ms and then sets 'VT' for 250 ms. This process is repeated over.
 * 	@return	An integer when successfully run.
 */ 
int main(void){

	// initialize port F
	PortF_Init();  
	while(1) {

		// ready signal goes high
		SetReady();

		// wait for switch to be pressed
		WaitForASLow();

		// ready signal goes low
		ClearReady();

		// delay for 10 ms
		Delay1ms(10);

		// wait for switch to be released
		WaitForASHigh();

		// delay for 250 ms
		Delay1ms(250);

		// VT signal goes high
		SetVT();
		Delay1ms(250);

		// VT signals goes low
		ClearVT();
  }
}

/* Initialize port F */
void PortF_Init(void) { 
	volatile unsigned long delay;
  	SYSCTL_RCGC2_R |= 0x00000020;      // F clock
  	delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize     
  	GPIO_PORTF_AMSEL_R &= 0x00;        // disable analog function
  	GPIO_PORTF_PCTL_R &= 0x00000000;   // GPIO clear bit PCTL  
  	GPIO_PORTF_DIR_R &= ~0x10;         // PF4 input
  	GPIO_PORTF_DIR_R |= 0x0E;          // PF3,2,1 output  
  	GPIO_PORTF_AFSEL_R &= 0x00;        // no alternate function
  	GPIO_PORTF_PUR_R |= 0x10;          // enable pullup resistor on PF4       
  	GPIO_PORTF_DEN_R |= 0x1E;          // enable digital pins PF4-PF1
}

/* Wait for AS to be low */
void WaitForASLow(void){
	// PF4 denotes AS
	unsigned long AS = (GPIO_PORTF_DATA_R & 0x10) >> 4;
	if (AS == 0) {
		return;
	} else {
		Delay1ms(10);
		WaitForASLow();
	}
}

/* Wait for AS to be high */
void WaitForASHigh(void){
	// PF4 denotes AS
	unsigned long AS = (GPIO_PORTF_DATA_R & 0x10) >> 4;
	if (AS == 1) {
		return;
	} else {
		Delay1ms(10);
		WaitForASHigh();
	}
}

/* Set VT */
void SetVT(void){
	// PF1 dentoes VT
	GPIO_PORTF_DATA_R |= 0x2;
}

/* Clear VT */
void ClearVT(void){
	// PF1 denotes VT
	GPIO_PORTF_DATA_R &= ~0x2;
}

/* Set Ready */
void SetReady(void){
	// PF3 denotes Ready
	GPIO_PORTF_DATA_R |= 0x8;
}

/* Clear Ready */
void ClearReady(void){
	// PF3 denotes Ready
	GPIO_PORTF_DATA_R &= ~0x8;
}

/* Delay function */
void Delay1ms(unsigned long msec){
	unsigned long count;
	while (msec > 0) {
		count = 14333;
		while (count > 0) {
			count--;
		}
		msec--;
	}
}

