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

; FILENAME: control-vacio.asm
; DESCRIPTION: General VACIO logic & Control + Crypress CYWM6935 control
; VERSION: 1.0
; DATE: 24/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>

	__CONFIG _CONFIG1, _WDT_OFF & _PWRTE_ON & _INTRC_IO & _MCLR_OFF & _BODEN_ON & _LVP_OFF & _CPD_OFF & _WRT_PROTECT_OFF & _DEBUG_OFF & _CCP1_RB0 & _CP_OFF
	__CONFIG _CONFIG2, _FCMEN_ON & _IESO_ON

	#define BOT_ID		0x01	; Change this for robot 2

	include <p16f88.inc>
	include "../common/sac.h"
	include "../common/temps.inc"
	include "../wireless/cywusb6935.inc"
	include "../wireless/cywmsr.inc"
	include "../wireless/cyspi.inc"

	list p=16F88
	
	ORG	0x00

; -----------------------------------------------------------------------------
Setup
; -----------------------------------------------------------------------------
; DESCRIPTION:		I/O Port and registers setup
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		-
; CALL TO:		Logic_Reset
; -----------------------------------------------------------------------------

	bcf	STATUS,RP1
	bsf	STATUS,RP0 	; Bank 1

	movlw	b'01100110'	; 4Mhz
	movwf	OSCCON 
        movlw   b'01111110'     ; PORTA Setup
	movwf	TRISA 
	movlw   b'00000011'     ; PORTB Setup
	movwf	TRISB 
	movlw	b'00000111'	; No Analogue comparators
	movwf	CMCON
	movlw   b'00000000'     ; No Analogue comparators
	movwf   ANSEL
	movlw   b'00000000'     ; No Analogue comparators
	movwf   ADCON0
	movlw	b'11000000'	; SPI Configuration
	movwf	SSPSTAT 
	bcf	STATUS,RP0 	; Bank 0 

	movlw	b'00100000'	; Activate SPI, CKP=0
	movwf	SSPCON

	clrf	PORTB
	clrf	PORTA

	movlw	0x80		; offset = 1000000
	movwf	Offset
	bsf	SPI_SS		; Slave select asserted (Disabled) 
	call	delay_1_sec	; SPI settle time

	bsf	BUZZER		; Beep until Radio gets started
	call	Radio_reset
	call	Radio_init
	bcf	BUZZER		; Radio initialization ok

	goto	Logic_Reset

; -----------------------------------------------------------------------------
Logic_Reset
; -----------------------------------------------------------------------------
; DESCRIPTION:		Reset all the vars to default values
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:		Setup
; CALLS TO:		Config_vars
; -----------------------------------------------------------------------------
	clrf	PORTB		; Clean I/O Ports
	clrf	PORTA
	clrf	bot_speed	; Reset values to its defaults
	clrf	pos
	clrf	switch_pos
	clrf	switch_reset
	clrf	rtv_status

	bsf	MANDA_RESET	; Send reset to motor control uC. This is 
	call	delay_0.03_sec	; needed in order to display speed and exit in
	bcf	MANDA_RESET	; the same I/O pins

	movlw	b'00000111'	; Load default exit
	movwf	bot_exit	; Destination: 111 (free-style mode)

	movlw	BOT_ID		; Load the robot ID
	movwf	vacio_id

	movlw	0x03		; Speed=Max by default
	movwf	bot_speed

	movlw	0x00		; Reset the position for this robot in the UC
	call	Send_position

	goto	Config_vars

; -----------------------------------------------------------------------------
Config_vars
; -----------------------------------------------------------------------------
; DESCRIPTION:		Initial user variables setup
; INPUTS:		Increment and start/stop buttons
; OUTPUTS:		DP Led, Display
; CALLED FROM:		Logic_Reset
; CALLS TO:		Show_exit_ini, Speed_Control, Main
; -----------------------------------------------------------------------------
	movf	bot_exit,W	; Load bot_exit in W as a parameter for 
	call	Show_exit_ini	; Show_exit_ini routine

	btfsc	INC_BOT		; Check increment button pressed
	call	Inc_pressed

	btfss	START_STOP	; Don't start until Start button pressed
	goto	Config_vars

	call	delay_0.53_sec	; Wait for release button

	btfsc	START_STOP	; If start/stop button is still pressed, we
	goto	Id_swap		; will change robot ID

	call	Speed_Control	; Setup initial speed
	
	bsf	SALIDA		; Pulse pin indicating motor control uC
	call	delay_0.03_sec	; that we are starting!
	bcf	SALIDA

	goto	Main

