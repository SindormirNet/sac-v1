

	ORG	0x2A0

escribe_eeprom_id
	bcf	STATUS,RP0
	bsf	STATUS,RP1

	movlw	0x00		; Ponemos en W la dirección que vamos a escribir
	movwf	EEADR		; y la guardamos en EEADR
	movlw	vacio_id	; Ponemos ahora el dato a escribir en W
	movwf	EEDATA		; y lo guardamos en EEDATA

	bsf	STATUS,RP0
	bsf	STATUS,RP1
	
	bsf	EECON1,WREN	; Colocamos el permiso de escritura (WREN) en EECON1 a 1 

	movlw	0x55		; Inicio de las intrucciones para EECON2
	movwf	EECON2
	movlw	0xAA
	movwf	EECON2		; Fin de las intrucciones para EECON2 

	bsf	EECON1,WR	; Añadir el dato en la EEPROM
	btfsc	EECON1,WR	; Esperamos a que termine (10 ms)
	goto	$-1
	
	bcf	STATUS,RP0
	bcf	STATUS,RP1
	return

lee_eeprom_id
; 	bcf	STATUS,RP0	; Seleccionamos el Banco 2
; 	bsf	STATUS,RP1	; Seleccionamos el Banco 2

	banksel	EEADR

	movlw	0x00		; Ponemos en W la dirección que vamos a leer
	movwf	EEADR		; y pasamos W al registro EEADR, definiendo la dirección a leer

; 	bsf	STATUS,RP1	; Seleccionamos el Banco 3
; 	bsf	STATUS,RP0      ; Seleccionamos el Banco 3
	
	banksel	EECON1

	bcf	EECON1,EEPGD
	bsf	EECON1,RD	; Hacemos la lectura colocando RD a 

; 	bsf	STATUS,RP1	; Seleccionamos el Banco 2
; 	bcf	STATUS,RP0	; Seleccionamos el Banco 2

	banksel	EEDATA

	movf	EEDATA,W	; para finalmente rescatar el dato desde EEDATA hacia W

	banksel	PORTA
; 	bcf	STATUS,RP1	; Seleccionamos el Banco 0
; 	bcf	STATUS,RP0	; Seleccionamos el Banco 0

	movwf	vacio_id
	return
