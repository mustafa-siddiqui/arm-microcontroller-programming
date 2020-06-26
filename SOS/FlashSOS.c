/** @file	FlashSOS.c
 *  @brief	Program for sending an SOS signal as Morse Code
 * 		using the LED on the ARM TM4C123 LaunchPad.
 * 		The program operates as follows:
 * 		Pressing SW1 starts SOS (Green LED flashes SOS)
 * 		- S: toggle light 3 times with 1/2 sec gap between on/off
 * 		- O: toggle light 3 times with 2 sec gap between on/off
 * 		- S: toggle light 3 times with 1/2 sec gap between on/off
 * 		- 5 second delay between SOS messages
 * 		Pressing SW2 stops SOS.
 *  @author 	Mustafa Siddiqui
 *  @date 	06/25/20
 */

/* Define Ports */
#define GPIO_PORTF_DATA_R	(*((volatile unsigned long*)0x400253FC))
#define GPIO_PORTF_DIR_R	(*((volatile unsigned long*)0x40025400))
#define GPIO_PORTF_AFSEL_R	(*((volatile unsigned long*)0x40025420))
#define GPIO_PORTF_PUR_R	(*((volatile unsigned long*)0x40025510))
#define GPIO_PORTF_DEN_R	(*((volatile unsigned long*)0x4002551C))
#define GPIO_PORTF_LOCK_R	(*((volatile unsigned long*)0x40025520))
#define GPIO_PORTF_CR_R		(*((volatile unsigned long*)0x40025524))
#define GPIO_PORTF_AMSEL_R	(*((volatile unsigned long*)0x40025528))
#define GPIO_PORTF_PCTL_R	(*((volatile unsigned long*)0x4002552C))
#define SYSCTL_RCGC2_R		(*((volatile unsigned long*)0x400FE108))

/* Global Variables */
unsigned long SW1;					// input from PF4
unsigned long SW2;					// input from PF0

/** @fn		void portF_Init(void)
 *  @brief 	Initializes port F pins for input and output. PF4 is input
 * 		SW1 and PF2 is output LED.
*/
void portF_Init(void);

/** @fn 	void flash_SOS(void)
 *  @brief	Flashes the green LED SOS once.
 * 		PF3 is green LED: SOS
*/
void flash_SOS(void);

/** @fn		void delay(unsigned long)
 *  @brief 	Subroutine to delay in units of half-seconds.
 *  @param 	Half-seconds to be delayed for.
*/
void delay(unsigned long halfSecs);

/** @fn		main()
 *  @brief 	This is the main function of the program that calls other
 * 			defined functions to send SOS messages using the Green LED
 * 			on the LaunchPad until SW2 is pressed.
 *  @return An integer when successfully run.
*/
int main(void) {
	
	portF_Init();
	while (1) {
		do {
			// PF4 into SW1
			SW1 = GPIO_PORTF_DATA_R & 0x10;
		} while (SW1 == 0x10);
		do {
			flash_SOS();
			// PF0 into SW2
			SW2 = GPIO_PORTF_DATA_R & 0x01;
		} while (SW2 == 0x01);
	}
	
	return 0;
}

/* Initialize port */
void portF_Init(void) {
	volatile unsigned long delay;
	
	// F clock & delay
	SYSCTL_RCGC2_R |= 0x00000020;
	delay = SYSCTL_RCGC2_R;
	
	// unlock port F PF0
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	
	// allow changes to PF4-0
	GPIO_PORTF_CR_R |= 0x1F;
	
	// disable analog function
	GPIO_PORTF_AMSEL_R &= 0x0;
	
	// GPIO clear bit PCTL
	GPIO_PORTF_PCTL_R &= 0x0;
	
	// PF4, PF0 input
	GPIO_PORTF_DIR_R &= ~0x11;
	
	// PF3 output
	GPIO_PORTF_DIR_R |= 0x08;
	
	// no alternate functions
	GPIO_PORTF_AFSEL_R &= ~0x1F;
	
	// enable pullup resistors on PF4, PF0
	GPIO_PORTF_PUR_R |= 0x11;
	
	// enable digital pins on PF4-0
	GPIO_PORTF_DEN_R |= 0x1F;
}

/* 	Flash SOS */
/* 	Color 	|	LED(s)	| Port F
	---------------------------------------
	dark 	| 	---		| 0
	red 	|	R--		| 0x02
	blue 	| 	--B		| 0x04
	green 	| 	-G-		| 0x08
	yellow	| 	RG-		| 0x0A
	sky blue| 	-GB		| 0x0C
	white	| 	RGB		| 0x0E
*/
void flash_SOS(void) {
	
	// S
	GPIO_PORTF_DATA_R |= 0x08;
	delay(1);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(1);
	GPIO_PORTF_DATA_R |= 0x08;
	delay(1);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(1);
	GPIO_PORTF_DATA_R |= 0x08;
	delay(1);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(1);
	
	// O
	GPIO_PORTF_DATA_R |= 0x08;
	delay(4);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(4);
	GPIO_PORTF_DATA_R |= 0x08;
	delay(4);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(4);
	GPIO_PORTF_DATA_R |= 0x08;
	delay(4);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(4);
	
	// S
	GPIO_PORTF_DATA_R |= 0x08;
	delay(1);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(1);
	GPIO_PORTF_DATA_R |= 0x08;
	delay(1);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(1);
	GPIO_PORTF_DATA_R |= 0x08;
	delay(1);
	GPIO_PORTF_DATA_R &= ~0x08;
	delay(1);
	
	// delay for 5 sec between flashes
	delay(10);
}

/* Delay */
/* Originally count = 400,000 which took 0.13 sec to complete
   Changed it to 400000 * 0.5 / 0.13 = 1538460 so that it takes
   0.5 seconds to complete.
*/
void delay(unsigned long halfSecs) {
	unsigned long count;
	while (halfSecs > 0) {
		count = 1538460;
		while (count > 0) {
			count--;
		}
		halfSecs--;
	}
}