; -----------------------------------------------------------------------------
Id_swap
; -----------------------------------------------------------------------------
; DESCRIPTION:		Change robot's ID from 1 to 2 and viceversa
; INPUTS:		Hold start/stop button
; OUTPUTS:		7-Seg display
; CALLED FROM:		Config_vars
; CALLS TO:		Show_exit_ini, Config_vars
; -----------------------------------------------------------------------------
	movf	vacio_id,W	; Display first current ID
	call	Show_exit_ini	; We'll use this function anyway

	call	delay_1_sec	; Wait for release
	btfss	START_STOP	; If the button isn't still pressed
	goto	Config_vars	; go back to previus state

	btfsc	vacio_id,0	; If the button is still pressed
	movlw	0x02		; Change the ID to the other

	btfsc	vacio_id,1	; If the button is still pressed
	movlw	0x01		; Change the ID to the other

	movwf	vacio_id
	call	Show_exit_ini	; We'll use this function anyway
	call	delay_1_sec	; for display the new ID

	goto	Config_vars	

; ---------------------------------------------------------------------------
Show_exit_ini
; ---------------------------------------------------------------------------
; DESCRITCION:		Shows the configured exit
; INPUTS:		w preload with the data to display
; OUTPUTS:		VEL_MSB and VEL_LSB pins
; CALLED FROM:		Config_vars
; CALLS TO:		-
; ---------------------------------------------------------------------------
	movwf	temp		; Save w en in a temp variable
	btfsc	temp,0
	bsf	VEL_LSB
	btfss	temp,0
	bcf	VEL_LSB
	btfsc	temp,1
	bsf	VEL_MSB
	btfss	temp,1
	bcf	VEL_MSB
	return

; ---------------------------------------------------------------------------
Inc_pressed
; ---------------------------------------------------------------------------
; DESCRIPTION:		Routine called when INC button is pressed. It will
;			increase speed and exit values depending on the user's
;			pulses
; INPUTS:		INC Button
; OUTPUTS:		DP Led of the 7-Seg display
; CALLED FROM:		Config_vars
; CALLS TO:		Show_exit_ini
; ---------------------------------------------------------------------------
	call	delay_0.53_sec	
	btfsc	INC_BOT		; Was it a long press?
	incf	bot_exit,F	; If yes, change the routing mode
	bcf	bot_exit,3	; Mask for limiting the exit to 3 bits
	movf	bot_exit,W	; Load bot_exit in W as parameter
	call	Show_exit_ini	; to Show_exit_ini routine

	btfss	INC_BOT
	decf	bot_speed,F	; If not. We'll increase the value
	movlw	b'00000011'
	andwf	bot_speed,F	; Mask for limiting the speed to 2 bits
	call	delay_0.53_sec
	return

; ---------------------------------------------------------------------------
Read_pos
; ---------------------------------------------------------------------------
; DESCRIPTION:		Read encoders redundantly
; INPUTS:		I1, I2 and I3 sensors
; OUTPUTS:		pos variable with the sector read
; CALLED FROM:		Main
; CALLS TO:		Send_position
; ---------------------------------------------------------------------------
	movf	PORTA,W		; Read sensors
	movwf	pos1		; Save them in pos1
	movlw	b'00001110'	; Mask for reading position sensors
	andwf	pos1,F
	movf	pos1,F		; Check if we've read all 0s
	btfsc	STATUS,Z
	return			; If so, then exit this routine

	movlw	b'00001110'	; Check if we've read all 1s
	subwf	pos1,W
	btfsc	STATUS,Z
	return			; If so, the exit this routine

	btfsc	bot_exit,2	; Chet bot_exit, bit 2. If that bit is active
	return			; we will not read the position (Freestyle)

	btfsc	SALIDA		; If in the exit sector we don't read more 
	return			; sectors. THIS COULD CAUSE PROBLEMS!

	bcf	SALIDA		; Clear EXIT byte 

	movf	PORTA,W		; Read sensors
	movwf	pos1		; Save the result into pos1
	movlw	b'00001110'	; Mask for reading position bits
	andwf	pos1,F		; Copy the result into pos1

	call	delay_0.06_sec	; Wait 60ms and read again (2nd time)

	movf	PORTA,W		; Read sensors
	movwf	pos2		; Save the result into pos2
	movlw	b'00001110'	; Mask for reading position bits
	andwf	pos2,F		; Copy the result into pos2

	call	delay_0.06_sec	; Wait 60ms and read again (3rd time)

	movf	PORTA,W		; Read sensors
	movwf	pos3		; Save the result into pos3
	movlw	b'00001110'	; Mask for reading position bits
	andwf	pos3,F		; Copy the result into pos3

	movf	pos2,W		; Compare the first two read positions
	subwf	pos1,W
	btfss	STATUS,Z	; Read until are equal
	goto	Read_pos

	movf	pos3,W		; Compare with the tirth read position
	subwf	pos2,F
	btfss	STATUS,Z	; Read until are equal
	goto	Read_pos
	movwf	pos		; Save the position in pos variable

	call	Pos_table	; Put in w the exit correspoding with the current
	subwf	bot_exit,W	; sector. Then compare it with the user programmed
	btfsc	STATUS,Z	; exit. If they are the same this means that we've
	bsf	SALIDA		; to get this exit

	rrf	pos,W		; Rotate pos and save it in W for representing it
	call	Send_position	

	btfss	vacio_id,0	; Wait depending on the Robot ID
	call	delay_0.71_sec
	btfss	vacio_id,1	; This prevents overlaping messages
	call	delay_0.53_sec

	rrf	pos,W		; Send the position again
	call	Send_position	
	return

