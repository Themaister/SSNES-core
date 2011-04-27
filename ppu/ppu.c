#include "system/memory.h"
#include "ppu/ppu.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// This is definitely not good for cache, so we'll have to see what we can do to improve this ...
static uint16_t *xbgr2rgb_lut = NULL;

#define COLOR_LUT(color, bright) (xbgr2rgb_lut[(color) | ((unsigned)(bright) << 16)])
static inline uint16_t xbgr2rgb(unsigned color, unsigned bright)
{
   bright++;
   uint16_t r = (((color >>  0) & 0x1f) * bright) >> 4;
   uint16_t g = (((color >>  5) & 0x1f) * bright) >> 4;
   uint16_t b = (((color >> 10) & 0x1f) * bright) >> 4;
   return (r << 10) | (g << 5) | (b << 0);
}

static void init_xbgr_lut(void)
{
   for (unsigned i = 0; i < (1 << 20); i++)
   {
      unsigned bright = (i >> 16) & 0xf;
      unsigned color = i & 0xffff;

      xbgr2rgb_lut[i] = xbgr2rgb(color, bright);
   }
}

void ssnes_ppu_init(void)
{
   ALLOCATE(PPU.buffer, 1024, 1024 * 256 * sizeof(uint16_t));
   ALLOCATE(xbgr2rgb_lut, 1024, 1 << 21);

   init_xbgr_lut();
}

void ssnes_ppu_deinit(void)
{}

#define READ_CGRAMW(wordaddr) ((uint16_t)MEM.cgram[(wordaddr) << 1] | ((uint16_t)MEM.cgram[((wordaddr) << 1) + 1] << 8))
#define READ_VRAMW(wordaddr) ((uint16_t)MEM.vram[(wordaddr) << 1] | ((uint16_t)MEM.vram[((wordaddr) << 1) + 1] << 8))

// Render the basic color (color 0 which is rendered when everything else falls through).
static void ppu_render_bg(unsigned scanline)
{
   uint16_t *line = PPU.buffer + 1024 * scanline;

   uint8_t bright = (PPU.inidisp & 0xf);
   uint16_t incol = READ_CGRAMW(0);
   uint16_t bg = COLOR_LUT(incol, bright);

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

#include "mode0.h"

static void ppu_render_mode0(unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   uint8_t bright = (PPU.inidisp & 0xf);
   uint16_t *baseline = PPU.buffer + 1024 * scanline;

   // Priority bits aren't handled :(
   uint16_t *line = baseline;
   if (PPU.tm & 0x08) // BG4
   {
      unsigned vofs = PPU.bg4vofs;
      scanline = (scanline + vofs) & 0xff;
      unsigned hofs = PPU.bg4hofs;
      unsigned tilemap_addr = (PPU.bg4sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg34nba & 0xf0) << 8;
      unsigned scanline_mask = scanline & 7;

      for (unsigned i = 0; i < 256; i++)
      {
         ppu_render_bg_mode0(line++, scanline, scanline_mask, (i + hofs) & 0xff, bright, tilemap_addr, character_data, 96);
      }
   }

   line = baseline;
   if (PPU.tm & 0x04) // BG3
   {
      unsigned vofs = PPU.bg3vofs;
      scanline = (scanline + vofs) & 0xff;
      unsigned hofs = PPU.bg3hofs;
      unsigned tilemap_addr = (PPU.bg3sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0x0f) << 12;
      unsigned scanline_mask = scanline & 7;

      for (unsigned i = 0; i < 256; i++)
      {
         ppu_render_bg_mode0(line++, scanline, scanline_mask, (i + hofs) & 0xff, bright, tilemap_addr, character_data, 64);
      }
   }

   line = baseline;
   if (PPU.tm & 0x02) // BG2
   {
      unsigned vofs = PPU.bg2vofs;
      scanline = (scanline + vofs) & 0xff;
      unsigned hofs = PPU.bg2hofs;
      unsigned tilemap_addr = (PPU.bg2sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf0) << 8;
      unsigned scanline_mask = scanline & 7;

      for (unsigned i = 0; i < 256; i++)
      {
         ppu_render_bg_mode0(line++, scanline, scanline_mask, (i + hofs) & 0xff, bright, tilemap_addr, character_data, 32);
      }
   }

   line = baseline;
   if (PPU.tm & 0x01) // BG1
   {
      unsigned vofs = PPU.bg1vofs;
      scanline = (scanline + vofs) & 0xff;
      unsigned hofs = PPU.bg1hofs;
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf) << 12;
      unsigned scanline_mask = scanline & 7;

      for (unsigned i = 0; i < 256; i++)
      {
         ppu_render_bg_mode0(line++, scanline, scanline_mask, (i + hofs) & 0xff, bright, tilemap_addr, character_data, 0);
      }
   }
}

// Just a background color for now :D
void ssnes_ppu_scanline(unsigned scanline)
{
   ppu_render_bg(scanline);
   ppu_render_mode0(scanline);
}
