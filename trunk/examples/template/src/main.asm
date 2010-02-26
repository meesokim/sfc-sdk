#include "header.inc"

; Reset vector entry point.
Start:
  ; Initialize the SNES.
  init
  
  ; Loop forever.
Forever:
  jmp Forever

.ends