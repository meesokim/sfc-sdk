.A16
.I8
.smart

.include "basic.inc"

.segment "CODE"

xproc EmptyHandler
	rti
.endproc

xproc VBlank
	rti
.endproc

xproc Start
	setA8
	setI8
	jsr snes_init

	setA8
	lda     #%10000000  ; Force VBlank by turning off the screen.
	sta     $2100
	lda     #%11100000  ; Load the low byte of the green color.
	sta     $2122
	lda     #%00000000  ; Load the high byte of the green color.
	sta     $2122
	lda     #%00001111  ; End VBlank, setting brightness to 15 (100%).
	sta     $2100
	setA16

Forever:
	jmp Forever

.endproc

