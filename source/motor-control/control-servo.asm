; SAC Project V1.0 - Copyright (C) 2009 Jorge Gomez Arenas
; This file is part of SAC Project. (http://sac.sindormir.net)
;
; SAC Project is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; SAC Project is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with SAC Project.  If not, see <http://www.gnu.org/licenses/>.

; FILENAME: control-servo.asm
; DESCRIPTION: VACIO Movement main program
; VERSION: 0.3
; DATE: 21/02/09
; AUTHOR: Jose Luis Iglesias <joejif@gmail.com>
; CHANGELOG:
; Version 0.1
; 	SETUP Function
;	Servo movement calling function
;	PWM-servo movement 
;	Follow the line Function
; 	Servo speeds yet to be stablished
; 	Sensor lecture and case evaluating function
;
; Version 0.2
;	Improved SETUP function 
; 	Sensor lecture and case evaluating function improved
;   Exit searching function added
; 	Special cases when taking an exit added
; 	'Searching the line when lost' function added
; 	Displaying speed on 7-segment function added
; 	Servo movement stablished
;
; Version 0.3
; 	Pre-initialization function improved
;	'Search the line when lost' function improved.
; 	General reliability in sensor lecture improved
; 	RESET_PIN added for returning to SETUP function.


   __CONFIG _WDT_OFF & _PWRTE_OFF & _INTOSC_OSC_NOCLKOUT & _MCLRE_OFF & _BOREN_ON & _LVP_OFF & _DATA_CP_OFF & _CP_OFF

	include <p16f628A.inc>			; Loads PIC library 16F628A
        include "../common/temps.inc"
	include "movs.inc"			; Loads movements library

	#define SPD1		PORTA,5		; Pins controlling the speed
	#define SPD2		PORTA,4		

	#define	TAKE_EXIT	PORTB,0		; 0=TRACKS THE INSIDE-LINE, 1=TAKES EXIT
	
	#define DISPLAY_MSB	PORTB,1		; 7segment control pins
	#define DISPLAY_LSB	PORTB,2
	
	#define	RESET_PIN	PORTB,3		; reset pin

	#define SENSOR_A1	PORTB,4
	#define SENSOR_P1	PORTB,5
	#define SENSOR_P2	PORTB,6
	#define SENSOR_A2	PORTB,7

	list 	p=16F628A

speed		equ	0x20	
SENSORS		equ	0x21	; stores data read by the central sensors
last_sensor_on	equ	0x22	; 0x00 if right, 0x01 if left

       ORG     0x00

; -----------------------------------------------------------------------------
SETUP
; -----------------------------------------------------------------------------
; DESCRIPTION:	Configuration for PIC ports and registers.
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		PRE_INIT
; -----------------------------------------------------------------------------
	clrf	PORTA		; clears PORTA
	movlw 	0X07 		; turns off comparers and enable pins
	movwf 	CMCON 		; for I/O
	bcf	STATUS, RP1
	bsf	STATUS, RP0	; Change to bank 1
	movlw 	b'00001011'	; speed clock setting
	movwf	PCON
	movlw   b'00110000'     ; RA0~RA4 = EXITS
	movwf   TRISA
	movlw   b'11111001'	; RB0 sensors input
	movwf   TRISB
	bcf     STATUS, RP0	; back to bank 0

	clrf	PORTB		; clears ports
	clrf	PORTA

	call	delay_1_sec	; Waits 1 second
	goto	PRE_INIT


; -----------------------------------------------------------------------------
PRE_INIT
; -----------------------------------------------------------------------------
; DESCRIPTION:	Pre-Initialization function called just once. Via the Speed 
;		Selection Inputs, the exit to be taken is displayed on the
;		7-segment display. The program stays at this function until
;		the TAKE_EXIT bit sets.
; INPUTS:	SPD1, SPD2, TAKE_EXIT
; OUTPUTS:	DISPLAY_MSB, DISPLAY_LSB
; CALLED FROM:	SETUP
; CALLS TO:	PRE_INIT, CHECK_EXIT
; -----------------------------------------------------------------------------


	btfsc	SPD1	
	bsf	DISPLAY_LSB

	btfss	SPD1
	bcf	DISPLAY_LSB

	btfsc	SPD2
	bsf	DISPLAY_MSB

	btfss	SPD2
	bcf	DISPLAY_MSB

	btfss	TAKE_EXIT
	goto	PRE_INIT

	goto	CHECK_EXIT


; -----------------------------------------------------------------------------
CHECK_EXIT
; -----------------------------------------------------------------------------
; DESCRIPTION:	Takes the decision of calling either FOLLOW_LINE or SRCH_EXIT,
; 		depending on the state of TAKE_EXIT.
; INPUTS:	W loaded with the sensors read in PORTB.
; 		TAKE_EXIT, RESET_PIN,
; OUTPUTS:	LED_R, LED_L
; CALLED FROM:	PRE_INIT
; CALLS TO:	SETUP, FOLLOW_LINE, SRCH_EXIT, BUSCA_CAMINO
; -----------------------------------------------------------------------------

	btfsc	RESET_PIN	; If RESET_PIN = 1, goes back to SETUP
	goto	SETUP

	bcf	LED_R		; Clears servo movement 
	bcf	LED_L		; green led exits.
	
	movlw	0xF0
	andwf	PORTB,W		; Mask PORTB = 11110000
	movwf	SENSORS		; Reads central sensors and stores them in SENSORS

	btfss	TAKE_EXIT	; Checks wether the next exit must be taken
	goto	FOLLOW_LINE

	goto	SRCH_EXIT
	

; -----------------------------------------------------------------------------
SRCH_WAY
; -----------------------------------------------------------------------------
; DESCRIPTION:	In case of getting lost and any sensor sees the line, checks
; 		the last sensor that was set, and makes the next move based on it.
; INPUTS:	last_sensor
; OUTPUTS:		-
; CALLED FROM:	FOLLOW_LINE
; CALLS TO:	TURN_LEFT, TURN_RIGHT, CHECK_EXIT
; -----------------------------------------------------------------------------
	btfsc	last_sensor_on,0	; Change last movement taken
	call	TURN_RIGHT		; in case of getting lost
	btfss	last_sensor_on,0
	call	TURN_LEFT
	goto	CHECK_EXIT
	

; -----------------------------------------------------------------------------
DISPLAY_SPD
; -----------------------------------------------------------------------------
; DESCRIPTION:	Displays the selected speed on the 7-segment, sending the
;		speed bits to it, and returns to the function of calling.
; INPUTS:	SPD1, SPD2
; OUTPUTS:	DISPLAY_LSB, DISPLAY_MSB
; CALLED FROM:	SRCH_EXIT, FOLLOW_LINE
; CALLS TO:		-
; -----------------------------------------------------------------------------
	bcf	DISPLAY_LSB	; Clears 7-segment
	bcf	DISPLAY_MSB
	btfsc	SPD1		; Displays speed on 7-segment
	bsf	DISPLAY_LSB
	btfsc	SPD2
	bsf	DISPLAY_MSB
	return

; -----------------------------------------------------------------------------
SRCH_EXIT	
; -----------------------------------------------------------------------------
; DESCRIPTION:	Checks 3 special cases in sensor reading, only when TAKE_EXIT		
;		is set and the VACIO is searching for the next exit. It any of 
;		these special cases takes place, calls FOLLOW_LINE, the main
;		movement function.
; INPUTS:	W loaded with the reading of the central sensors.
; OUTPUTS:		-
; CALLED FROM:	CHECK_EXIT
; CALLS TO:	DISPLAY_SPD, FOLLOW_LINE, TURN_RIGHT
; -----------------------------------------------------------------------------
				; Cases if TAKE_EXIT = 1 
	call	DISPLAY_SPD

	movf	SENSORS,W	; EXIT checking
	sublw	0xF0		; 1111 = all set
	btfsc	STATUS,Z	
	goto	TURN_RIGHT

	movf	SENSORS,W	; EXIT checking
	sublw	0xD0		; 1101 
	btfsc	STATUS,Z	
	goto	TURN_RIGHT

 	movf	SENSORS,W	; EXIT checking
 	sublw	0x90		; 1001
 	btfsc	STATUS,Z	
 	goto	TURN_RIGHT
	goto	FOLLOW_LINE


; -----------------------------------------------------------------------------
FOLLOW_LINE 
; -----------------------------------------------------------------------------
; DESCRIPTION:	Main movement function. 
; 		Checks the sensors set and compares the lecture with the cases on
;		the function, in order to take a turn or a forward movement.
; 		lado, a otro, o avanzando recto.
; INPUTS:	W loaded with the lecture of the central sensors.
; OUTPUTS:	-
; CALLED FROM:	SRCH_EXIT, CHECK_EXIT
; CALLS TO:	DISPLAY_SPD, MOVE_FORWARD, TURN_LEFT, TURN_RIGHT
; -----------------------------------------------------------------------------
				; Cases if TAKE_EXIT = 0
	call	DISPLAY_SPD	

	movf	SENSORS,W	; Moving forward checkign
	sublw	0x60		; 0110 = central sensors set
	btfsc	STATUS,Z
	goto	MOVE_FORWARD

	; CHECKING CASES TO TURN LEFT
	
	; 3 EXTRA CASES TO TURN LEFT
	movf	SENSORS,W	
	sublw	0xF0		; 1111
	btfsc	STATUS,Z	
	goto	TURN_LEFT

	movf	SENSORS,W	
	sublw	0xD0		; 1101 
	btfsc	STATUS,Z	
	goto	TURN_LEFT

 	movf	SENSORS,W	
 	sublw	0x90		; 1001
 	btfsc	STATUS,Z	
 	goto	TURN_LEFT

	; 4 MAIN CASES TO TURN LEFT
	movf	SENSORS,W
	sublw	0x10		; 0001
	btfsc	STATUS,Z
	goto	TURN_LEFT

	movf	SENSORS,W
	sublw	0x20		; 0010
	btfsc	STATUS,Z
	goto	TURN_LEFT

	movf	SENSORS,W
	sublw	0x30		; 0011
	btfsc	STATUS,Z
	goto	TURN_LEFT

	movf	SENSORS,W
	sublw	0x70		; 0111
	btfsc	STATUS,Z
	goto	TURN_LEFT

	; CHECKING CASES TO TURN RIGHT

	; 4 MAIN CASES TO TURN RIGHT
	movf	SENSORS,W
	sublw	0x80		; 1000
	btfsc	STATUS,Z
	goto	TURN_RIGHT

	movf	SENSORS,W
	sublw	0x40		; 0100
	btfsc	STATUS,Z
	goto	TURN_RIGHT

	movf	SENSORS,W
	sublw	0xC0		; 1100
	btfsc	STATUS,Z
	goto	TURN_RIGHT

	movf	SENSORS,W
	sublw	0xE0		; 1110
	btfsc	STATUS,Z
	goto	TURN_RIGHT

	goto	SRCH_WAY	; In case that no sensor is set
	END
