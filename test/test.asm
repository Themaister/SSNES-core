.include "header.inc"
.include "registers.asm"
.include "macros.asm"

.bank 0
.section "Main"

InitSprite:
   pha

   lda #%00000000
   sta $1002
   stz $1003
   LoadOAM $1002, $100, 2
   lda #$10
   sta $1000
   lda #$01
   sta $1001
   LoadOAM $1000, 0, 2

   LoadVRAM Sprite, $2000, $0020
   LoadVRAM Sprite, $2020, $0020

   pla
   rts

; Entry point
Start:
   clc
   xce
   cli

   stz $01

   sep #$20
   rep #$10

   jsr InitSPC

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
   LoadCGRAM Palette, $0080, 8
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

   jsr InitOAM
   jsr InitSprite
   jsr InitWindow

   lda #$01
   sta OBSEL
   lda #$04
   sta BG1SC
   lda #$01
   sta BG12NBA
   sta BG34NBA
   stz BGMODE
   lda #$15
   sta TM

   lda #$ff
   sta BG1HOFS
   stz BG1HOFS
   sta BG1VOFS
   stz BG1VOFS

   ldx.w #100
   stx VTIMEL ; Trigger VIRQ on scanline 100
   ldx.w #100
   stx HTIMEL ; Trigger HIRQ on pixel 100

   lda #%10010001
   sta NMITIMEN ; Joypad autopoll / NMI / HVIRQ
   lda #$0f
   sta INIDISP

MainLoop:
   wai ; Wait for NMI
   jmp MainLoop

VIRQ:
   pha

   lda TIMEUP ; Have to read this ...

   ; Set brightness lower.
   lda $80
   inc A
   and #$0f
   sta $80
   sta INIDISP

   pla
   rti

VBlank: ; VBlank routine
   pha
   lda RDNMI

   ; Set brightness back again.
   ;lda #$0f
   ;sta INIDISP

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

Sprite:
   .db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
   .db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
   .dw $0000, $0000, $0000, $0000
   .dw $0000, $0000, $0000, $0000



InitOAM:
   pha
   phx

; Sets OAM to an initial state. Remove all sprites off-screen.
   ldx #0
   lda #$01

-  sta $1000, x
   inx
   inx
   inx
   inx
   cpx #$0200
   bne -

   LoadOAM $1000, 0, $200

; Set signed bit in all of high part of OAM.
   lda #$55
-  sta $1000, x
   inx
   cpx #$0220
   bne -

   LoadOAM $1000 + $0200, $0100, $0020

   plx
   pla
   rts


StallSPC:
   pha
   lda #$aa
-  cmp APUIO0
   bne -
   lda #$bb
-  cmp APUIO1
   bne -
   pla
   rts

UploadSPC:
   pha
   phx

   ldx #$1000
   stx APUIO2
   stz APUIO1
   lda #$cc
   sta APUIO0

   plx
   pla
   rts
   

InitSPC:
   jsr StallSPC
   jsr UploadSPC
   rts

InitWindow:
   pha

   lda #$03
   sta W12SEL
   lda #$20
   sta WH0
   lda #$e0
   sta WH1

   lda #$01
   sta TMW

   pla
   rts

.ends
   
