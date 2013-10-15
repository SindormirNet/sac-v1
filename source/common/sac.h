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

; FILENAME: sac.h
; DESCRIPTION: Defines and common variables file
; VERSION: 0.1
; DATE: 18/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>

	#define START_STOP	PORTB,0		; I
	#define SPI_MISO	PORTB,1		; I
	#define SPI_MOSI	PORTB,2		; O
	#define BUZZER		PORTB,3		; O
	#define SPI_CLK		PORTB,4		; O
	#define SPI_SS		PORTB,5		; O
	#define VEL_MSB		PORTB,6		; O
	#define MANDA_RESET	PORTB,7		; O

	#define SALIDA		PORTA,0		; O
	#define I1		PORTA,1		; I
	#define I2		PORTA,2		; I
	#define I3		PORTA,3		; I
	#define D1		PORTA,4		; I
	#define D2		PORTA,5		; I
	#define INC_BOT		PORTA,6		; I
	#define VEL_LSB		PORTA,7		; O

	#define REG_ID			0x00
	#define REG_CONTROL		0x03
	#define REG_DATA_RATE		0x04
	#define REG_CONFIG		0x05
	#define REG_SERDES_CTL		0x06
	#define REG_RX_INT_EN		0x07
	#define REG_RX_INT_STAT		0x08
	#define REG_RX_DATA_A		0x09
	#define REG_RX_VALID_A		0x0A
	#define REG_RX_DATA_B		0x0B
	#define REG_RX_VALID_B		0x0C
	#define REG_TX_INT_EN		0x0D
	#define REG_TX_INT_STAT 	0x0E
	#define REG_TX_DATA		0x0F
	#define REG_TX_VALID		0x10
	#define REG_THRESHOLD_L		0x19
	#define REG_THRESHOLD_H		0x1A
	#define REG_WAKE_EN		0x1C
	#define REG_WAKE_STAT		0x1D
	#define REG_ANALOG_CTL		0x20
	#define REG_CHANNEL		0x21
	#define REG_RSSI		0x22
	#define REG_PA			0x23
	#define REG_CRYSTAL_ADJ		0x24
	#define REG_VCO_CAL		0x26
	#define REG_PWR_CTL		0x2E
	#define REG_CARRIER_DETECT	0x2F
	#define REG_CLOCK_MANUAL	0x32
	#define REG_CLOCK_ENABLE	0x33
	#define REG_SYN_LOCK_CNT	0x38

bot_speed	equ	0x20	; Bits 0 y 1 (LSB) -> Speed
bot_exit	equ	0x21	; Exit to take
temp		equ	0x22
temp2		equ	0x23
pos		equ	0x24
pos1		equ	0x25
pos2		equ	0x26
pos3		equ	0x27
switch_pos	equ	0x28
switch_reset	equ	0x29
Offset		equ	0x2A
Databyte	equ	0x2B
Payload		equ	0x2C
Payload_s	equ	0x2D
wtemp		equ	0x3E
conta		equ	0x2F
data_tx_ok	equ	0x30
data_rx_ok	equ	0x31
vacio_id	equ	0x32
datagram	equ	0x33
recv_conta	equ	0x34

green_sectors	equ	0x35
red_sectors	equ	0x36

bot_speed_orig	equ	0x37
rtv_status	equ	0x43	; Any VACIO in rtv state?
ahead		equ	0x44	; bit 0 high if VACIO1 ahead, bit 1 high if VACIO2 ahead

CounterA	equ	0x40
CounterB	equ	0x41
CounterC	equ	0x42
