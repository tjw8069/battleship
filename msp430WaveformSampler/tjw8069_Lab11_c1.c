#include <msp430.h> 

/**
 * Author: Tyler Winship
 * Email : tjw8069@g.rit.edu
 *
 * Purpose:
 * This is the main control loop for the lab 11 project.
 * All function calls made to the assembly functions will perform the necessary
 * operations to operate the microcontroller.
 *
 * The C file will also declare all the data for the program, and the assembly
 * files will reference this data with pointers. The following functions
 * will be defined in the assembly file
 *
 *  1. waitForCenter: Contol the LEDs while waiting for the center button to be
 *     pressed
 *
 *  2. waitForUpDown: monitors the top and bottom button to set the sample rate:
 *      up: sample 20 samples at ~100Hz
 *      down: sample 20 samples at ~200Hz
 *
 *  3. waitForLeftRight: Monitors the left and right wheel buttons to set scale
 *      left: measurements will be displayed on linear scale
 *      right: measurements will be display on log scale
 *
 *  4. getSamples: Sample with ADC, write those values to data
 *
 *  5. convertSamples: convert the measurements for display
 *
 *  6. displaySamples: display the values starting at bottom left and growing Clock
 *     wise. All LED display driving will be done in assembly.
 */

// Function prototypes: external are defined in tjw_lab11_a1.asm

extern void setup();
extern void waitForCenter(); //pointers to meas_base and thresh_low
extern void waitForUpDown(); //meas_base
extern void waitForLeftRight();
extern void getSamples();
extern void convertSamples();
extern void displaySamples();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	setup();

	waitForCenter();

	while(1){
	    waitForUpDown();
	    waitForLeftRight();
	    getSamples();
	    convertSamples();
	    displaySamples();
	}

	return 0;
}
