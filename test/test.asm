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

   stz $01

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
   LoadCGRAM Palette, $0040, 8
   LoadVRAM Tilemap, $0400, $0020
   LoadVRAM Tilemap, $0500, $0020
   LoadVRAM Tilemap, $0600, $0020
   LoadVRAM Tilemap, $0620, $0020
   LoadVRAM Tilemap, $0640, $0020
   LoadVRAM Tilemap, $0680, $0020
   LoadVRAM Tilemap, $06a0, $0020
   LoadVRAM Tilemap, $06c0, $0020
   LoadVRAM Tilemap, $06e0, $0020
   LoadVRAM Tilemap, $0720, $0020
   LoadVRAM Tilemap, $0750, $0020
   LoadVRAM Tilemap, $0040, $0020
   LoadVRAM Tilemap, $0140, $0020
   LoadVRAM Tilemap, $0240, $0020
   LoadVRAM Tiles, $1008, 16

   lda #$04
   sta BG1SC
   lda #$01
   sta BG12NBA
   sta BG34NBA
   stz BGMODE
   lda #$05
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
   lda JOY1H
   and #$01
   beq +
   inc $01
   lda $01
   sta BG1HOFS
   stz BG1HOFS
   sta BG1VOFS
   stz BG1VOFS
+
   pla
   rti


Palette:
   .dw $001f, $7fff, $001f, $7c00

Tilemap:
   .dw $0001, $0001, $0001, $0001, $0001, $0001, $0001, $0001
   .dw $0001, $0001, $0001, $0001, $0001, $0001, $0001, $0001

Tiles:
   .db $ff, $ff, $00, $00, $ff, $ff, $00, $00
   .db $ff, $ff, $00, $00, $ff, $ff, $00, $00

.ends
   
