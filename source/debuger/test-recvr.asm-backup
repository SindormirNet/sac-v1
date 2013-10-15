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

; FILENAME: test-recvr.asm
; DESCRIPTION: Debug control wich 'prints' with leds the last message transmited
; VERSION: 0.4
; DATE: 09/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>

	__CONFIG _CONFIG1, _WDT_OFF & _PWRTE_ON & _INTRC_IO & _MCLR_ON & _BODEN_ON & _LVP_OFF & _CPD_OFF & 	_WRT_PROTECT_OFF & _DEBUG_OFF & _CCP1_RB0 & _CP_OFF
	__CONFIG _CONFIG2, _FCMEN_ON & _IESO_ON

	include <p16f88.inc>
	include "../common/temps.inc"
	include "../common/sac.h"
	include "../wireless/cywusb6935.inc"
	include "../wireless/cywmsr.inc"
	include "../wireless/cyspi.inc"

	list p=16F88
	
	ORG	0x00
	
Setup
	bcf	STATUS,RP1	; Bank 1 selection
	bsf	STATUS,RP0 	
	movlw	b'01100110'	; 4Mhz internal clock
	movwf	OSCCON 
	movlw	b'00100000'	; PORTA I/O Setup
	movwf	TRISA 
	movlw	b'00000011'	; PORTB I/O Setup
	movwf	TRISB 
	movlw	b'00000111'	; No Analog comparators
	movwf	CMCON
	movlw   b'00000000'
	movwf   ANSEL
	movlw   b'00000000'
	movwf   ADCON0
	movlw	b'11000000'	; SPI Configuration
	movwf	SSPSTAT 
	bcf	STATUS,RP0 	; Bank 0 selection
	movlw	b'00100000'	; SPI Enabled, CKP=0.Freq = OSC/4
	movwf	SSPCON
	movlw	0x80		; Load 0x80 to offset (+1000000)
	movwf	Offset
	bsf	SPI_SS		; Slave select high (Disabled) 
	call	delay_1_sec	; Wait for SPI initialization 
	call	Radio_reset
	call	Radio_init
	call	delay_1_sec	; Wait for Wireless estabilization
	goto	Main

isend_mode
	call	send
	btfsc	data_tx_ok,0	; Send all the time
	return
	call	delay_0.05_sec	; Was 0.2 sec
	goto	isend_mode	; Until the packet is send

ireceive_mode
	call	receiveNB
	btfss	data_rx_ok,0	; Listen all the time
	goto	ireceive_mode	; Until the packet is received

	movf	Payload,F	; Is the received data null?
	btfsc	STATUS,Z
	goto	ireceive_mode	; In that case discard it
	return

; ---------------------------------------------------------------------------
Parking_send
; ---------------------------------------------------------------------------
; DESCRIPTION:		This routine sends messages to the vacios for them
;			to go exit 1 and exit 3. Some sort of parking command
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		T= 1	E= Exit
;			|00TDD0EE|		D= Destination Vehicle ID 
;			----------
; ---------------------------------------------------------------------------
	movwf	temp2		; Save ID on temp2
	call	delay_0.2_sec
	clrf	datagram	; Clear the datagram

	btfsc	temp2,0		; If ID is 1 we'll go to exit 0
	movlw	0x00
	btfsc	temp2,1		; If ID is 2 we'll go to exit 2
	movlw	0x02
	movwf	datagram

	bcf	datagram,2	; Set to 0 datagram bit 2

	btfsc	temp2,0		; Fill the destination id field with
	bsf	datagram,3	; the contents of the vacio_id variable
	btfsc	temp2,1		; bits 3 and 4
	bsf	datagram,4

	btfsc	temp2,0		; Ugly code for fixing the ID sending. 
	bcf	datagram,4	; Don't know how to fix it

	bsf	datagram,5	; Set to 1 datagram bit 5
	bcf	datagram,6	; Set to 0 datagram bit 6
	bcf	datagram,7	; Set to 0 datagram bit 7

	movf	datagram,W
	movwf	PORTA		; Display the data on PORTA
	call	isend_mode	; Send the datagram

	call	delay_0.2_sec

	movf	datagram,W	; Send the datagram again
	call	isend_mode
	return

park				; Infinite loop sending the park command
	movlw	0x01
	call	Parking_send
	movlw	0x02
	call	Parking_send
	call	delay_0.53_sec
	goto	park

Main
	call	ireceive_mode	; Reveive data

	btfsc	Payload,5	; Change PORTA,5 by PORTB,7
	bsf	PORTB,7
	btfss	Payload,5
	bcf	PORTB,7
	movf	Payload,W	; Display the datagram until next one
	movwf	PORTA		; arrives

	btfss	PORTB,0		; Is park button pressed?
	call	park
	goto	Main

	END
