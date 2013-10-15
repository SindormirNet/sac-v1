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

; FILENAME: ucc.asm
; DESCRIPTION: UCC control main code
; VERSION: 0.9
; DATE: 24/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>

	__CONFIG _CONFIG1, _WDT_OFF & _PWRTE_ON & _INTRC_IO & _MCLR_OFF & _BODEN_ON & _LVP_OFF & _CPD_OFF & 	_WRT_PROTECT_OFF & _DEBUG_OFF & _CCP1_RB0 & _CP_OFF
	__CONFIG _CONFIG2, _FCMEN_ON & _IESO_ON

	include <p16f88.inc>
	include "../common/temps.inc"
	include "../common/sac.h"
	include "../wireless/cywusb6935.inc"
	include "../wireless/cywmsr.inc"
	include "../wireless/cyspi.inc"
	include "control-panel.inc"
	include "led-pos.inc"

	list p=16F88
	
	ORG	0x00
	goto	Setup

	ORG	0x04
	goto	Main
	
	ORG	0x10
	
Setup
	clrf	PORTB
	clrf	PORTA
	
	bcf	STATUS,RP1
	bsf	STATUS,RP0 	; Bank 1
	movlw	b'01100110'	; 4Mhz internal clock config
	movwf	OSCCON 
	movlw	b'00100000'	; PORTA Setup
	movwf	TRISA 
	movlw	b'00000011'	; PORTB Setup
	movwf	TRISB 
	movlw	b'00000111'	; No analog comparators
	movwf	CMCON
	movlw   b'00000000'     ; No analog comparators
	movwf   ANSEL
	movlw   b'00000000'     ; No analog comparators
	movwf   ADCON0
	movlw	b'11000000'	; SPI configuration
	movwf	SSPSTAT 
	bcf	STATUS,RP0 	; Bank 0
	movlw	b'00100000'	; SPI enabled, CKP=0. Clock idles low 
	movwf	SSPCON		; Freq = OSC/4
	movlw	0x80		; Load 0x80 into write offset (+1000000)
	movwf	Offset
	bsf	SPI_SS		; Slave select disabled

	call	red_demo_sector

	call	Radio_reset
	call	Radio_init

	call	green_demo_sector

	call	green_no_sector
	movlw	0xF0		; Clear low PORTA nibble before displaying red
	andwf	PORTA,F
	call	red_no_sector
	goto	Main

p_send_mode
	call	send
	btfsc	data_tx_ok,0	; Send the data until it gets delivered
	return
	goto	p_send_mode

p_receive_mode
	call	receive
	btfss	data_rx_ok,0	; Listen for data until it gets received
	goto	p_receive_mode
	movf	Payload,W	
	return

eval_pos
	; Is the received data a position?
	btfsc	Payload,5	; If datagram type is 1 ignore the datagram
	return
	btfsc	Payload,6	; IS the VACIO 1? Then display green leds
	call	Green_display
	btfsc	Payload,7	; IS the VACIO 2? Then display green reds
	call	Red_display
	return

eval_cp
	; Is the received data a position?
	btfss	Payload,5	; If datagram type is 0 ignore the datagram
	return
	btfss	Payload,4	; Bits 3&4 must be asserted for the 
	return			; datagram to be a CP request.
	btfss	Payload,3
	return
	
	btfsc	Payload,6	; IS the VACIO 1? Then display CP with green leds
	call	Green_cp
	btfsc	Payload,7	; IS the VACIO 2? Then display CP with red leds
	call	Red_cp
	return

eval_t
	; Is the received data a position?
	btfss	Payload,5	; If datagram type is 0 ignore the datagram
	return
	btfsc	Payload,4	; Bits 3&4 must be asserted for the 
	return			; datagram to be a CP request.
	btfsc	Payload,3
	return
	
	btfsc	Payload,6	; IS the VACIO 1? Then display T exit with green leds
	call	Green_display_t
	btfsc	Payload,7	; IS the VACIO 2? Then display T exit with red leds
	call	Red_display_t
	return

eval_rtv
	movf	red_sectors,W
	subwf	green_sectors,W
	btfss	STATUS,Z	; Are the sectors not the same?
	goto	_rtv_end	; Then, end RTV

	movf	ahead,W		; Anybody ahead?
	btfsc	STATUS,Z
	goto	_rtv_end

	sublw	0x03		; Both ahead?
	btfsc	STATUS,Z	; Then, end RTV
	goto	_rtv_end		
				
	btfsc	ahead,1		; VACIO1 Ahead
	movlw	0x01
	btfsc	ahead,0		; VACIO2 Ahead
	movlw	0x02
_rtv_send
	call	RTV_send
	return
_rtv_end
	movlw	0x02		; Prepare the destination of the RTV
	btfsc	rtv_status,0	; depending on rtv_status (See envia_rtv)
	movlw	0x01

	movf	rtv_status,F
; 	btfss	STATUS,Z	; Don't send the datagram if nobody is in RTV
	call	RTV_end_send	; (Clean states on en [green|red]_display)
	return

Main
	call	p_receive_mode	; First listen
	
	movf	Payload,W	
	andlw	b'11000000'
	btfsc	STATUS,Z	; Discard all datagrams beginning with 00
	goto	Main
	sublw	b'11000000'
	btfsc	STATUS,Z	; Discard all datagrams beginning with 11
	goto	Main
	
	call	eval_pos	; Check in circuit positions
	call	eval_cp		; Check CP positions 
	call	eval_t		; Check T exit positions
	call	eval_rtv	; Check for collisions
	goto	Main

	END
