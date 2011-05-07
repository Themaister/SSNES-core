.include "header.inc"
.include "registers.asm"
.include "macros.asm"

.bank 0
.section "Main"

Start:
   clc
   xce
   cli

   rep #$20
   lda.w #$0000
   tcd
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

   jsr InitHDMA

   lda #%10000001
   sta NMITIMEN ; Joypad autopoll / NMI / HVIRQ
   lda #$0f
   sta INIDISP

MainLoop:
   wai ; Wait for NMI
   jmp MainLoop

VBlank: ; VBlank routine
   pha
   lda RDNMI

-  lda HVBJOY
   bpl -
-  lda HVBJOY
   bmi -

   lda JOY1H
   and #$80
   beq +

   lda $80
   inc A
   and #$7f
   sta $80

+
   pla
   rti

InitHDMA:

   ldx #(IHDMA_End - IHDMA - 1)
-  lda IHDMA.w, x
   sta $00, x
   dex
   bpl -

   ldx #(HDMATable_End - HDMATable - 1)
-  lda HDMATable.w, x
   sta $80, x
   dex 
   bpl -

   lda #$00 ; Indirect HDMA, transfer mode 0.
   sta DMAP0
   ldx #$0080
   stx A1T0L
   stz A1B0

   stz DASB0

   ; Flashy pattern ftw.
   stz BBAD0

   lda #$01
   sta HDMAEN
   rts

IHDMA:
   IHDMA_End:


HDMATable:
   .db $0a
   .db $02
   .db $9a
   .db $03
   .db $04, $05
   .db $06, $07
   .db $08, $09
   .db $0a, $0b
   .db $0c, $0d
   .db $0e, $0f
   .db $0e, $0d
   .db $0c, $0b
   .db $0a, $09
   .db $08, $07
   .db $06, $05
   .db $04, $03
   .db $02
   .db $00
HDMATable_End:

.ends
   