; ---------------------------------------------------------------------------
Pos_table
; ---------------------------------------------------------------------------
; DESCRIPTION:		Returns the exit corresponding to the sector
;			a return of 7 means that no there's no exit on that sector
; INPUTS:		pos variable  with the sector to check
; OUTPUTS:		w with the information about the exit
; CALLED FROM:	Lee_pos
; CALLS TO:			-
; NOTES:		Sect_V	Sect_R	Encoder Exit	Exit_type
; 			1	1	001	-	Input
; 			2	3	011	0	Output T	
; 			3	2	010	1	Output B	
; 			4	6	110	-	Input
; 			5	4	100	2	Output T
; 			6	5	101	3	Output B
;----------------------------------------------------------------------------
	rrf	pos,W		; Rotate pos in order jump to work
	andlw	b'00000111'	; Mask for the last three bits
	addwf	PCL,F
	retlw	b'00000111'	; Case 0 real: INVALID SECTOR
	retlw	b'00000111'	; Case 1 real: Input
	retlw	b'00000001'	; Case 2 real: Output 1B
	retlw	b'00000000'	; Case 3 real: Output 0T
	retlw	b'00000010'	; Case 4 real: Output 2T
	retlw	b'00000011'	; Case 5 real: Output 3B
	retlw	b'00000111'	; Case 6 real: Input
	retlw	b'00000111'	; Case 7 real: INVALID SECTOR
	return			; This line should never be reached

; ---------------------------------------------------------------------------
cp_req_eval
; ---------------------------------------------------------------------------
; DESCRIPTION:		Ask for permission for joining to the main track
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-
; NOTES:
;			This is the datagram format:
;			----------		T= 1
;			|00TDD111|		D= Destination Vehicle ID
;			----------
; -------------------------------------------------------------------------
	btfss	vacio_id,0	; Wait a time depending on the Vacio ID
	call	delay_0.71_sec
	btfss	vacio_id,1
	call	delay_0.53_sec

	call	Send_CP		; Send request for joining track
	bsf	BUZZER		; begin beeping!
	call	modo_recibe	; Receive the response
	bcf	BUZZER		; stop beeping!

	btfss	Payload,5	; Discard packets with type (bit 5) = 0
	goto	cp_req_eval

	movlw	b'11000000'	; Is the received packet comming from the UCC?
	andwf	Payload,W	; Mask for reading the Source of the packet
	iorlw	b'00000000'	; The two MSB bits should be 00. In other cases
	btfss	STATUS,Z	; discard the packet
	goto	cp_req_eval

	movlw	b'00000111'	; The packet should have a payload of 111
	andwf	Payload,W	; Mask the packet payload
	sublw	b'00000111'	; Check for the payload
	btfss	STATUS,Z
	goto	cp_req_eval

	movlw	b'00011000'	; Is the received packet destination ourselves?
	andwf	Payload,W	; Mask for the packet destination
	movwf	temp
	rlf	temp,F		; When doing rlf, lsb bit become 1
	bcf	temp,0
	swapf	temp,W
	subwf	vacio_id,W
	btfss	STATUS,Z
	goto	cp_req_eval
	goto	_cpdone

