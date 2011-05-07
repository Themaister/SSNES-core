#ifndef __PPU_H
#define __PPU_H

#include <stdint.h>
#include "system/util.h"
#include <stdbool.h>

#define READ_CGRAMW(wordaddr) READ_16LE(MEM.cgram.w + (wordaddr))
#define READ_VRAMW(wordaddr) READ_16LE(MEM.vram.w + (wordaddr))
#define READ_OAML(wordaddr) READ_32LE(MEM.oam.l + (wordaddr))
#define WRITE_CGRAMW(wordaddr, data) WRITE_16LE(MEM.cgram.w + (wordaddr), data)
#define WRITE_VRAMW(wordaddr, data) WRITE_16LE(MEM.vram.w + (wordaddr), data)
#define WRITE_OAML(wordaddr, data) WRITE_32LE(MEM.oam.l + (wordaddr), data)
#define WRITE_OAMW(wordaddr, data) WRITE_16LE(MEM.oam.w + (wordaddr), data)

struct ppu_state
{
   bool vsync;

   uint8_t inidisp; // $2100
   uint8_t obsel; // $2101

   uint8_t bgmode; // $2105

   uint8_t tm; // $212c
   uint8_t ts; // $212d
   uint8_t tmw; // $212e
   uint8_t tsw; // $212f

   uint8_t bg1sc, bg2sc, bg3sc, bg4sc; // $2107-$210a
   uint8_t bg12nba, bg34nba; // $210b-$210c

   uint16_t bg1hofs, bg1vofs; // $210d-$2114
   uint16_t bg2hofs, bg2vofs;
   uint16_t bg3hofs, bg3vofs;
   uint16_t bg4hofs, bg4vofs;

   uint8_t hvbjoy; // $4212

   uint8_t stat78; // $213f

   uint8_t w12sel; // $2123
   uint8_t w34sel; // $2124
   uint8_t wobjsel; // $2125
   uint8_t wh0; // $2126
   uint8_t wh1; // $2127
   uint8_t wh2; // $2128
   uint8_t wh3; // $2129
   uint8_t wbglog; // $212a
   uint8_t wobjlog; // $212b

   uint16_t *buffer;
};

void ssnes_ppu_init(void);
void ssnes_ppu_deinit(void);

void ssnes_ppu_scanline(unsigned scanline);

#endif
