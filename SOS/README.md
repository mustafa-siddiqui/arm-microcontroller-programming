# SOS Message
This program uses the green LED on port F of the ARM TM4C123 microcontroller launchpad to send an SOS message by toggling the light 3 times with a gap of 1/2 seconds for 'S' and three times with a gap of 2 seconds for 'O'.

Port registers are first defined using Macros and then initialized to configure the appropriate inputs (two switches) and outputs (green LED).

Pressing switch 1 will start the SOS sequence and will continue with 5-second delays between SOS messages until switch 2 is pressed.
