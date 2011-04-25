.include "header.inc"
.include "registers.asm"

.macro Stall
   ldx #\1
-- dex
   bne --
.endm
   

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
   jmp MainLoop


VBlank: ; VBlank routine
   pha
   lda RDNMI

-  lda HVBJOY
   and #$01
   beq -
-  lda HVBJOY
   and #$01
   bne -

   lda JOY1H
   and #$80
   beq +

   lda $00
   inc A
   and #$1f
   stz CGADD
   sta CGDATA
   stz CGDATA
   sta $00
+
   pla
   rti

.ends
   
