.segment "EXTHEADER"

; Extended header.
.byte $00, $00, "SNES", $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

; ROM name, must be 21 bytes large.
.byte "ABCDEFGHIJKLMNOPQRSTU"

; ROM Layout. $20 means LoROM, not FastROM.
.byte $20

; Cartridge type. $00 means ROM-only.
.byte $00

; ROM size byte. $0n means 2^n kilobytes.
.byte $01

; RAM size byte. Same size metric as above.
.byte $00

; Country code. $0c for PAL, $0d for NTSC.
.byte $0d

; Licensee code. $33 means we have an extended header.
.byte $33
;.byte $00

; Version. By default, $00.
.byte $00

;#  $ffdc..$ffdd => Checksum complement, which is the bitwise-xor of the checksum and $ffff.
.word $1E57
;# $ffde..$ffdf => SNES checksum, an unsigned 16-bit checksum of bytes.
.word $E1A8

;# $ffe0..$ffe3 => Unknown.
.word $0000, $0000

;# $ffe4..$ffef => Table of interrupt vectors for native mode.

.word EmptyHandler	; COP_NATIVE_HANDLER 
.word EmptyHandler	; BRK_NATIVE_HANDLER	
.word EmptyHandler	; ABORT_NATIVE_HANDLER
.word VBlank		; NMI_NATIVE_HANDLER
.word $00		; Unused
.word EmptyHandler	; IRQ_NATIVE_HANDLER 

;# $fff0..$fff3 => Unknown.
;.word $0000, $0000

;# $fff4..$ffff => Table of interrupt vectors for emulation mode.

.word EmptyHandler	; COP_EMULATED_HANDLER	
.word $00		; Unused
.word EmptyHandler	; ABORT_EMULATED_HANDLER
.word EmptyHandler	; NMI_EMULATED_HANDLER
.word EntryPoint	; RESET_EMULATED_HANDLER
.word EmptyHandler	; IRQBRK_EMULATED_HANDLER

