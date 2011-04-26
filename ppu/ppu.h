#ifndef __PPU_H
#define __PPU_H

#include <stdint.h>
#include "system/util.h"
#include <stdbool.h>

struct ppu_state
{
   bool vsync;

   uint8_t inidisp; // $2100

   uint8_t bgmode; // $2105

   uint8_t tm; // $212c

   uint8_t bg1sc, bg2sc, bg3sc, bg4sc; // $2107-$210a
   uint8_t bg12nba, bg34nba; // $210b-$210c

   uint16_t bg1hofs, bg1vofs; // $210d-$2114
   uint16_t bg2hofs, bg2vofs;
   uint16_t bg3hofs, bg3vofs;
   uint16_t bg4hofs, bg4vofs;

   uint8_t hvbjoy; // $4212

   uint16_t *buffer;
};

void ssnes_ppu_init(void);
void ssnes_ppu_deinit(void);

void ssnes_ppu_scanline(unsigned scanline);

#endif
