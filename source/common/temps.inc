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

; FILENAME: temps.inc
; DESCRIPTION: delay routines for the entire project
; VERSION: 0.2
; DATE: 30/12/08
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>
; NOTES: Delay codes generated by PiKloops

	ORG	0x200

delay_10_sec
	movlw 	D'51'
	movwf 	CounterC
	movlw 	D'237'
	movwf 	CounterB
	movlw 	D'173'
	movwf 	CounterA
delay_10_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_10_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_10_sec_loop 
	decfsz 	CounterC,1
 	goto 	delay_10_sec_loop 
	return

delay_1_sec
	movlw 	D'6'
	movwf 	CounterC
	movlw 	D'24'
	movwf 	CounterB
	movlw 	D'168'
	movwf 	CounterA
delay_1_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_1_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_1_sec_loop 
	decfsz 	CounterC,1
 	goto 	delay_1_sec_loop 
	return

delay_0.71_sec
	movlw 	D'4'
	movwf 	CounterC
	movlw 	D'158'
	movwf 	CounterB
	movlw 	D'12'
	movwf 	CounterA
delay_0.71_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.71_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.71_sec_loop 
	decfsz 	CounterC,1
 	goto 	delay_0.71_sec_loop 
	return

delay_0.53_sec
	movlw 	D'3'
	movwf 	CounterC
	movlw 	D'179'
	movwf 	CounterB
	movlw 	D'73'
	movwf 	CounterA
delay_0.53_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.53_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.53_sec_loop 
	decfsz 	CounterC,1
 	goto 	delay_0.53_sec_loop 
	return

delay_0.2_sec
	movlw 	D'2'
	movwf 	CounterC
	movlw 	D'5'
	movwf 	CounterB
	movlw 	D'183'
	movwf 	CounterA
delay_0.2_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.2_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.2_sec_loop 
	decfsz 	CounterC,1
 	goto 	delay_0.2_sec_loop 
	return

delay_0.1_sec
	movlw 	D'130'
	movwf 	CounterB
	movlw 	D'220'
	movwf 	CounterA
delay_0.1_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.1_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.1_sec_loop 
	return

delay_0.06_sec
	movlw 	D'78'
	movwf 	CounterB
	movlw 	D'235'
	movwf 	CounterA
delay_0.06_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.06_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.06_sec_loop 
	return

delay_0.05_sec
	movlw 	D'65'
	movwf 	CounterB
	movlw 	D'237'
	movwf 	CounterA
delay_0.05_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.05_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.05_sec_loop 
	return

delay_0.03_sec
	movlw 	D'39'
	movwf 	CounterB
	movlw 	D'245'
	movwf 	CounterA
delay_0.03_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.03_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.03_sec_loop 
	return

delay_0.01897_sec
	movlw 	D'25'
	movwf 	CounterB
	movlw 	D'161'
	movwf 	CounterA
delay_0.01897_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.01897_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.01897_sec_loop 
	return

delay_0.01877_sec
	movlw 	D'25'
	movwf 	CounterB
	movlw 	D'95'
	movwf 	CounterA
delay_0.01877_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.01877_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.01877_sec_loop 
	return

delay_0.01868_sec
	movlw 	D'25'
	movwf 	CounterB
	movlw 	D'65'
	movwf 	CounterA
delay_0.01868_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.01868_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.01868_sec_loop 
	return

delay_0.01862_sec
	movlw 	D'25'
	movwf 	CounterB
	movlw 	D'43'
	movwf 	CounterA
delay_0.01862_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.01862_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.01862_sec_loop 
	return

delay_0.01857_sec
	movlw 	D'25'
	movwf 	CounterB
	movlw 	D'28'
	movwf 	CounterA
delay_0.01857_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.01857_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.01857_sec_loop 
	return

delay_0.01837_sec
	movlw 	D'24'
	movwf 	CounterB
	movlw 	D'218'
	movwf 	CounterA
delay_0.01837_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.01837_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.01837_sec_loop 
	return

delay_0.002_sec
	movlw 	D'3'
	movwf 	CounterB
	movlw 	D'147'
	movwf 	CounterA
delay_0.002_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.002_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.002_sec_loop 
	return

delay_0.00163_sec
	movlw 	D'3'
	movwf 	CounterB
	movlw 	D'28'
	movwf 	CounterA
delay_0.00163_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00163_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.00163_sec_loop 
	return

delay_0.00143_sec
	movlw 	D'2'
	movwf 	CounterB
	movlw 	D'218'
	movwf 	CounterA
delay_0.00143_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00143_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.00143_sec_loop 
	return

delay_0.00138_sec
	movlw 	D'2'
	movwf 	CounterB
	movlw 	D'200'
	movwf 	CounterA
delay_0.00138_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00138_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.00138_sec_loop 
	return

delay_0.0132_sec
	movlw 	D'18'
	movwf 	CounterB
	movlw 	D'35'
	movwf 	CounterA
delay_0.0132_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.0132_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.0132_sec_loop 
	return

delay_0.00132_sec
	movlw 	D'2'
	movwf 	CounterB
	movlw 	D'181'
	movwf 	CounterA
delay_0.00132_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00132_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.00132_sec_loop 
	return

delay_0.0013_sec
	movlw 	D'2'
	movwf 	CounterB
	movlw 	D'175'
	movwf 	CounterA
delay_0.0013_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.0013_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.0013_sec_loop 
	return

delay_0.00123_sec
	movlw 	D'2'
	movwf 	CounterB
	movlw 	D'151'
	movwf 	CounterA
delay_0.00123_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00123_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.00123_sec_loop 
	return

delay_0.00103_sec
	movlw 	D'2'
	movwf 	CounterB
	movlw 	D'84'
	movwf 	CounterA
delay_0.00103_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00103_sec_loop 
	decfsz 	CounterB,1
 	goto 	delay_0.00103_sec_loop 
	return

delay_0.0006_sec
	movlw 	D'199'
	movwf 	CounterA
delay_0.0006_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.0006_sec_loop 
	return

delay_0.0002_sec
	movlw 	D'65'
	movwf 	CounterA
delay_0.0002_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.0002_sec_loop 
	return

delay_0.00006_sec
	movlw 	D'19'
	movwf 	CounterA
delay_0.00006_sec_loop
	decfsz 	CounterA,1
 	goto 	delay_0.00006_sec_loop 
	return	

delay_250us
	movlw	D'83'
	movwf	CounterA
delay_250us_loop
	decfsz	CounterA,1
	goto	delay_250us_loop
	return

; delay_8	; 8 cycle delay == 8 uS
; 	nop
; 	nop	; call   = 2 uS
; 	nop	; 4 nops = 4 uS
; 	nop	; return = 2 uS
; 	return
