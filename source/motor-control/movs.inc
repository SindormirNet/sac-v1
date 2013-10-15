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


; FILENAME: movs.inc
; DESCRIPTION: VACIO servo basics routines
; VERSION: 0.4
; DATE: 24/02/09
; AUTHOR: Jose Luis Iglesias <joejif@gmail.com>
; CHANGELOG:
; 
; Version 0.1
;	Independent servo basic movement functions
;	Calls to time functions for servo movement.
;	Turning movements
;	Basic function for servo stopping
; Version 0.2
;	Fixex basic function for servo stopping
;	Added servo movement complex functions 
;	2 Different speeds to choose on servo movement
;	Improved turning movement 
;	Problem with left servo on speed 1
; Version 0.3
; 	Added function for checking exit
;	Added new cases in sensor reading for taking exits
;	3 different servo movement speeds
;	Added new main speed control movement functions
;	Improved "Searching the line when lost" function 
;	Added simultaneous servo movement functions when moving forward
; Version 0.4
;	Fixed left servo movement on speed 1
;	3 Final speeds stablished
;	Improved simultaneous servo movement functions when moving forward
;	Improvement on general speed

	#define	SERVO_R		PORTA,0
	#define	SERVO_L		PORTA,3
	#define LED_R		PORTA,1
	#define LED_L		PORTA,2

	ORG     100

; BASIC MOVEMENT FUNCTIONS

; -----------------------------------------------------------------------------
STOP_LS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Stops left servo
; INPUTS:		-
; OUTPUTS:		SERVO_L
; CALLED FROM:		STOP, MOVE_LEFT_MIN, MOVE_LEFT_MED
; 			MOVE_LEFT_MAX
; CALLS TO:		-
; -----------------------------------------------------------------------------
	BCF	SERVO_L
	RETURN


; -----------------------------------------------------------------------------
STOP_RS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Stops right servo
; INPUTS:		-
; OUTPUTS:		SERVO_R
; CALLED FROM:		STOP, MOVE_RIGHT_MIN, MOVE_RIGHT_MED
; 			MOVE_RIGHT_MAX
; CALLS TO:		-
; -----------------------------------------------------------------------------
	BCF	SERVO_R
	RETURN


; -----------------------------------------------------------------------------
VMIN_LS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts left servo at minimum speed 
; INPUTS:		-
; OUTPUTS:		SERVO_L
; CALLED FROM:		MOVE_LEFT_MIN
; CALLS TO:		delay_0.00138_sec, delay_0.01862_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_L
	CALL	delay_0.00138_sec 	;original=1,36
	BCF	SERVO_L
	CALL	delay_0.01862_sec 	;original=64
	RETURN


; -----------------------------------------------------------------------------
VMIN_RS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts right servo at minimum speed 
; INPUTS:		-
; OUTPUTS:		SERVO_R
; CALLED FROM:		MOVE_RIGHT_MIN
; CALLS TO:		delay_0.00132_sec, delay_0.01868_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_R
	call	delay_0.00132_sec
	bcf	SERVO_R
	call	delay_0.01868_sec
	RETURN


; -----------------------------------------------------------------------------
VMED_LS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts left servo at medium speed
; INPUTS:		-
; OUTPUTS:		SERVO_L
; CALLED FROM:		MOVE_LEFT_MED
; CALLS TO:		delay_0.00143_sec, delay_0.01857_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_L
	call	delay_0.00143_sec
	bcf	SERVO_L
	call	delay_0.01857_sec
	RETURN


; -----------------------------------------------------------------------------
VMED_RS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts right servo at medium speed
; INPUTS:		-
; OUTPUTS:		SERVO_R
; CALLED FROM:		MOVE_RIGHT_MED
; CALLS TO:		delay_0.00123_sec, delay_0.01877_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_R
	call	delay_0.00123_sec
	bcf	SERVO_R
	call	delay_0.01877_sec
	RETURN


; -----------------------------------------------------------------------------
VMAX_LS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts left servo at maximum speed 
; INPUTS:		-
; OUTPUTS:		SERVO_L
; CALLED FROM:		MOVE_LEFT_MAX
; CALLS TO:		delay_0.00163_sec, delay_0.01837_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_L
	CALL	delay_0.00163_sec
	BCF	SERVO_L
	CALL	delay_0.01837_sec
	RETURN


