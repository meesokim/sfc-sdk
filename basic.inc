; Conventions:
;	- cdecl
;		- caller cleans up stack.
;		- callee has to preserve registers
;	- A is return register @ 16-bit.
;	- If return is void, then A SHOULD BE PRESERVED!
;	- All registers are as follows:
;		- A = 16-bit; X and Y are 8-bit
;		- If a proc changes this, it must revert back before it is finished!

; Assembler must be assembled in SMART mode for the 65816!
.A16
.I8
.Smart

.segment "DATA"
	savedsp:	.res 256
	savedspoffset:	.byte 0
	retval:		.word 0
.segment "CODE"

.define setA8        sep #%00100000 
.define setA16       rep #%00100000
.define setI8        sep #%00010000
.define setI16       rep #%00010000

; DO NOT ABUSE THIS!  This is merely to give the assembler some hints about
; what the convention is.  If you need to change the registers, DO SO!  See the
; set* .define's above.  REMEMBER TO ALTER EVERYTHING BACK TO OUR CONVENTION!
; Don't forget or you'll potentially screw everything up!
.macro xproc name
	.export name
	.proc name
	.A16
	.I8
.endmacro

.macro iproc name
	.proc name
	.A16
	.I8
.endmacro

;.macro xmacro name
;.macro name
;.A16
;.I8
;.endmacro

.macro push arg
        .if (.match (.left (1,{arg}),a))
                phx
        .elseif (.match (.left (1,{arg}),x))
                phy
	.elseif (.match (.left (1,{arg}),y))
                pha
        .elseif (.match (.left (1,{arg}),'('))
                pei arg
        .else
                pea arg
        .endif
.endmacro

; Note: A MUST BE IN 16-BIT MODE.
; DON'T TRY TO CIRCUMVENT THIS UNLESS YOU KNOW WHAT YOU'RE DOING!
; Further note: Conveniently, you may call this in the "PROPER order."
; This means that you can do call func 1 2 3, and the callee will receive 1 first, instead of 3, on the stack.
; This is because arguments are pushed in reversed for the caller.
.macro autocall callee, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, argA, argB, argC, argD, argE, argF
	pha
	tsc
	sta savedsp	; Save stack pointer.
	pla		; Restore A to whatever it was.
	
	.ifnblank argF
		push argF
	.ifnblank argE
		push argE
	.ifnblank argD
		push argD
	.ifnblank argC
		push argC
	.ifnblank argB
		push argB
	.ifnblank argA        	
		push argA	
	.ifnblank arg9     	
		push arg9
	.ifnblank arg8
		push arg8
	.ifnblank arg7
		push arg7
	.ifnblank arg6
		push arg6
	.ifnblank arg5
		push arg5
	.ifnblank arg4
		push arg4
	.ifnblank arg3
		push arg3
	.ifnblank arg2
		push arg2
	.ifnblank arg1
		push arg1
	.ifnblank arg0
		push arg0		
	jsr callee
	
	sta retval
	lda savedsp
	tcs
	lda retval	; A should now contain the RETURN VALUE of the function.  If no return, then value is unaltered.
.endmacro

.macro savestack
	pha
	phx
	tsc
	ldx savedspoffset
	sta savedsp, x		; Save stack pointer.
	inx
	inx
	stx savedspoffset
	plx
	pla			; Restore A to whatever it was.
.endmacro

.macro restorestack
	phx
	sta retval
	ldx savedspoffset
	lda savedsp, x
	tcs
	dex
	dex
	stx savedspoffset
	lda retval	; A should now contain the RETURN VALUE of the function.  If no return, then value is unaltered.
	plx
.endmacro