; ---------------------------------------------------------------------------
Control_CP
; ---------------------------------------------------------------------------
; DESCRIPTION:		Controls Give way signals
; INPUTS:		D1 and D2 sensors
; OUTPUTS:		Speed variables
; CALLED FROM:		Main
; CALLS TO:		-
; ---------------------------------------------------------------------------
	btfsc	bot_exit,2	; Is 3rd bit of bot_exit enabled? In this case
	return			; we'll never go into Give Way 
_cp_pre_loop
	movlw	b'00111110'	; Mask for all the sensors
	andwf	PORTA,W
	movwf	temp		; WARN: Doublecheck this code!
	sublw	b'00110000'	; Was D1 and D2 asserted?
	btfss	STATUS,Z	; In that case, we're on a Give Way sign
	return

	movlw	b'00111110'	; Mask for I1, I2, I3, D1 and D2 PORTA sensors
	andwf	temp,W		; WARN: Doublecheck this code!
	sublw	b'00111110'	; All of them asserted?
	btfsc	STATUS,Z	; In that case, we're off road. Stop robot.
	bsf	switch_reset,0
	btfsc	STATUS,Z
	return

	call	Pos_table	; Save into W current sector by calling Pos_table
	subwf	bot_exit,W	; 
	btfss	STATUS,Z	; Is position read the same as in pos_table?
	return			; Then do CP, else return

	call	delay_0.03_sec	; Wait 30ms and read again

	movlw	b'00111110'	; Mask for all the sensors
	andwf	PORTA,W
	movwf	temp
	sublw	b'00110000'	; Are D1 and D2 active?
	btfss	STATUS,Z	; In that case we are on a CP
	return

	bcf	VEL_LSB		; Stop both motors
	bcf	VEL_MSB
 	bcf	SALIDA		; And clear "take exit" bits
	call	Send_CP		; Send CP befor in order to display in the track
_cploop
	goto	cp_req_eval	; Do we have the answer?
	goto	_cploop
_cpdone
	bsf	VEL_LSB		; Little trick: Start with max speed for 0.2 secs
	bsf	VEL_MSB		; 
	call	delay_0.2_sec
	bcf	VEL_LSB		; The use the normal (configured) speed
	bcf	VEL_MSB		; 
	clrf	pos		; Forget about the sector in wich we are
	return

; ---------------------------------------------------------------------------
Speed_Control
; ---------------------------------------------------------------------------
; DESCRIPTION:		Talks to the servo PIC indicating the speed
; INPUTS:		Speed variable bot_speed
; OUTPUTS:		output pins VEL_MSB y VEL_LSB
; CALLED FROM:		Main
; CALLS TO:		-
; ---------------------------------------------------------------------------
	btfsc	bot_speed,1	; Shows the speed (MSB)
	bsf	VEL_MSB
	btfss	bot_speed,1
	bcf	VEL_MSB
	btfsc	bot_speed,0	; Shows the speed (LSB)
	bsf	VEL_LSB
	btfss	bot_speed,0
	bcf	VEL_LSB
	return

; ---------------------------------------------------------------------------
Manual_exit
; ---------------------------------------------------------------------------
; DESCRIPTION:		In freestyle mode, this function will be watching for
;			the manual exits
; INPUTS:		D1 Sensor
; OUTPUTS:		Exit Servo PIC bit
; CALLED FROM:		Control_Salida
; CALLS TO:			-
; ---------------------------------------------------------------------------
	btfss	bot_exit,2	; Is bot_exit,2 enabled?
	return			; If not, exit

	movlw	b'00111110'	; Mask for all the sensors
	andwf	PORTA,W		; I1, I2, I3, D1 y D2 from PORTA
	sublw	b'00010000'	; Is the manual exit sensor enabled?
	btfss	STATUS,Z
	return

	call	delay_0.05_sec

	movf	PORTA,W
	andlw	b'00111110'
	sublw	b'00010000'
	btfss	STATUS,Z
	return

	bsf	SALIDA		; Activate exit indication for 2 seconds
	call	delay_1_sec
	call	delay_1_sec
	bcf	SALIDA
	return

