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

; FILENAME: control-panel.inc
; DESCRIPTION: UCC control routines
; VERSION: 0.9
; DATE: 14/03/09
; AUTHOR: Jorge Gomez Arenas <syvic@sindormir.net>

	ORG	0x500

Green_display_t
	movlw	b'00000111'	; Mask for reading the position
	andwf	Payload,W
	movwf	temp		; Save position into temp
		
	sublw	0x03
	btfsc	STATUS,Z
	call	green_e0

	movf	temp,W
	sublw	0x04
	btfsc	STATUS,Z
	call	green_e2
	
	return

Red_display_t
	movlw	0xF0		; Before displaying red, empty PORTA's 
	andwf	PORTA,F		; low nibble

	movlw	b'00000111'	; Mask for reading the position
	andwf	Payload,W
	movwf	temp		; Save position into temp

	sublw	0x03
	btfsc	STATUS,Z
	call	red_e0

	movf	temp,W
	sublw	0x04
	btfsc	STATUS,Z
	call	red_e2
	return

; ---------------------------------------------------------------------------
RTV_send
; ---------------------------------------------------------------------------
; DESCRIPTION:		Send RTV signal over wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		T= 1
;			|00TDD101|		D= Destination Vehicle Id
;			----------
; ---------------------------------------------------------------------------
	movwf	rtv_status	; Save Destination ID in rtv_status and in 
	movwf	temp2		; temp2. rtv_status will be cleared on [Red|Green]_Display  (syv)
	clrf	datagram	; Clear datagram

	bsf	datagram,0	; Set to 1 datagram bit 0
	bcf	datagram,1	; Set to 1 datagram bit 1
	bsf	datagram,2	; Set to 1 datagram bit 2
	bsf	datagram,5	; Set to 1 datagram bit 5
	bcf	datagram,6	; Set to 0 datagram bit 6
	bcf	datagram,7	; Set to 0 datagram bit 7

	btfsc	temp2,0		; Fills ID field with vacio_id variable
	bsf	datagram,3	; bits 3 and 4
	btfsc	temp2,1
	bsf	datagram,4

	btfsc	temp2,0		; Ugly code for fixing the ID sending. 
	bcf	datagram,4	; Don't know how to fix it

	movf	datagram,W	; Send the RTV datagram
	call	p_send_mode

	movf	datagram,W	; twice
	call	p_send_mode
	return

; ---------------------------------------------------------------------------
RTV_end_send
; ---------------------------------------------------------------------------
; DESCRIPTION:		Sends RTV signal over wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		T= 1
;			|00TDD100|		D= Destinatio Vehicle ID
;			----------
; ---------------------------------------------------------------------------
	movwf	temp2		; Save Destination ID in temp2
	clrf	datagram	; Clear datagram

	bcf	datagram,0	; Set to 0 datagram bit 0
	bcf	datagram,1	; Set to 0 datagram bit 1
	bsf	datagram,2	; Set to 1 datagram bit 2
	bsf	datagram,5	; Set to 1 datagram bit 5
	bcf	datagram,6	; Set to 0 datagram bit 6
	bcf	datagram,7	; Set to 0 datagram bit 7

	btfsc	temp2,0		; Fills ID field with vacio_id variable
	bsf	datagram,3	; bits 3 and 4
	btfsc	temp2,1
	bsf	datagram,4

	btfsc	temp2,0		; Ugly code for fixing the ID sending. 
	bcf	datagram,4	; Don't know how to fix it

	movf	datagram,W	; Send the RTV end datagram
	call	p_send_mode

	movf	datagram,W	; twice
	call	p_send_mode
	return


; ---------------------------------------------------------------------------
CP_ACK_send
; ---------------------------------------------------------------------------
; DESCRIPTION:		Sends CP ACK signal over wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		T= 1
;			|00TDD111|		D= Destinatio Vehicle ID
;			----------
; ---------------------------------------------------------------------------
	movwf	temp2		; Save Destination ID in temp2
	clrf	datagram	; Clear datagram

	bsf	datagram,0	; Set to 1 datagram bit 0
	bsf	datagram,1	; Set to 1 datagram bit 1
	bsf	datagram,2	; Set to 1 datagram bit 2
	bsf	datagram,5	; Set to 1 datagram bit 5
	bcf	datagram,6	; Set to 0 datagram bit 6
	bcf	datagram,7	; Set to 0 datagram bit 7

	btfsc	temp2,0		; Fills ID field with vacio_id variable
	bsf	datagram,3	; bits 3 and 4
	btfsc	temp2,1
	bsf	datagram,4

	btfsc	temp2,0		; Ugly code for fixing the ID sending
	bcf	datagram,4	; Don't know how to fix it

	movf	datagram,W	; Send the CP ACK datagram
	call	p_send_mode	
	return

