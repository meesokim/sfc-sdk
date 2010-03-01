; Conventions:
;	- cdecl
;		- caller cleans up stack.
;		- callee has to preserve registers
;	- A is return register @ 16-bit.
;	- If return is void, then A SHOULD BE PRESERVED!
;	- All registers are as follows:
;		- A = 16-bit; X and Y are 8-bit
;		- If a proc changes this, it must revert back before it is finished!

changecom(`;')

define(xproc,$1:
	.accu 16
	.index 8)

define(xendproc,)

define(setA8,sep #%00100000
	.accu 8)
define(setA16,rep #%00100000
	.accu 16)
define(setI8,sep #%00010000
	.index 8)
define(setI16,rep #%00010000
	.index 16)

define(`STD_INITSYSTEM',sei        	; Disabled interrupts
	clc        		   	; clear carry to switch to native mode
	xce        		   	; Xchange carry & emulation bit. native mode
	;rep #``$'`18''   	 	; Binary mode ``('decimal mode off`)'', X/Y 16 bit
	cld
	setI16

	ldx #``$'`1''FFF		; set stack to ``$'`1''FFF
	txs
	jsr STD_ActualInit)


