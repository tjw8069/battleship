;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
			.global	setup					; Make functions known to linker
            .global waitForCenter           ;
            .global	waitForUpDown			;
            .global	waitForLeftRight		;
            .global	getSamples				;
            .global	convertSamples			;
            .global	displaySamples			;

;-------------------------------------------------------------------------------
; Program Data
;-------------------------------------------------------------------------------
			.bss 	data, 40				; holds samples, 20 words=40 bytes

			.bss	meas_base, 10			; baseline, high threshold
			.bss	thresh_low, 10			; low threshold for de-bouncing
			.bss	meas_latest, 10			; latest measurements

			.bss	speedSet, 1			; boolean for sample speed
			.bss	scaleSet, 1			; boolean for setting scale
			.bss	SWdelay, 2			; value for software delay
			.bss	timerSWflag, 2		; timer flag controlled by ISR

			.bss	LEDdisplay,40		; 40 bytes that hold the LED states
										; for the data.

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
setup:
				clr		P1OUT		; Clear display
				mov.b	#0x20, R12	; initialize to point to P2.x
				mov.b	#0x00, R13	; initialize 12 to the base of meas_base
				bic.b	#0xff,&P1DIR	; set up P1 as input
				bis.b	#0xf8,&P1DIR	; set up P1[7:3] as outputs
				mov		#4, SWdelay


meas_base_again	call #meas_setup	;
;-------------------------------------------------------------------------------
; Clear TAR and start TA0 in continuous mode; use BIS and not MOV
; so that you don't cancel previous settings
;-------------------------------------------------------------------------------
			bis #MC_2 + TACLR, &TA0CTL 	;
;-------------------------------------------------------------------------------
; Call the SW delay routine, which here it is used to provide the accumulation
; period; could use instead ACLK fed from VLO
;-------------------------------------------------------------------------------
			call #SWtimer			;
;-------------------------------------------------------------------------------
; Now, after the accumulation period has passed, generate a SW based
; capture trigger by toggeling CCIS0
;-------------------------------------------------------------------------------
			xor	#CCIS0, &TA0CCTL1	;
;-------------------------------------------------------------------------------
; Save the baseline captured value in meas_base array
;-------------------------------------------------------------------------------
			mov	TA0CCR1, meas_base(R13)	; note the use of the SYMBOLIC AM
			mov TA0CCR1, thresh_low(R13)	; make low threshold for debouncing
			bic #MC1+MC0, &TA0CTL 	; Stop TA
			sub #2, meas_base(R13)	; Adjust this baseline
			sub #6, thresh_low(R13)	; Adjust low threshold

			bic.b 	R12,&P2SEL2		; Stop the oscillation on the latest. pin
			rra.b	R12				; Prepare next x
			add.b	#0x02, R13		; Prepare the next index into the array
			cmp.b	#0x01, R12		; Check if done with all five sensors
			jne		meas_base_again	;
			ret						;

waitForCenter:
			call 	#meas_latest_val		; measure latest
			cmp 	meas_latest, thresh_low	; is button pressed?
			jl 		waitForCenter		; wait: not pressed yet.
			ret						; Could add debouncing with
									; pressed state

waitForUpDown:
; Will evaluate up and down buttons to see if they're pressed
; up is index 1 (+2bytes) of measure arrays, down is index 3(+6bytes)

			mov.b	#2,speedSet	; #2 is garbage value, shows no value has been
								; read in yet
			mov.b	#0x90, P1OUT ; prepare LEDs for up down display LED 1 and 4

checkUp		call	#meas_latest_val
			inv		P1OUT
			mov 	#0x30, SWdelay
			call 	#SWtimer	; delay for multiplexing
			mov.b	#2, r13		; index = 1 to access up sensor data
			cmp		meas_latest(r13), thresh_low(r13)
			jl		checkDown		; button not pressed
			mov.b	#0, speedSet	; up was pressed, will still check down
checkDown	add		#4, r13		; index = 3 to access down sensor data
			inv 	P1OUT
			call	#SWtimer	; delay for multiplexing
			cmp.b	meas_latest(r13), thresh_low(r13)
			jl		nopress			; button not pressed
			mov.b 	#1, speedSet	; down was pressed
nopress		cmp.b	#2, speedSet	; has speedSet been set
			jeq		checkUp			; speed set hasn't been set
			bic.b	#0xFF, P1OUT	; clear LED display
			bic		#(MC_1 + MC_0), &TACTL 	; Stop timer
			ret

