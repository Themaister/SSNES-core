#include "system/memory.h"
#include "ppu/ppu.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void ssnes_ppu_init(void)
{
   ALLOCATE(PPU.buffer, 1024, 1024 * 256 * sizeof(uint16_t));
}

void ssnes_ppu_deinit(void)
{}

#define READ_CGRAMW(wordaddr) (MEM.cgram[wordaddr << 1] | (MEM.cgram[(wordaddr << 1) + 1] << 8))

static inline uint16_t xbgr2rgb(uint16_t color, uint16_t bright)
{
   uint16_t r = (((color >>  0) & 0x1f) * bright) >> 4;
   uint16_t g = (((color >>  5) & 0x1f) * bright) >> 4;
   uint16_t b = (((color >> 10) & 0x1f) * bright) >> 4;
   return (r << 10) | (g << 5) | (b << 0);
}


// Just a background color for now :D
void ssnes_ppu_scanline(unsigned scanline)
{
   uint16_t *line = PPU.buffer + 1024 * scanline;

   uint8_t bright = (PPU.inidisp & 0xf) + 1;
   uint16_t incol = READ_CGRAMW(0);
   uint16_t bg = xbgr2rgb(incol, bright);

   if (PPU.inidisp & 0x80)
   {
      memset(line, 0, 256 * sizeof(uint16_t));
   }
   else
   {
      for (unsigned i = 0; i < 256; i++)
         line[i] = bg;
   }
}