; ---------------------------------------------------------------------------
Send_position
; ---------------------------------------------------------------------------
; DESCRIPTION:		Send the current sector via Wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		I= Vacio_ID	T= 0
;			|IITVVSSS|		V= Speed	S= Sector
;			----------
; ---------------------------------------------------------------------------
	clrf	datagram	; Clear datagram
	
	addwf	datagram,F	; Add sector information to bits 0,1,2

	btfsc	bot_speed,0	; Fill in the speed field reading from
	bsf	datagram,3	; bot_speed variable
	btfsc	bot_speed,1	; bits 3,4
	bsf	datagram,4

	bcf	datagram,5	; Type of Datagram (bit 5) = 0

	btfsc	vacio_id,0	; Fill in the ID field reading from 
	bsf	datagram,6	; vacio_id variable
	btfsc	vacio_id,1	; bits 6,7
	bsf	datagram,7

	btfsc	vacio_id,0	; This code needs to be here in order to send
	bcf	datagram,7	; a correct ID

	movf	datagram,W	; Send the Datagram
	call	modo_envia	

	call	delay_0.2_sec	; Wait for 0.2 seconds

	movf	datagram,W	; Send the Datagram again
	call	modo_envia	

	return

; ---------------------------------------------------------------------------
Send_CP
; ---------------------------------------------------------------------------
; DESCRIPTION:		Sends a CP via wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		I= Vacio_ID	T= 1
;			|IIT11SSS|		V= Speed	S= Sector
;			----------
; ---------------------------------------------------------------------------
	clrf	datagram	; Clear datagram

	rrf	pos,W		; Rotate position to right
	addwf	datagram,F	; Add the position to the datagram bits 0,1,2

	bsf	datagram,3	; Set to 1 datagram type. Bit 3
	bsf	datagram,4	; Set to 1 datagram type. Bit 4
	bsf	datagram,5	; Set to 1 datagram type. Bit 5

	btfsc	vacio_id,0	; Fill in the ID field reading from 
	bsf	datagram,6	; vacio_id variable
	btfsc	vacio_id,1	; bits 6,7
	bsf	datagram,7

	btfsc	vacio_id,0	; This code needs to be here in order to send
	bcf	datagram,7	; a correct ID

	movf	datagram,W	; Send the Datagram
	call	modo_envia	
	return

; ---------------------------------------------------------------------------
Send_T
; ---------------------------------------------------------------------------
; DESCRIPTION:		Sends a T signal (End of track) via wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		I= Vacio_ID	T= 1
;			|IIT00SSS|		V= Speed	S= Sector
;			----------
; ---------------------------------------------------------------------------
	movf	pos,W		; One T-type exit is in sector 100
	andlw	b'00001110'
	sublw	b'00001000'
	btfsc	STATUS,Z
	goto	_e_t

	movf	pos,W		; The other T-type exit is in sector 011 
	andlw	b'00001110'
	sublw	b'00000110'
	btfsc	STATUS,Z
	goto	_e_t

	call	delay_0.1_sec	; Wait

	movlw   b'00111110'     ; Mask for reading the sensors
	andwf   PORTA,W         ; I1, I2, I3, D1 y D2 from PORTA
	sublw   b'00111110'     ; Has been all of them activated?
	btfsc   STATUS,Z        ; In that case, stop the robot
	goto	Logic_Reset
	goto	Main		; Else go back to main
_e_t
	clrf	datagram	; Clear datagram
	
	rrf	pos,W		; Rotate sector to the right
	addwf	datagram,F	; Add sector to the datagram bits 0,1,2

	bcf	datagram,3	; Set to 0 datagram type. Bit 3
	bcf	datagram,4	; Set to 0 datagram type. Bit 4
	bsf	datagram,5	; Set to 1 datagram type. Bit 5

	btfsc	vacio_id,0	; Fill in the ID field reading from
	bsf	datagram,6	; vacio_id variable
	btfsc	vacio_id,1	; bits 6,7
	bsf	datagram,7

	btfsc	vacio_id,0	; This code needs to be here in order to send
	bcf	datagram,7	; a correct ID

	movf	datagram,W	; Send datagram
	call	modo_envia	

	bcf	VEL_MSB		; This block of code allows the UCC to show 
	bcf	VEL_LSB		; the T exit for 10 seconds before desapear
	bsf	MANDA_RESET
	call	delay_0.2_sec
	bcf	MANDA_RESET
	movf	datagram,W	; Send datagram again
	call	modo_envia	
	call	delay_10_sec	; Wait 10 seconds before reseting the robot
	goto    Logic_Reset

modo_envia
	call	send
	btfsc	data_tx_ok,0	; send all the time
	return
	call	delay_0.05_sec		; Was 0.2 sec
	goto	modo_envia	; until it gets send

