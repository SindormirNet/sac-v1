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

; FILENAME: led-pos.inc
; DESCRIPTION: UCC led control routines
; VERSION: 0.3
; DATE: 18/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>

	ORG	0x600

green_no_sector
	bcf	PORTA,6
	bcf	PORTA,7
	bsf	PORTB,6
	bsf	PORTB,7
	movlw	b'00000000'
	movwf	green_sectors
	bcf	ahead,0
	return	

green_s1
	bcf	PORTA,6
	bcf	PORTA,7
	bcf	PORTB,6
	bcf	PORTB,7
	movlw	b'00000001'
	movwf	green_sectors

	bsf	ahead,0		; Green ahead default
	btfsc	red_sectors,0	; Is red in this sector?
	bcf	ahead,0		; Then, red ahead
	return

green_s2
	bsf	PORTA,6
	bcf	PORTA,7
	bcf	PORTB,6
	bcf	PORTB,7
	movlw	b'00000010'
	movwf	green_sectors

	bsf	ahead,0		; Green ahead default
	btfsc	red_sectors,1	; Is red in this sector?
	bcf	ahead,0		; Then, red ahead
	return

green_s3
	bcf	PORTA,6
	bsf	PORTA,7
	bcf	PORTB,6
	bcf	PORTB,7
	movlw	b'00000100'
	movwf	green_sectors

	bsf	ahead,0		; Green ahead default
	btfsc	red_sectors,2	; Is red in this sector?
	bcf	ahead,0		; Then, red ahead
	return

green_s4
	bsf	PORTA,6
	bsf	PORTA,7
	bcf	PORTB,6
	bcf	PORTB,7
	movlw	b'00001000'
	movwf	green_sectors
	
	bsf	ahead,0		; Green ahead default
	btfsc	red_sectors,3	; Is red in this sector?
	bcf	ahead,0		; Then, red ahead
	return

green_s5
	bcf	PORTA,6
	bcf	PORTA,7
	bsf	PORTB,6
	bcf	PORTB,7
	movlw	b'00010000'
	movwf	green_sectors

	bsf	ahead,0		; Green ahead default
	btfsc	red_sectors,4	; Is red in this sector?
	bcf	ahead,0		; Then, red ahead
	return

green_s6
	bsf	PORTA,6
	bcf	PORTA,7
	bsf	PORTB,6
	bcf	PORTB,7
	movlw	b'00100000'
	movwf	green_sectors

	bsf	ahead,0		; Green ahead default
	btfsc	red_sectors,5	; Is red in this sector?
	bcf	ahead,0		; Then, red ahead
	return

green_e0
	bcf	PORTA,6
	bsf	PORTA,7
	bsf	PORTB,6
	bcf	PORTB,7
	movlw	b'00000000'
	movwf	green_sectors
	bcf	ahead,0
	return

green_e1
	bsf	PORTA,6
	bsf	PORTA,7
	bsf	PORTB,6
	bcf	PORTB,7
	movlw	b'00000000'
	movwf	green_sectors
	bcf	ahead,0
	return

green_e2
	bcf	PORTA,6
	bcf	PORTA,7
	bcf	PORTB,6
	bsf	PORTB,7
	movlw	b'00000000'
	movwf	green_sectors
	bcf	ahead,0
	return

green_e3
	bsf	PORTA,6
	bcf	PORTA,7
	bcf	PORTB,6
	bsf	PORTB,7
	movlw	b'00000000'
	movwf	green_sectors
	bcf	ahead,0
	return	

red_no_sector
	movlw	0x0D
	addwf	PORTA,F
	movlw	b'00000000'
	movwf	red_sectors
	bcf	ahead,1
	return

red_s1
	movlw	0x00
	addwf	PORTA,F
	movlw	b'00000001'
	movwf	red_sectors

	bsf	ahead,1		; Red ahead default
	btfsc	green_sectors,0 ; Is green in this sector?
	bcf	ahead,1		; Then, green ahead
	return

red_s2
	movlw	0x01
	addwf	PORTA,F
	movlw	b'00000010'
	movwf	red_sectors

	bsf	ahead,1		; Red ahead default
	btfsc	green_sectors,1 ; Is green in this sector?
	bcf	ahead,1		; Then, green ahead
	return

red_s3
	movlw	0x02
	addwf	PORTA,F
	movlw	b'00000100'
	movwf	red_sectors

	bsf	ahead,1		; Red ahead default
	btfsc	green_sectors,2 ; Is green in this sector?
	bcf	ahead,1		; Then, green ahead
	return

red_s4
	movlw	0x03
	addwf	PORTA,F
	movlw	b'00001000'
	movwf	red_sectors

	bsf	ahead,1		; Red ahead default
	btfsc	green_sectors,3 ; Is green in this sector?
	bcf	ahead,1		; Then, green ahead
	return

red_s5
	movlw	0x04
	addwf	PORTA,F
	movlw	b'00010000'
	movwf	red_sectors

	bsf	ahead,1		; Red ahead default
	btfsc	green_sectors,4 ; Is green in this sector?
	bcf	ahead,1		; Then, green ahead
	return

red_s6
	movlw	0x05
	addwf	PORTA,F
	movlw	b'00100000'
	movwf	red_sectors

	bsf	ahead,1		; Red ahead default
	btfsc	green_sectors,5 ; Is green in this sector?
	bcf	ahead,1		; Then, green ahead
	return

red_e0
	movlw	0x06
	addwf	PORTA,F
	movlw	b'00000000'
	movwf	red_sectors
	bcf	ahead,1
	return
	
red_e1
	movlw	0x07
	addwf	PORTA,F
	movlw	b'00000000'
	movwf	red_sectors
	bcf	ahead,1
	return

red_e2
	movlw	0x08
	addwf	PORTA,F
	movlw	b'00000000'
	movwf	red_sectors
	bcf	ahead,1
	return
	
red_e3
	movlw	0x09
	addwf	PORTA,F
	movlw	b'00000000'
	movwf	red_sectors
	bcf	ahead,1
	return

green_demo_sector
	call	green_s1
	call	delay_0.1_sec

	call	green_s2
	call	delay_0.1_sec

	call	green_s3
	call	delay_0.1_sec

	call	green_s4
	call	delay_0.1_sec

	call	green_s5
	call	delay_0.1_sec

	call	green_s6
	call	delay_0.1_sec

	call	green_e0
	call	delay_0.1_sec

	call	green_e1
	call	delay_0.1_sec

	call	green_e2
	call	delay_0.1_sec

	call	green_e3
	call	delay_0.1_sec

	call	green_no_sector
	return

red_demo_sector
	call	red_s1
	call	delay_0.1_sec

	call	red_s2
	call	delay_0.1_sec

	call	red_s3
	call	delay_0.1_sec

	call	red_s4
	call	delay_0.1_sec

	call	red_s5
	call	delay_0.1_sec

	call	red_s6
	call	delay_0.1_sec

	call	red_e0
	call	delay_0.1_sec

	call	red_e1
	call	delay_0.1_sec

	call	red_e2
	call	delay_0.1_sec

	call	red_e3
	call	delay_0.1_sec

	call	red_no_sector
	return
