.include "header.inc"
.include "registers.asm"
.include "macros.asm"

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

   LoadCGRAM Palette, 0, 8
   LoadVRAM Tilemap, $0400, 2
   LoadVRAM Tiles, $1008, 16

   lda #$04
   sta BG1SC
   lda #$01
   sta BG12NBA
   stz BGMODE
   sta TM

   lda #$ff
   sta BG1HOFS
   stz BG1HOFS
   sta BG1VOFS
   stz BG1VOFS

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


Palette:
   .dw $0000, $efff, $001f, $7c00

Tilemap:
   .dw $0001

Tiles:
   .db $ff, $ff, $00, $00, $ff, $ff, $00, $00
   .db $ff, $ff, $00, $00, $ff, $ff, $00, $00

.ends
   