waitForLeftRight:
; Will evaluate left and right buttons to see if they're pressed
; right is index 2 (+4bytes) of measure arrays, left is index 4 (+8bytes)

; Will multiplex the left and right most LEDs by inverting the P1OUT register

			mov.b	#2,scaleSet	; #2 is garbage value, shows no value has been
								; read in yet
			mov.b	#0x60, P1OUT		; set up initial outputs LED 2 and 3
checkRight	call	#meas_latest_val
			inv		P1OUT		; multiplex LED Display, show LED 6 and 7
			mov 	#0x30, SWdelay
			call 	#SWtimer
			mov.b	#4, r13		;
			cmp		meas_latest(r13), thresh_low(r13)
			jl		checkLeft		; button not pressed
			mov.b	#0, scaleSet	; right was pressed
			jmp		checkPress
checkLeft	add		#4, r13		;
			inv		P1OUT		; multiplex LED Display, show LED 2 and 3
			call 	#SWtimer
			cmp.b	meas_latest(r13), thresh_low(r13)
			jl		checkPress		; button not pressed
			mov.b 	#1, scaleSet	; left was pressed
checkPress	cmp.b	#2, scaleSet	; has speedSet been set
			jeq		checkRight		; speed set hasn't been set
			bic.b	#0xFF, P1OUT	; clear LED display
			bic		#(MC_1 + MC_0), &TACTL 	; Stop timer
			ret
getSamples:
; Need to use HW timer to get accurate samples.
; Sampling rate is dependent on speedSet.
; ADC will be enabled by timer A out input which will generate a rising
; edge at intervals equal to the sampling period
			clr		&TACTL							; clear timer control
			bis 	#(TASSEL_2 + TACLR), &TACTL	; timer setup
