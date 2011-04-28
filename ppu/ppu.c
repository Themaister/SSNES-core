#include "system/memory.h"
#include "ppu/ppu.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// This is definitely not good for cache, so we'll have to see what we can do to improve this ...
static uint16_t *xbgr2rgb_lut = NULL;

#define COLOR_LUT(color, bright) (xbgr2rgb_lut[((color) & 0x7fff) | ((unsigned)(bright) << 15)])
#define COLOR_LUT_S(color, bright) (xbgr2rgb_lut[((color) & 0x7fff) | (unsigned)(bright)])
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
   for (unsigned i = 0; i < (1 << 19); i++)
   {
      unsigned bright = (i >> 15) & 0xf;
      unsigned color = i & 0x7fff;

      xbgr2rgb_lut[i] = xbgr2rgb(color, bright);
   }
}

void ssnes_ppu_init(void)
{
   ALLOCATE(PPU.buffer, 1024, 1024 * 256 * sizeof(uint16_t));
   ALLOCATE(xbgr2rgb_lut, 1024, 1 << 20);

   init_xbgr_lut();
}

void ssnes_ppu_deinit(void)
{}


// Render the basic color (color 0 which is rendered when everything else falls through).
static void ppu_render_bg(uint16_t *out_buf, unsigned scanline)
{
   uint16_t incol = READ_CGRAMW(0);
   if (PPU.inidisp & 0x80)
   {
      memset(out_buf, 0, 256 * sizeof(uint16_t));
   }
   else
   {
      for (unsigned i = 0; i < 256; i++)
         out_buf[i] = incol;
   }
}

#include "mode0.h"
#include "sprite.h"

static void ppu_render_mode0(uint16_t *out_buf, unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   //uint8_t prio_buf[256];

   // Priority bits aren't handled :(
   uint16_t *line = out_buf;
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
         ppu_render_bg_mode0(line++, scanline, scanline_mask, 
               (i + hofs) & 0xff, tilemap_addr, character_data, 96);
      }
   }

   line = out_buf;
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
         ppu_render_bg_mode0(line++, scanline, scanline_mask, 
               (i + hofs) & 0xff, tilemap_addr, character_data, 64);
      }
   }

   line = out_buf;
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
         ppu_render_bg_mode0(line++, scanline, scanline_mask, 
               (i + hofs) & 0xff, tilemap_addr, character_data, 32);
      }
   }

   line = out_buf;
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
         ppu_render_bg_mode0(line++, scanline, scanline_mask, 
               (i + hofs) & 0xff, tilemap_addr, character_data, 0);
      }
   }
   
   line = out_buf;
   if (PPU.tm & 0x10)
   {
      const uint8_t *oam_hi = &MEM.oam.b[512];
      ppu_render_sprites(line, oam_hi, scanline);
   }
}

static void blit_scanline_lo(uint16_t * restrict output, const uint16_t * restrict input)
{
   unsigned bright = ((unsigned)PPU.inidisp & 0xf) << 15;
   for (unsigned i = 0; i < 256; i++)
      *output++ = COLOR_LUT_S(*input++, bright);
}

// Just a background color for now :D
void ssnes_ppu_scanline(unsigned scanline)
{
   uint16_t out_buf[256];
   ppu_render_bg(out_buf, scanline);
   ppu_render_mode0(out_buf, scanline);
   blit_scanline_lo(PPU.buffer + 1024 * scanline, out_buf);
}
