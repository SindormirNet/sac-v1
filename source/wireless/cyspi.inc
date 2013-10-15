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

; FILENAME: cyspi.inc
; DESCRIPTION: SPI read/write routines for CYWM6935 wireless module
; VERSION: 0.1
; DATE: 09/02/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>
; NOTES: We cannot use call Delay8_cycles here because the stack. 
;	 Instead we'll use multiple nop commands.

	ORG	0x1C0
	
spi_write
	bcf	SPI_SS		; nSS bit fall
	nop
	nop 
	iorwf	Offset,W	; Mask indicating writing operation
	movwf	SSPBUF		; Send the direction we want to write to
	nop			; Wait for the 8 bits to be sent
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	movf	Databyte,W	; Load data byte into buffer
	movwf	SSPBUF
	nop			; Wait for the 8 bits to be sent
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bsf	SPI_SS	 	; nSS bit rise
	return

spi_read
	bcf	SPI_SS		; nSS bit fall
	andlw	0x3F		; 2 MSB must be 0
	movwf	SSPBUF		; Send the direction we want to read from
	nop			; Wait for the 8 bits to be sent
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	movwf	SSPBUF		; Load again the same data for generating 
	nop			; clock pulses to get the response
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bsf	SPI_SS		; nSS bit rise

	bsf	STATUS,RP0
	btfss	SSPSTAT,BF	; Wait untill SPI buffer is full
	goto	$-1
	bcf	STATUS,RP0
	movf	SSPBUF,W	; Load received data into w
	return	