; Rising edge will be generated by TACCR0 with timer in up mode.
; TACCR1 will reset the waveform again. Will be small to ensure the pulse is
; shorter than the time to sample and convert. (< 14 ADC10 Clk cycles
			bis		#(OUTMOD_7), TACCTL1
			mov		#0x0002, TACCR1		; resets waveform

; Set up ADC10
			clr.w	&ADC10CTL0	; Clear configuration registers just in case
			clr.w	&ADC10CTL1	; some values were left on by a prior routine

;------------------------------------------------------------------------------
; The ADC will be set in Repeat Single channel mode and will be triggered by
; the output of TimerA.
; Registers
;	SREF=001, ADC10SHT=64*ADC10CLKs, ADC10SR=0, REFOUT=0, REFBURST=0, MSC=0
;	(waits for trigger before sampling again)REF2_5=0, REFON=1, ADC10ON=1,
;	ADC10IE=1, ADC10IFG=0, ENC=0, ADC10SC=0
;------------------------------------------------------------------------------
			bis #(SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE), &ADC10CTL0

;------------------------------------------------------------------------------
; The other control register will select sample and hold control SHS, input
; channel
;	INCH = 0000, SHS = 01 (TAOUT1), ADC10DF (binary), ISSH = 0 (no invert
;	sample and hold), AD10DIV = 111 (/8), ADC10SSEL = 00 (ADC10OSC ~5MHz),
;	CONSEQ = 01 (Repeat single channel)
;------------------------------------------------------------------------------
			bis.w #(INCH_0 + SHS_1 + ADC10DIV_7 + CONSEQ_2), &ADC10CTL1
;------------------------------------------------------------------------------
; Will use onboad Data transfer controller in one block mode, non-continuous
;	ADC10TB = 0 (one-block mode), ADC10CT = 0 (non-continuous)
;------------------------------------------------------------------------------
			clr		&ADC10DTC0
;------------------------------------------------------------------------------
; Need to define number of transfers in each block. Need to fill the 20 word
; data array. There will be 20(decimal) transfers which is 0x14(hex)
;------------------------------------------------------------------------------
			mov 	#0x14, &ADC10DTC1
;------------------------------------------------------------------------------
; Need to define the start address for the DTC, it will be the start of the
; data array. Need to check if ADC is busy (shouldn't be)
;------------------------------------------------------------------------------
busy_test	bit.w	#BUSY, &ADC10CTL0
			jnz		busy_test
			mov		#data, &ADC10SA

checkRate	tst.b 	speedSet			; Check speed set value
			jz		set100Hz
set200Hz	mov		#0x1388, &TACCR0	; set sample time 200Hz
			jmp 	start_samples
set100Hz	mov 	#0x2710, &TACCR0	; set sample time 100Hz

;------------------------------------------------------------------------------
; At this point the timer, ADC, and DTC are set and ready to go. Conversions
; are started by setting the ENC bit and starting the timer in up mode.
;------------------------------------------------------------------------------
start_samples
			bis 	#ENC, &ADC10CTL0	; Enable ADC10, now waiting for trigger
										; from timerA.
			eint						; Enable General Interupts
			bis		#MC_1, &TACTL		; Start timer in up mode

still_Sampling
			cmp.b 	#2, speedSet		; ISR sets speedSet to 2 when samplin
										; is completed
			jne		still_Sampling		; jump if not equal 2
			bic		#(MC_1 + MC_0), &TACTL	; sampling is done
			clr 	ADC10DTC1			; disable DTC (no more memory writes).


convertSamples:
			mov		#0x28, R12			; will be used to iterate through data
					;R13				  Will store number of LEDs that are on
			tst.b	scaleSet			; does scaleSet = 0?
			jz		logScale			; right was pressed, log scale

linearScale								; left was pressed, linear scale
; For linear scale need to mask all bits except 9 - 7 for index in data.
; When bit 9 is on, four LEDs are added.
; When bit 8 is on, two LEDs are added.
; When bit 7 is on, one LED is added.
; LEDdisplay stores the states of the LED in one word, the low byte is the
; 	inverse of P1OUT when LED 5-8 are on.
;	The high byte is P1OUT when LED 1-4 are on.
			mov		#0, R13				; Index in LEDfill will always >= 0
			sub		#2, R12				; decrement to next data index
testbit9	bit.w	#0x0200, data(R12)	; test bit #9
			jz		testbit8			; bit9 was 0: don't add to LEDs on
			add		#8, R13				; at least 4 more LEDs are on
										; 8 bytes, 4 words (index + 4)
testbit8	bit.w	#0x0100, data(R12)	; test bit #8
			jz		testbit7			; bit8 was 0: don't add to LEDs on
			add		#4, R13				; at least 2 more LEDs are on
										; 4 bytes, 2 words (index + 2)
testbit7	bit.w	#0x0080, data(R12)	; test bit #7
			jz		setLEDarray			; bit7 was 0: don't add to LEDs on
			add		#2, R13				; at least 1 more LED is on
										; 2 bytes, 1 word (index + 1)
setLEDarray
; LEDfill contains the correct binary to turn on the # of LEDs, it will write
; to the LEDdisplay array which will be used in the display values function
			mov		LEDfill(R13), LEDdisplay(R12)
; in order to display a nice value on the UART interface, the # of LEDs
; will be stored in the lowest byte of the LEDdisplay value. R13 needs to
; be divided by 2 in order to make sense becasue R13, holds the index in
; LEDfill. Also need to add one to account for the LED that is always on
; The extra LED will be accounted for in the offset to print and ascii
; character.
			rra.b	R13
			and.b 	#0x0F, R13
			bis		R13, LEDdisplay(R12)
checkDone	tst.w	R12					; is R12 zero
			jnz		linearScale			; R12 is not zero, do the next index
			ret

logScale
; To detect the number of LEDs that need to be on for the log scale, need
; to see where the first high bit is. This is done by rotating left until
; carry = 1. For every rotation

nextData	mov 	#0x10, R13			; index LEDfill = 8 - # of rotations
										; address LEDfill =
										; (LEDfill + 16) - 2*#rotations
			sub		#2, R12				; prepare for next data index
			mov		data(R12), R15
			rrc		R15					; Moving data to be byte aligned
			rrc		R15					; so that the rotate lefts go from
										; bit 7 to carry.
rotateAgain sub		#2, R13					; subtract on LED
			cmp		#0, R13				; #LEDs must be >= 0
			jeq		noRotate			; all bits were 0
			rlc.b	R15					; Rotate byte (bit 8 to carry)
			jnc		rotateAgain			; Carry was zero, haven't found
										; first one yet.

; LEDfill contains the correct binary to turn on the # of LEDs, it will write
; to the LEDdisplay array which will be used in the display values function
noRotate	mov		LEDfill(R13), LEDdisplay(R12)
			rra.b	R13
			and.b 	#0x0F, R13
			bis		R13, LEDdisplay(R12)

checkDataIndex
			tst		R12
			jnz		nextData			; Have not gone through all data
			ret

displaySamples:

;-------------------------------------------------------------------------------
; Setup USI in UART mode, minimal configuration, point to point
;-------------------------------------------------------------------------------

			bis.b #6h,&P1SEL 	; P1.2/P1.1 = USART0 TXD/RXD
			bis.b #6h,&P1SEL2 	; P1.2/P1.1 = USART0 TXD/RXD

SetupUART0 	clr.b &UCA0CTL0 		; default values - see UG
			clr.b &UCA0CTL1 		; default values - see UG
			bis.b #UCSSEL1 + UCSSEL0,&UCA0CTL1 ; UCLK = SMCLK ~1 MHz
			clr.b &UCA0STAT 		; default values - see UG
;			bis.b #UCLISTEN,&UCA0STAT ; loopback - used for debugging only
;-------------------------------------------------------------------------------
; For a baud rate of 9600,the pre-scaler value is
;    = (UCAxBR0 + UCAxBR1 × 256) = 104 in decimal - integer part - see UG
;-------------------------------------------------------------------------------
			mov.b #074h,&UCA0BR0 	; Baud Rate = ? - YOU MUST COME UP WITH THIS VALUE
			mov.b #000h,&UCA0BR1 	; UCBRx = ?		- FOR THE REQUIRED BAUD RATE
;-------------------------------------------------------------------------------
; Modulation Control Register - fractional part - see UG
;-------------------------------------------------------------------------------
			mov.b #002h,&UCA0MCTL 	; UCBRFx = 0, UCBRSx = 1, UCOS16 = 0
;-------------------------------------------------------------------------------
; SW reset of the USI state machine
;-------------------------------------------------------------------------------
			bic.b #UCSWRST,&UCA0CTL1 ; **Initialize USI state machine**
			bis.b #UCA0RXIE,&IE2 	; Enable USART0 RX interrupt
	 		bis.b #GIE,SR 			; General Interrupts Enabled
;-------------------------------------------------------------------------------
; After the state machine is reset, the TXD line seems to oscillate a few times
; It is therefore safer to check if the machine is in a state in which it is
; ready to transmit the next byte.  Don't remove this code!
;-------------------------------------------------------------------------------
NewLine1	bit.b #UCA0TXIFG,&IFG2 	; USI TX buffer ready?
			jz NewLine1					; Jump if TX buffer not ready
			mov.b #0x0D,&UCA0TXBUF 	; TX <carriage return> charac. eq. to #0x0D in ASCII
Cr1			bit.b #UCA0TXIFG,&IFG2 	; USI TX buffer ready?
			jz Cr1 					; Jump if TX buffer not ready
			mov.b #0x0A,&UCA0TXBUF 	; TX <line feed> charac. eq. to #0x0A in ASCII

			clr 	R12				; data and LEDdisplay index
			; will be incremented on the SW flag

;-------------------------------------------------------------------------------
; Timer Set up: TimerA1 will generate an interupt every 500 ms to switch to next
; index.
;-------------------------------------------------------------------------------
			mov		#(TASSEL_2 + ID_3 + TACLR), TA1CTL
			mov		#0xF42F, TA1CCR0	; period of 500ms in up mode
			bis		#CCIE, TA1CCTL0	; enable interupt on TA1CCR0
			eint					; enable general interupts
			bis		#(MC_1), TA1CTL ; start timer in up mode

checkFlag	tst.b	timerSWflag		; check SW flag
			jz		showRight		; if the flag isn't set show last value
			bic		#0xFFFF, timerSWflag	; clear flag
print		bit.b 	#UCA0TXIFG,&IFG2 	; USI TX buffer ready?
			jz 		print				; Jump if TX buffer not ready
			mov		LEDdisplay(R12), R15		; Move data to data register
			and		#0x000F, R15			; mask LED bits
			add		#0x31, R15			; offset to make it a number
			mov.b	R15,&UCA0TXBUF	; Transmitt input back to user
			add		#2, R12			; prepare next index
checkData	cmp.b	#0x28, R12		; is it at the last index
			jl		showRight		; not done with data jump to display
			clr		R12				; start over
			clr		P1OUT
			; Print new line to show the process started over in terminal
NewLine		bit.b #UCA0TXIFG,&IFG2 	; USI TX buffer ready?
			jz NewLine					; Jump if TX buffer not ready
			mov.b #0x0D,&UCA0TXBUF 	; TX <carriage return> charac. eq. to #0x0D in ASCII
Cr			bit.b #UCA0TXIFG,&IFG2 	; USI TX buffer ready?
			jz Cr 					; Jump if TX buffer not ready
			mov.b #0x0A,&UCA0TXBUF 	; TX <line feed> charac. eq. to #0x0A in ASCII

showRight	push	R12
			call	#meas_latest_val
			pop		R12
			cmp		thresh_low, meas_latest
			jl		exit		; center button pressed
			mov		LEDdisplay(R12), R15	; R15 is data manipulation register
			and 	#0x00FF, R15	; mask upper byte
			inv.b	R15				; invert
			mov.b	R15, P1OUT
			mov 	#0x30, SWdelay	; delay to make them visible
			call 	#SWtimer

showLeft	push	R12
			call	#meas_latest_val
			pop		R12
			cmp		thresh_low, meas_latest
			jl		exit		; center button pressed
			mov		LEDdisplay(R12), R15	; R15 is data manipulation register
			and 	#0xFF00, R15	; mask upper byte
			swpb	R15				; swap high and low byte
			mov.b	R15, P1OUT		;
			mov 	#0x30, SWdelay	; delay to make them visible
			call 	#SWtimer
			jmp		checkFlag

exit		ret

meas_setup:	bic.b 	R12,&P2DIR 	; P2.x input
			bic.b 	R12,&P2SEL 	;
			bis.b 	R12,&P2SEL2	;
			mov 	#TASSEL_3, &TA0CTL 	;
;-------------------------------------------------------------------------------
; Setting up to capture the value of TAR on either rising or falling edges
; using SW based trigger
;-------------------------------------------------------------------------------
			mov #CM_3 + CCIS_2 + CAP, &TA0CCTL1 	;
			ret

meas_latest_val:
			mov.b	#0x20, R12	; initialize R5 to point to P2.1
			mov.b	#0x00, R13	; initialize R6 to the base of meas_base
			mov.w	#4,	SWdelay
meas_latest_again:
			call #meas_setup	;
			bis #MC_2 + TACLR, &TA0CTL 	; Continuous, Clear TAR
			call #SWtimer			;
			xor #CCIS0, &TA0CCTL1	; Trigger SW capture
			mov TA0CCR1, meas_latest(R13)	; Save captured value in array
			bic #MC1+MC0, &TA0CTL 	; Stop timer
			bic.b 	R12,&P2SEL2		; Stop the oscillation on the latest. pin
			rra.b	R12				; Prepare next x
			add.b	#0x02, R13		; Prepare the next index into the array
			cmp.b	#0x01, R12		; Check if done with all five sensors
			jne		meas_latest_again	;
			ret					;

SWtimer:	mov	SWdelay, r14		; Load delay value in r5
Reloadr7	mov	SWdelay, r15		; Load delay value in r6
ISr70		dec	r15					;
			jnz	ISr70				; The total SW delay count is
			dec	r14					;  = SWdelay * SWdelay
			jnz	Reloadr7			;
			ret						; Return from this subroutine

;-----------------------------------------------------------------------------
; Data written in ROM: This data will be written the to LEDdisplay values to
; store which LED combination should be on. Data is stroed like this:

; LED#: 4 | 3 | 2 | 1 | 0000 | 8 | 7 | 6 | 5 | 0000

; This way the bytes can be swapped in order to write them to the correct
; P1OUT locations.
;-----------------------------------------------------------------------------
LEDfill:
			.word 	0x8000			; 1 LED  on (index 0)
			.word	0xC000			; 2 LEDs on
			.word	0xE000			; 3 LEDs on
			.word	0xF000			; 4 LEDs on
			.word	0xF010			; 5 LEDs on
			.word	0xF030			; 6 LEDs on
			.word	0xF070			; 7 LEDs on
			.word	0xF0F0			; 8 LEDs on (index 7)

;------------------------------------------------------------------------------
; ISR definitions
;------------------------------------------------------------------------------
TIMER_ISR:
			bic.w 	#CCIFG, TACCTL0	; clear interupt
;			bis.b	#1, timerSWflag	; enable sample flag
			reti

TIMER1_ISR:
			bic.w 	#CCIFG, TA1CCTL0	; clear interupt
			mov		#1, timerSWflag	; enable sample flag
			reti



ADC10_ISR:
			bic.w #ADC10IFG, &ADC10CTL0	; Clear interupt
			mov.b #2, speedSet			; to tell processor sampling is done
			reti 				;

			.sect ".int05" 			; ADC10 Vector
isr_adc10: 	.short ADC10_ISR ;

			.sect ".int09"			; Timer A0
isr_timerA: .short TIMER_ISR

			.sect ".int13"			; Timer A1
isr_timerA1:
			.short TIMER1_ISR

.end
