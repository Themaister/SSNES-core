.include "header.inc"

.include "registers.asm"

.bank 0
.section "Main"

; Entry point
Start:

   clc
   xce
   cli

   sep #$20
   rep #$10

   ldx #$1fff
   txs

   lda #$80
   sta INIDISP


   stz CGADD
   lda #%00011111
   sta CGDATA
   stz CGDATA

   lda #%10000001
   sta NMITIMEN ; Joypad autopoll / NMI
   lda #$0f
   sta INIDISP

MainLoop:
   wai ; Wait for NMI
   ;wai
   ;wai

   lda $00
   inc A
   and #$1f
   sta $00
   stz CGADD
   sta CGDATA
   stz CGDATA

   jmp MainLoop


VBlank: ; VBlank routine
   pha
   lda RDNMI
   pla
   rti

.ends
   
