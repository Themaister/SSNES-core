.include "header.inc"
.include "registers.asm"
.include "macros.asm"

.bank 0
.section "Main"

; Test address wrapping edge cases.
; Entry point
Start:
   clc
   xce
   cli

   sep #$20
   rep #$10

   ldx #$1fff
   txs

   lda #$7e
   pha
   plb

   rep #$20
   lda.w #$ff00
   tcd

   stz $ffff
   stz $0001

   ldx #$0001
   lda $ff, x

End:
   jmp End

.ends
   