; ---------------------------------------------------------------------------
CP_NACK_send
; ---------------------------------------------------------------------------
; DESCRIPTION:		Sends CP NACK signal over wireless
; INPUTS:		-
; OUTPUTS:		-
; CALLED FROM:	-
; CALLS TO:		-	-
; NOTES:		Packet format:
;			----------		T= 1
;			|00TDD110|		D= Destinatio Vehicle ID
;			----------
; ---------------------------------------------------------------------------
	movwf	temp2		; Save Destination ID in temp2
	clrf	datagram	; Clear datagram

	bcf	datagram,0	; Set to 0 datagram bit 0
	bsf	datagram,1	; Set to 1 datagram bit 1
	bsf	datagram,2	; Set to 1 datagram bit 2
	bsf	datagram,5	; Set to 1 datagram bit 5
	bcf	datagram,6	; Set to 0 datagram bit 6
	bcf	datagram,7	; Set to 0 datagram bit 7

	btfsc	temp2,0		; Fills ID field with vacio_id variable
	bsf	datagram,3	; bits 3 and 4
	btfsc	temp2,1
	bsf	datagram,4

	btfsc	temp2,0		; Ugly code for fixing the ID sending
	bcf	datagram,4	; Don't know how to fix it

	movf	datagram,W	; Send the CP ACK datagram
	call	p_send_mode	
	return

red_at_e1
	call	red_e1
	btfss	green_sectors,3	; Is there anybody in sector 4?
	goto	_r_e1_safe
	movlw	0x02
	call	CP_NACK_send
	return
_r_e1_safe
	movlw	0x02
	call	CP_ACK_send
	movlw	0x02
	call	RTV_end_send
	return

red_at_e3
	call	red_e3
	btfss	green_sectors,5	; Is there anybody in sector sector 6?
	goto	_r_e3_safe
	movlw	0x02
	call	CP_NACK_send
	return
_r_e3_safe
	movlw	0x02
	call	CP_ACK_send
	movlw	0x02
	call	RTV_end_send
	return

green_at_e1
	call	green_e1
	btfss	red_sectors,3	; Is there anybody in sector sector 4?
	goto	_g_e1_safe
	movlw	0x01
	call	CP_NACK_send
	return
_g_e1_safe
	movlw	0x01
	call	CP_ACK_send
	movlw	0x01
	call	RTV_end_send
	return

green_at_e3
	call	green_e3
	btfss	red_sectors,5	; Is there anybody in sector sector 6?
	goto	_g_e3_safe
	movlw	0x01
	call	CP_NACK_send
	return
_g_e3_safe
	movlw	0x01
	call	CP_ACK_send
	movlw	0x01
	call	RTV_end_send
	return

Green_cp
	movlw	b'00000111'	; Mask for reading position
	andwf	Payload,W
	movwf	temp		; Save position in temp

	sublw	0x02
	btfsc	STATUS,Z
	call	green_at_e1

	movf	temp,W
	sublw	0x05
	btfsc	STATUS,Z
	call	green_at_e3
	return

Red_cp
	movlw	0xF0		; Before displaying red, clear
	andwf	PORTA,F		; low PORTA nibble

	movlw	b'00000111'	; Mask for reading position
	andwf	Payload,W
	movwf	temp		; Save position in temp

	sublw	0x02
	btfsc	STATUS,Z
	call	red_at_e1

	movf	temp,W
	sublw	0x05
	btfsc	STATUS,Z
	call	red_at_e3
	return

Green_display
	; We look at the speed for knowing if the VACIO has received the RTV END signal
	movlw	b'00011000'	; Mask for reading speed
	andwf	Payload,W
	sublw	b'00011000'	; Is the speed==3? Then RTV END 
	btfsc	STATUS,Z	; Reset rtv_status
	bcf	rtv_status,0

	movlw	b'00000111'	; Mask for reading position
	andwf	Payload,W
	movwf	temp		; Save position in temp

	btfsc	STATUS,Z	; Is red in initializing?
	call	green_no_sector

	btfsc	Payload,5	; If packet type is 1 don't continue
	return

	sublw	0x06
	btfsc	STATUS,Z
	call	green_s4

	movf	temp,W
	sublw	0x05
	btfsc	STATUS,Z
	call	green_s6

	movf	temp,W
	sublw	0x04
	btfsc	STATUS,Z
	call	green_s5

	movf	temp,W
	sublw	0x03
	btfsc	STATUS,Z
	call	green_s2

	movf	temp,W
	sublw	0x02
	btfsc	STATUS,Z
	call	green_s3

	movf	temp,W
	sublw	0x01
	btfsc	STATUS,Z
	call	green_s1 

	return

Red_display
	; We look at the speed for knowing if the VACIO has received the RTV END signal
	movlw	b'00011000'	; Mask for reading speed
	andwf	Payload,W
	sublw	b'00011000'	; Is the speed==3? Then RTV END
	btfsc	STATUS,Z	; Reset rtv_status
	bcf	rtv_status,1

	movlw	0xF0		; Before displaying red, clear
	andwf	PORTA,F		; low PORTA nibble

	btfsc	Payload,5	; If packet type is 1 don't continue
	return

	movlw	b'00000111'	; Mask for reading position
	andwf	Payload,W
	movwf	temp		; Save position in temp
	
	btfsc	STATUS,Z	; Is red in initializing?
	call	red_no_sector

	sublw	0x06
	btfsc	STATUS,Z
	call	red_s4

	movf	temp,W
	sublw	0x05
	btfsc	STATUS,Z
	call	red_s6

	movf	temp,W
	sublw	0x04
	btfsc	STATUS,Z
	call	red_s5

	movf	temp,W
	sublw	0x03
	btfsc	STATUS,Z
	call	red_s2

	movf	temp,W
	sublw	0x02
	btfsc	STATUS,Z
	call	red_s3

	movf	temp,W
	sublw	0x01
	btfsc	STATUS,Z
	call	red_s1 
	return
