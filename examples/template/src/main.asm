#include "header.inc"

.BANK 0 SLOT 0      ; Defines the ROM bank and the slot it is inserted in memory.
.ORG 0              ; .ORG 0 is really $8000, because the slot starts at $8000
.SECTION "EmptyVectors" SEMIFREE
 
EmptyHandler:
        rti
.ENDS

.bank 0
.section "MainCode" SEMIFREE

; Reset vector entry point.
Start:
  ; Loop forever.
  sep     #$20        ; Set the A register to 8-bit.
    .accu 8
    lda     #%10000000  ; Force VBlank by turning off the screen.
    sta     $2100
    lda     #%11111111  ; Load the low byte of the green color.
    sta     $2122
    lda     #%00000011  ; Load the high byte of the green color.
    sta     $2122
    lda     #%00001111  ; End VBlank, setting brightness to 15 (100%).
    sta     $2100
Forever:
  jmp Forever

.ends