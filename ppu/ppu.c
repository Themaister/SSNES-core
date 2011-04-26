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

#define READ_CGRAMW(wordaddr) ((uint16_t)MEM.cgram[(wordaddr) << 1] | ((uint16_t)MEM.cgram[((wordaddr) << 1) + 1] << 8))
#define READ_VRAMW(wordaddr) ((uint16_t)MEM.vram[(wordaddr) << 1] | ((uint16_t)MEM.vram[((wordaddr) << 1) + 1] << 8))

static inline uint16_t xbgr2rgb(uint16_t color, uint16_t bright)
{
   uint16_t r = (((color >>  0) & 0x1f) * bright) >> 4;
   uint16_t g = (((color >>  5) & 0x1f) * bright) >> 4;
   uint16_t b = (((color >> 10) & 0x1f) * bright) >> 4;
   return (r << 10) | (g << 5) | (b << 0);
}

// Render the basic color (color 0 which is rendered when everything else falls through).
static void ppu_render_bg(unsigned scanline)
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

static inline void ppu_render_bg1_mode0(uint16_t *pixel, unsigned scanline, unsigned scanline_mask, unsigned x, uint16_t bright,
      unsigned tilemap_addr, unsigned character_data)
{
   unsigned tile_no = ((scanline >> 3) << 5) + (x >> 3);
   uint16_t tile = READ_VRAMW(tilemap_addr + tile_no);

   unsigned x_mask = x & 7;
   //unsigned tile_x = isel_if(tile & 0x4000, 7 - x_mask, x_mask);
   //unsigned tile_y = isel_if(tile & 0x8000, 7 - scanline_mask, scanline_mask);
   unsigned tile_x = tile & 0x4000 ? 7 - x_mask : x_mask;
   unsigned tile_y = tile & 0x8000 ? 7 - scanline_mask : scanline_mask;

   unsigned pal = (tile & 0x1c) >> 8;

   unsigned index = tile & 0x3ff;

   unsigned base_addr = character_data + (index << 3) + tile_y;
   uint16_t plane = READ_VRAMW(base_addr);

   unsigned color = ((plane >> (7 - tile_x)) & 1) << 1;
   color |= (plane >> (15 - tile_x)) & 1;

   if (color)
      *pixel = xbgr2rgb(READ_CGRAMW(color + pal), bright);
}

static void ppu_render_mode0(unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   uint8_t bright = (PPU.inidisp & 0xf) + 1;
   uint16_t *line = PPU.buffer + 1024 * scanline;

   if (PPU.tm & 0x01)
   {
      unsigned vofs = PPU.bg1vofs;
      scanline = (scanline + vofs) & 0xff;
      unsigned hofs = PPU.bg1hofs;
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf) << 12;
      unsigned scanline_mask = scanline & 7;

      for (unsigned i = 0; i < 256; i++)
      {
         ppu_render_bg1_mode0(line++, scanline, scanline_mask, (i + hofs) & 0xff, bright, tilemap_addr, character_data);
      }
   }
}

// Just a background color for now :D
void ssnes_ppu_scanline(unsigned scanline)
{
   ppu_render_bg(scanline);
   ppu_render_mode0(scanline);
}