; -----------------------------------------------------------------------------
VMAX_RS
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts right servo at maximum speed
; INPUTS:		-
; OUTPUTS:		SERVO_R
; CALLED FROM:		MOVE_RIGHT_MAX
; CALLS TO:		delay_0.00103_sec, delay_0.01897_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_R
	CALL	delay_0.00103_sec
	BCF	SERVO_R
	CALL	delay_0.01897_sec
	RETURN


;FUNCTIONS FOR BOTH SERVOS SIMULTANEOUS MOVEMENT WHEN MOVING STRAIGHT
; -----------------------------------------------------------------------------
FWD_MAX
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts both servos simultaneously maximum speed
; INPUTS:		-
; OUTPUTS:		SERVO_L, SERVO_R
; CALLED FROM:		MOVE_FORWARD
; CALLS TO:		delay_0.00103_sec, delay_0.0006_sec, delay_0.01837_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_L
	BSF	SERVO_R
	CALL	delay_0.00103_sec
	BCF	SERVO_R
	CALL	delay_0.0006_sec
	BCF	SERVO_L
	CALL	delay_0.01837_sec
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
FWD_MED
; -----------------------------------------------------------------------------
; DESCRIPTION:	Starts both servos simultaneously at medium speed
; INPUTS:		-
; OUTPUTS:		SERVO_L, SERVO_R
; CALLED FROM:		MOVE_FORWARD
; CALLS TO:		delay_0.00123_sec, delay_0.0002_sec, delay_0.01857_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_L
	BSF	SERVO_R
	CALL	delay_0.00123_sec
	BCF	SERVO_R
	CALL	delay_0.0002_sec
	BCF	SERVO_L
	CALL	delay_0.01857_sec
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
FWD_MIN
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts both servos simultaneously at minimum speed
; INPUTS:		-
; OUTPUTS:		SERVO_L, SERVO_R
; CALLED FROM:		MOVE_FORWARD
; CALLS TO:		delay_0.00132_sec, delay_0.00006_sec, delay_0.01862_sec
; -----------------------------------------------------------------------------
	BSF	SERVO_L
	BSF	SERVO_R
	CALL	delay_0.00132_sec
	BCF	SERVO_R
	CALL	delay_0.00006_sec
	BCF	SERVO_L
	CALL	delay_0.01862_sec; 
	GOTO	CHECK_EXIT


;FUNCTIONS FOR COMPLEX MOVEMENT
; -----------------------------------------------------------------------------
STOP
; -----------------------------------------------------------------------------
; DESCRIPTION:		Stops both servos
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		STOP_I, STOP_S
; -----------------------------------------------------------------------------
	CALL	STOP_LS
	CALL	STOP_RS
	GOTO	CHECK_EXIT



; -----------------------------------------------------------------------------
MOVE_RIGHT_MIN
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts left servo and stops right servo
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		TURN_RIGHT
; CALLS TO:		VMIN_LS, VMED_LS_RS
; -----------------------------------------------------------------------------
	CALL	VMIN_LS
	CALL	STOP_RS
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
MOVE_RIGHT_MED
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts left servo and stops right servo
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		TURN_RIGHT
; CALLS TO:		VMED_LS, VMED_LS_RS
; -----------------------------------------------------------------------------
	CALL	VMED_LS
	;CALL	VMIN_RS	
	CALL	STOP_RS
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
MOVE_RIGHT_MAX
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts left servo and stops right servo
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		TURN_RIGHT
; CALLS TO:		VMAX_LS, VMED_LS_RS
; -----------------------------------------------------------------------------
	CALL	VMAX_LS
;	CALL	VMIN_RS
	CALL	STOP_RS
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
MOVE_LEFT_MIN
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts right servo and stops left servo
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		TURN_LEFT
; CALLS TO:		VMIN_RS, VMED_LS_LS
; -----------------------------------------------------------------------------
	CALL	VMIN_RS
	CALL	STOP_LS
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
MOVE_LEFT_MED
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts right servo and stops left servo
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		TURN_LEFT
; CALLS TO:		VMED_RS, VMED_LS_LS
; -----------------------------------------------------------------------------
	CALL	VMED_RS
; 	CALL	VMIN_LS
	CALL	STOP_LS
	GOTO	CHECK_EXIT


