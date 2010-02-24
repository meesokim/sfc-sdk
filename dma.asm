.include "basic.inc"
.include "dma.inc"

.segment "CODE"

; Args:
;	-: Channel ID -- 0 based, byte
;	-: Control byte, byte.
; Return: void
xproc dma_setcontrol
	pha					; 2
		savestack
			pea DMAPX		; Address to write.  Note pea is "broken" and acts as if there is a '#' prepended to the address.
			setA8
			lda 4, s		; Arg 2
			pha
			lda 3, s		; Arg 1
			pha
			setA16
			jsr mem_orstorebyte
		restorestack
	pla					; -2
	rts
.endproc

; Args:
;	-: Channel ID, zero based, byte
;	-: Destination, byte
;	-: Source bank, byte
;	-: Source address, word
;	-: Size of transfer, word
; Return: void
xproc dma_prepare
	pha					; 2
	phx					; 1
		savestack
			pea BBADX		; 2 Address to write.  Note pea is "broken" and acts as if there is a '#' prepended to the address.
			setA8
			lda 5, s		; Arg 2
			pha			; 1
			lda 4, s		; Arg 1
			pha			; 1
			setA16
			jsr mem_orstorebyte
						; Note: We are NOT cleaning up the stack here.  BEWARE!
						
;
;
;		REVERSE ORDER OF ARGS WHEN CALLING PROCS!!!11111
;		FROM HERE ON AND DOWN, START REVERSING!
;
;
;
			setA8
			lda 8, s
			sta 2, s
			setA16
			lda #A1BX
			sta 3, s
			jsr mem_orstorebyte
						; Note: We are NOT cleaning up the stack here.  BEWARE!
			; Write low byte, then high byte
			lda 9, s
			setA8
			sta 2, s
			xba
			tax
			setA16
			lda #A1TXL
			sta 3, s
			jsr mem_orstorebyte
			
			setA8
			txa
			sta 2, s
			setA16
			lda #A1TXH
			sta 3, s
			jsr mem_orstorebyte
						
			; Write low byte, then high byte
			
			lda 11, s
			setA8
			sta 2, s
			xba
			tax
			setA16
			lda #DASXL
			sta 3, s
			jsr mem_orstorebyte
			
			setA8
			txa
			sta 2, s
			setA16
			lda #DASXH
			sta 3, s
			jsr mem_orstorebyte
		restorestack
	plx			; -1
	pla			; -2
	rts
.endproc

; Args:
;	-: Enable byte, each bit corresponding to the DMA channel to write/flush.
; Return: void
xproc dma_writeflush
	pha
		setA8
		lda 3, s	; The enable byte.
		sta MDMAEN	; Enables the DMA channels based on passed in var.
		setA16
	pla
	rts
.endproc

; Args:
;	-: Channel ID, zero based, byte.
;	-: Byte to store, byte.
;	-: Address to OR with Channel ID, word.
; Return: void
xproc mem_orstorebyte
	pha			; 2
	phy			; 1
		and $0000
		setA8
		lda 4, s	; Arg 1; Channel ID.  Byte.  We want to left shift this once, and OR it with the literal value of DMAP_.
		setA16
		asl		; Perform 1st left-shift.
		asl		; Perform 2nd left-shift.
		asl		; Perform 3rd left-shift.
		asl		; Perform 4th left-shift.
		ora 5, s	; OR with Arg 3.
		pha		; 2; This stores the address we want to which we want to write to 1,s
		setA8
		lda 7, s	; Arg 2; Payload byte.  We need to write this byte to (Arg_3 | (channel ID * 16))
		ldy #$00	; 0 out y.
		sta (1, s),y	; Writing byte to proper address...
		pla		; -2
		setA16
	ply			; -1
	pla			; -2
	rts
.endproc
