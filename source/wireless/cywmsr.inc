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

; FILENAME: cywmsr.inc
; DESCRIPTION: Send/receive functions for the CYWM6935 wireless module
; VERSION: 0.5 final
; DATE: 09/02/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>
; NOTES: We cannot use call Delay8_cycles here because the stack. 
;	 Instead we'll use multiple nop commands.

	ORG	0x400
	
; ---------------------------------------------------------------------------
send
; ---------------------------------------------------------------------------
; DESCRIPTION:		Send a byte over wifi 
; INPUTS:		Payload: The byte to send
; OUTPUTS:		data_tx_ok: 1 if tx ok, 0 if not
; CALLED FROM:		A lot of routines
; CALLS TO:		spi_write, spi_read, delays
; NOTES:
; ---------------------------------------------------------------------------
	movwf	Payload		; Save the data to send in Payload

	movlw	0xFF		; All the bits in SERDES are valid
	movwf	Databyte	; This write is mandatory
	movlw	REG_TX_VALID	; 
	call	spi_write	; REG_TX_VALID register must not be readed!
	call	delay_250us

	movlw	REG_TX_INT_STAT	; Read empty interrupt from the interrupt register
	clrf	data_tx_ok
	call	spi_read	; to clean interrupt table
	call	delay_250us
_config_inte
	movlw	0x01		; Empty interrupt enabled
	movwf	Databyte
	movlw	REG_TX_INT_EN
	call	spi_write
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	movlw	REG_TX_INT_EN	; Read the interrupt register to verify
	call	spi_read	; that empty interrupt was enabled
	sublw	0x01
	btfss	STATUS,Z
	goto	_config_inte
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_send_mode
	movlw	0x50		; TX Mode with Syn Lock Signal Bypass
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	movlw	REG_CONTROL	; Verify TX Mode
	call	spi_read
	sublw	0x50
	btfss	STATUS,Z
	goto	_send_mode

	call	delay_250us	; Wait 350 uS for the sintentizer to
	call	delay_250us	; estabilize and the preamble for 16kbps

	movf	Payload,W	; Load the data to send
	movwf	Databyte
	movlw	REG_TX_DATA
	call	spi_write
_txintec
	movlw	REG_TX_INT_STAT	; Read empty interrupt to check if the
	call	spi_read	; tx has ended
	movwf	wtemp
	btfss	wtemp,0		; Check if empty interrupt is enabled
	goto	_txintec	; if not ask again
_config_intd
	movlw	0x02		; Enable done interrupt
	movwf	Databyte
	movlw	REG_TX_INT_EN
	call	spi_write
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	movlw	REG_TX_INT_EN	; Check done interrupt enabled
	call	spi_read
	sublw	0x02
	btfss	STATUS,Z
	goto	_config_intd

	movlw	0xFF		; Inicialialize counter to FF. 255 will
	movwf	conta		; be the maximun number of tx retries
_txdr	
	movlw	REG_TX_INT_STAT	; Read done interrupt
	call	spi_read
	movwf	wtemp
	decf	conta,F		; Check if the counter has reached the limit
	btfsc	STATUS,Z	; If so, we finish the routine w/o tx.
	goto	_idle_mode
	btfss	wtemp,1
	goto	_txdr		; TX Done ready
	bsf	data_tx_ok,0
_idle_mode
	movlw	0x00		; Setup Iddle mode
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write
	call	delay_250us	; Wait a little

	movlw	REG_CONTROL
	call	spi_read
	sublw	0x00
	btfss	STATUS,Z
	goto	_idle_mode

	movlw	REG_TX_INT_STAT	; Read interrupts register in order to clean it
	call	spi_read
	return

; ---------------------------------------------------------------------------
receive
; ---------------------------------------------------------------------------
; DESCRIPTION:		Receive a byte over wifi 
; INPUTS:		-
; OUTPUTS:		Payload: the received byte 
;			data_rx_ok: 1 if tx ok, 0 if not
; CALLED FROM:		A lot of routines
; CALLS TO:		spi_write, spi_read, delays
; NOTES:
; ---------------------------------------------------------------------------
	movlw	REG_RX_INT_STAT	; Read interrupts register in order to clean it
	call	spi_read
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_config_intr
	movlw	0x03		; Enable RXFULL and EOF A interrupts
	movwf	Databyte
	movlw	REG_RX_INT_EN
	call	spi_write

	movlw	REG_RX_INT_EN	; Check that the interrupts has been enabled
	call	spi_read
	sublw	0x03
	btfss	STATUS,Z
	goto	_config_intr
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_recv_mode
	movlw	0x90		; RX Mode with Syn Lock Signal bypass 
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write

	movlw	REG_CONTROL
	call	spi_read
	sublw	0x90
	btfss	STATUS,Z
	goto	_recv_mode
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	call	delay_250us	; 200usec syth settle + 35usec recv ready
_rc_eofful
	movlw	REG_RX_INT_STAT ; Wait for EOF or FULL interrupts to be
	call	spi_read	; received
	movwf	wtemp		; Save the data received in wtemp
	andlw	0x03		; Mas for reading only last 2 bits
	iorlw	0x03		; We use an OR to see if one or anoter
	btfsc	STATUS,Z
	goto	_rc_eofful	; If none is active, read again

	btfss	wtemp,0
	goto	_rc_eofful

	movlw	REG_RX_DATA_A	; Read the received data
	call	spi_read
	movwf	Payload		; And save it to payload variable

	movlw	0x00		; Idle mode
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write
	bsf	data_rx_ok,0	; We have read a data
	return

; ---------------------------------------------------------------------------
receiveNB
; ---------------------------------------------------------------------------
; DESCRIPTION:		Receive a byte over wifi. Non blocking variant
; INPUTS:		-
; OUTPUTS:		Payload: the received byte 
;			data_rx_ok: 1 if tx ok, 0 if not
; CALLED FROM:		A lot of routines
; CALLS TO:		spi_write, spi_read, delays
; NOTES:
; ---------------------------------------------------------------------------
	movlw	REG_RX_INT_STAT	; Read interrupts register in order to clean it
	call	spi_read
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_config_intrNB
	movlw	0x03		; Enable RXFULL and EOF A interrupts
	movwf	Databyte
	movlw	REG_RX_INT_EN
	call	spi_write

	movlw	REG_RX_INT_EN	; Check that the interrupts has been enabled
	call	spi_read
	sublw	0x03
	btfss	STATUS,Z
	goto	_config_intrNB
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_recv_modeNB
	movlw	0x90		; RX mode with Syn Lock Signal bypass
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write

	movlw	REG_CONTROL
	call	spi_read
	sublw	0x90
	btfss	STATUS,Z
	goto	_recv_modeNB
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	call	delay_250us	; 200usec syth settle + 35usec recv ready
_rc_eoffulNB
	movlw	REG_RX_INT_STAT ; Wait for EOF or FULL interrupts to be
	call	spi_read	; received
	movwf	wtemp		; Save the data received in wtemp
	andlw	0x03		; Mask for reading only last 2 bits
	iorlw	0x03		; We use an OR to see if one or anoter
	btfsc	STATUS,Z
	return			; If none is active, exit routine

	btfss	wtemp,0
	return

	movlw	REG_RX_DATA_A	; Read the received data
	call	spi_read
	movwf	Payload		; And save it to payload variable

	movlw	0x00		; Idle mode
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write
	bsf	data_rx_ok,0	; We have read a data
	return