modo_recibe
	call	receive
	btfss	data_rx_ok,0	; Listen all the time
	goto	modo_recibe	; until we get a response
	movf	Payload,W	
	return

; ---------------------------------------------------------------------------
eval_rtv
; ---------------------------------------------------------------------------
; DESCRIPTION:		Receive and analize RTV and remote control signals
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		RTV packet format:
;			----------		D= Destination	T= 1
;			|00TDD10X|		X= 0 if END, 1 if begin
;			----------		
;
;			Remote Control packet format:
;			----------		D= Destinatio	T= 1
;			|00TDD0EE|		E= Exit to take
;			----------		
; ---------------------------------------------------------------------------
	call	receiveNB	; Receive packets in a non blocking way 

	btfss	Payload,5	; If the packet is not type 1, ignore it
	return

	; If the packet is not from the UCC, ignore it
	movlw	b'11000000'	; Mask
	andwf	Payload,W
	iorlw	b'00000000'	; Two MSB bits must be 00
	btfss	STATUS,Z
	return

	; If the packet is nos directed to us, ignore it
	movlw	b'00011000'	; Mask
	andwf	Payload,W
	movwf	temp
	rlf	temp,F		; When doing RLF, LSB bit is set to 1
	bcf	temp,0
	swapf	temp,W
	subwf	vacio_id,W
	btfss	STATUS,Z
	return

	; If the packet has a payload of 101, we are at the beginning of the RTV
	movlw	b'00000111'
	andwf	Payload,W	; Mask
	sublw	b'00000101'
	btfsc	STATUS,Z
	goto	_rtv_ini

	; If the packet has a payload of 100, we are at the end of the RTV
	movlw	b'00000111'
	andwf	Payload,W	; Mask
	sublw	b'00000100'
	btfsc	STATUS,Z
	goto	_rtv_end

	; If the packet has a payload of 0XX, we are in remote control mode
	movlw	b'00000100'
	andwf	Payload,W	; Mask
	sublw	b'00000000'
	btfsc	STATUS,Z
	goto	_cr
	return			; Other kinds of packets are ignored
_cr	
	movlw	b'00000011'
	andwf	Payload,W	; Save exit sector in W
	movwf	bot_exit	; and load it into bot_exit variable
	movlw	0x03		; Also, set maximum speed
	movwf	bot_speed
	return
_rtv_ini
	btfsc	rtv_status,0	; If we are on RTV mode, return
	return

	; Don't do RTv if we are taking a exit
	btfsc	SALIDA
	return

	; Reduce speed
	bsf	rtv_status,0	; Mark RTV mode
	movf	bot_speed,W	; Save original speed
	movwf	bot_speed_orig
	movlw	0x01		; and load RTV speed
	movwf	bot_speed
	bsf	BUZZER		; Beep
	return	
_rtv_end
	bcf	BUZZER		; Stop beeping
	btfss	rtv_status,0	; If we're already in RTV mode, exit
	return
	bcf	rtv_status,0	; Unset RTV mode
	movf	bot_speed_orig,W; Restore original speed
	movwf	bot_speed
	return	

; -----------------------------------------------------------------------------
Main
; -----------------------------------------------------------------------------
; DESCRIPTION:          Main function controlling all the robot routines
; INPUTS:		-
; OUTPUTS:              -
; CALLED FROM:		Config_vars
; CALLS TO:		Logic_Reset, Control_CP, Control_Velocidad, Lee_pos,
;                       Control_Salida
; -----------------------------------------------------------------------------
	btfsc   START_STOP      ; Is the stop button pressed?
	goto    Logic_Reset	; TODO: Do this with interrupts

	movlw   b'00111110'     ; Mask for all the sensors
	andwf   PORTA,W         ; I1, I2, I3, D1 y D2 from PORTA
	sublw   b'00111110'     ; Are all of them activated?
	btfsc   STATUS,Z        ; Stop the robot in that case
	goto    Send_T		; (17/03/09)

	call    Control_CP      ; Are we in a CP? (Give way sign)

	btfsc   switch_reset,0  ; Read switch_reset variable to see if during
	goto    Send_T		; CP a end of track has been detected

	call	eval_rtv	; Has a RTV message arrive?

	call    Speed_Control	; Setup speed

	call    Read_pos	; Read position

	call    Manual_exit	; Do we do manual exit? (Freestyle mode)

	goto    Main

	END
