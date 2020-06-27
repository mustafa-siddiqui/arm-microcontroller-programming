# Pacemaker 

This code works as a simulator for a simple heart pacemaker. It runs on the ARM TM4C123 LaunchPad and was written using the Kiel µVision IDE version 4.74.
	
The input from switch 1 on the launch pad acts as an atrial sensor (AS) on a pacemaker. Output to the green LED on the pad is Ready and is used for debugging and does not exist on an actual pacemaker. Output to the red LED acts as a ventricular trigger (VT). 

The program begins by setting Ready as high and waiting for the switch to be pressed. When it is pressed, it clears Ready (set as low), and waits for the switch to be released. When it is released, it waits for 250 ms (simulates the time between atrial and ventricular contraction) and sets VT as high which will pulse the ventricles. It then waits for another 250 ms and then clears VT (set as low).
