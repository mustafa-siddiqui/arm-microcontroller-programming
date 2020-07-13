# Functional Debugging 

This program is a simple program to learn about the SysTick timer and debugging techniques. It runs on the ARM TM4C123 LaunchPad and was written using the Kiel µVision IDE version 4.74.

If either or both the switches are pressed, the LED flashes at 10 Hz and turns off if both the switches are released. 
Port F is initialized such that PF0 and PF4 are configured as inputs (the switches) and PF1 is configured as the output (LED). 
Debugging measures include dumping I/O data in `Time` and `Data` arrays. Data is dumped when there is a change in either PF0, PF1, or PF4 - meaning data is recorded when either of the switches are pressed/released or the LED turns on/off. Data includes the time when this change happens and the state of `GPIO_PORTF_DATA_R` - specifically only the three bits: PF0, PF1, PF4.

This technique of dumping data is similar to the operation of a 'blackbox' where data is dumped in ROM so that it can be recovered if there is a mishap and then inspected for irregularities or errors. 
