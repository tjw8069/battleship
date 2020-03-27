This program is written for the MSP430 microcontroller.

C code calls the assembly "functions." This took advantage of C readability
to show the operation of the program. 

The assembly functions use less memory and are more efficient to run on a small
device.

Program flow is as follows:
1. User presses center button of capacitive touch pad on top of device.
2. User presses up or down button to decide on the sampling rate.
3. User presses left or right button to decide on a linear or logarithmic
   scale to display measured values.
4. Device uses on board ADC to sample a an input waveform at the selected rate.
5. Device displays the voltage values using a ring of LEDs on capacitive
   touch board. The number of LEDs that are turned on is also sent over a
   UART connection and displayed in a console.
   
The assembly programs makes use of hardware timers, software timers,
debouncing, mutilplexed displays, and UART serial communications to display
the correct values.

There is some optimization that could be done to the code. For example, the
program determines the number of LED that should be on by referencing an
array in read only memory and copying the index with the correct sequence of
LEDs to memory specific to that sample. This is a wasteful process becasue
ROM could be moved directly to a register instead of being saved in RAM.
