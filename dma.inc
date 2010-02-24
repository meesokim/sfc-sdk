; DMA enable register.
.define MDMAEN       $420b

; HDMA enable register.
.define HDMAEN       $420c

;
; NOTE: The rest of these registers must be OR'ed with $x0,
; where x is the Channel ID.  First channel ID is 0.  Last is 7.
; There are obviously 8 channels.
;

; DMA control address.  Note: This has to be altered.
.define DMAPX        $4300

; DMA Destination
.define BBADX        $4301

; DMA Source address
.define A1TXL        $4302
.define A1TXH        $4302

; DMA Source bank
.define A1BX         $4304

; DMA transfer address
.define DASXL        $4305
.define DASXH        $4305

; HDMA Line Count
.define NTRLX        $430A