; -----------------------------------------------------------------------------
MOVE_LEFT_MAX
; -----------------------------------------------------------------------------
; DESCRIPTION:		Starts right servo and stops left servo
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		TURN_LEFT
; CALLS TO:		VMAX_RS, VMED_LS_LS
; -----------------------------------------------------------------------------
	CALL	VMAX_RS
;	CALL	VMIN_LS
	CALL	STOP_LS
	GOTO	CHECK_EXIT

; MAIN MOVEMENT FUNCTIONS
; -----------------------------------------------------------------------------
MOVE_FORWARD
; -----------------------------------------------------------------------------
; DESCRIPTION:	Select the speed to take, based on the speed bits.
; 			
; INPUTS:		Central I/R sensors, loaded on 'speed'
; OUTPUTS:		LED_L, LED_R
; CALLED FROM:		SIGUE_LINEA
; CALLS TO:		FWD_MAX, FWD_MED, FWD_MIN, STOP
; -----------------------------------------------------------------------------
	bsf	LED_R		; Turns on both servo movement LEDs
	bsf	LED_L

	movlw	0x30
	andwf	PORTA,W		; PORTA bit-mask, 0011 0000
	movwf	speed

	movf	speed,W
	sublw	0x30		; 11 = MAX speed
	btfsc	STATUS,Z
	goto	FWD_MAX		

	movf	speed,W
	sublw	0x10		; 10 = MED speed
	btfsc	STATUS,Z
	goto	FWD_MED

	movf	speed,W
	sublw	0x20		; 01 = MIN speed
	btfsc	STATUS,Z
	goto	FWD_MIN

	goto	STOP		; 00 = STOP


; -----------------------------------------------------------------------------
TURN_LEFT
; -----------------------------------------------------------------------------
; DESCRIPTION:	Select the speed to take, based on the speed bits.
; 			
; INPUTS:		Central I/R sensors, loaded on 'speed'
; OUTPUTS:		LED_L, LED_R, last_sensor_on
; CALLED FROM:		SIGUE_LINEA
; CALLS TO:		MOVE_LEFT_MAX, MOVE_LEFT_MED, 
; 			MOVE_LEFT_MIN, STOP
; -----------------------------------------------------------------------------
	bcf	LED_R		; Turns on the left movement LED  
	bsf	LED_L		; and turns off the left one

	BCF	last_sensor_on,0	; Clears last_sensor_on register
					; to be checked by SEARCH_LINE
	
	movlw	0x30
	andwf	PORTA,W		; PORTA bit-mask, 0011 0000
	movwf	speed

	movf	speed,W
	sublw	0x30		; 11 = MAX speed 
	btfsc	STATUS,Z
	goto	MOVE_LEFT_MAX

	movf	speed,W
	sublw	0x10		; 10 = MED speed
	btfsc	STATUS,Z
	goto	MOVE_LEFT_MED

	movf	speed,W
	sublw	0x20		; 01 = MIN speed
	btfsc	STATUS,Z
	goto	MOVE_LEFT_MIN
	
	goto	STOP		; 00 = STOP


; -----------------------------------------------------------------------------
TURN_RIGHT
; -----------------------------------------------------------------------------
; DESCRIPTION:		Select the speed to take, based on the speed bits.
; 			
; INPUTS:		Central I/R sensors, loaded on 'speed'
; OUTPUTS:		LED_L, LED_R, last_sensor_on
; CALLED FROM:		SIGUE_LINEA, BUSCA_SALIDA
; CALLS TO:		MOVE_RIGHT_MAX, MOVE_RIGHT_MED,
; 			MOVE_RIGHT_MIN, STOP
; -----------------------------------------------------------------------------
	bsf	LED_R		; Turns on the left movement LED  
	bcf	LED_L		; and turns off the left one

	BSF	last_sensor_on,0	; Sets last_sensor_on register
					; to be checked by SEARCH_LINE
				
	movlw	0x30
	andwf	PORTA,W		; PORTA bit-mask, 0011 0000
	movwf	speed

	movf	speed,W
	sublw	0x30		; 11 = MAX speed
	btfsc	STATUS,Z
	goto	MOVE_RIGHT_MAX

	movf	speed,W
	sublw	0x10		; 10 = MED speed
	btfsc	STATUS,Z
	goto	MOVE_RIGHT_MED

	movf	speed,W
	sublw	0x20		; 01 = MIN speed
	btfsc	STATUS,Z
	goto	MOVE_RIGHT_MIN

	goto	STOP		; 00 = STOP
