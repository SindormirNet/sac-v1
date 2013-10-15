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

; FILENAME: cywmusb6935.inc
; DESCRIPTION: Radio reset/init functions for the CYWM6935 wireless module
; VERSION: 0.3
; DATE: 11/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>
	
	ORG	0x300
	
Radio_reset
	bsf	SPI_SS
	call	delay_0.0013_sec; pwr_rst and pwr_pd time
	call	delay_0.002_sec ; Wait clock to estabilize
	call	delay_0.002_sec
	return

Radio_init
	movlw	REG_ID
	call	spi_read
	movwf	PORTA
	sublw	b'00000111'	; Read ID until a 0x07 is retrieved
	btfss	STATUS,Z	
	goto	Radio_init

	movlw	0x01		; Reset all variables to its defaults
	movwf	Databyte
	movlw	REG_ANALOG_CTL
	call	spi_write
_clock_manual
	movlw	0x41		; REG_CLOCK_MANUAL must be loaded with 0x41
	movwf	Databyte	; after each reset
	movlw	REG_CLOCK_MANUAL
	call	spi_write	

	movlw	REG_CLOCK_MANUAL
	call	spi_read
	sublw	0x41
	btfss	STATUS,Z
	goto	_clock_manual
_clock_enable
	movlw	0x41		; REG_CLOCK_MANUAL must be loaded with 0x41
	movwf	Databyte	; after each reset
	movlw	REG_CLOCK_ENABLE
	call	spi_write	

	movlw	REG_CLOCK_ENABLE
	call	spi_read
	sublw	0x41
	btfss	STATUS,Z
	goto	_clock_enable
_analog
	movlw	0x04		; Signal amplifier enabled
	movwf	Databyte
	movlw	REG_ANALOG_CTL
	call	spi_write

	movlw	REG_ANALOG_CTL
	call	spi_read
	sublw	0x04
	btfss	STATUS,Z
	goto	_analog
_vco_cal			; Read only register
	movlw	0xC0		; -5V / +5V VCO adjustment
	movwf	Databyte
	movlw	REG_VCO_CAL
	call	spi_write
_serdes
	movlw	0x08		; SERDES type communication enabled
	movwf	Databyte	; EOF=0
	movlw	REG_SERDES_CTL
	call	spi_write

	movlw	REG_SERDES_CTL
	call	spi_read
	sublw	0x08		
	btfss	STATUS,Z
	goto	_serdes
_reg_config
	movlw	0x03		; Open Source interrupts, high level active
	movwf	Databyte
	movlw	REG_CONFIG
	call	spi_write

	movlw	REG_CONFIG
	call	spi_read
	sublw	0x03
	btfss	STATUS,Z
	goto	_reg_config
_pa
	movlw	0x05		; Med tx power 5 over 7
	movwf	Databyte
	movlw	REG_PA
	call	spi_write

	movlw	REG_PA
	call	spi_read
	sublw	0x05
	btfss	STATUS,Z
	goto	_pa
	call	Radio_init_16kbps
_channel
	movlw	0x19		; Set frequency to 2419MHz 
	movwf	Databyte
	movlw	REG_CHANNEL
	call	spi_write

	movlw	REG_CHANNEL
	call	spi_read
	sublw	0x19
	btfss	STATUS,Z
	goto	_channel
_control2
	movlw	0x12		; Bypass syn lock signal idle mode 
	movwf	Databyte
	movlw	REG_CONTROL
	call	spi_write

	movlw	REG_CONTROL
	call	spi_read
	sublw	0x12
	btfss	STATUS,Z
	goto	_control2
	return

Radio_init_16kbps
	nop
_data_rate
	call	delay_250us
	movlw	0x00		; 64 bits chips per PN code
	movwf	Databyte	; 1 bit per PN code and 6x oversampling
	movlw	REG_DATA_RATE	; This give us a 15.625 kbits/s speed
	call	spi_write

	movlw	REG_DATA_RATE
	call	spi_read
	sublw	0x00
	btfss	STATUS,Z
	goto	_data_rate
_threshold_l
	call	delay_250us
	movlw	0x08		; Low sensibility adjustment
	movwf	Databyte
	movlw	REG_THRESHOLD_L
	call	spi_write

	movlw	REG_THRESHOLD_L
	call	spi_read
	sublw	0x08
	btfss	STATUS,Z
	goto	_threshold_l
_threshold_h
	call	delay_250us
	movlw	0x38		; High sensibility adjustment
	movwf	Databyte
	movlw	REG_THRESHOLD_H
	call	spi_write

	movlw	REG_THRESHOLD_H
	call	spi_read
	sublw	0x38
	btfss	STATUS,Z
	goto	_threshold_h
_synlock
	call	delay_250us
	movlw	0x7D		; 250uS delay between enable TX and preamble
	movwf	Databyte
	movlw	REG_SYN_LOCK_CNT
	call	spi_write

	movlw	REG_SYN_LOCK_CNT
	call	spi_read
	sublw	0x7D
	btfss	STATUS,Z
	goto	_synlock
	return
