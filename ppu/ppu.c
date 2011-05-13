#include "system/memory.h"
#include "ppu/ppu.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static uint16_t *ppu_buffer;
const uint16_t *ssnes_ppu_buffer(void)
{
   return ppu_buffer;
}

// This is definitely not good for cache, so we'll have to see what we can do to improve this ...
static uint16_t *xbgr2rgb_lut = NULL;

#define COLOR_LUT(color, bright) (xbgr2rgb_lut[(color) | ((unsigned)(bright) << 15)])
#define COLOR_LUT_S(color, bright) (xbgr2rgb_lut[(color) | (unsigned)(bright)])
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

static uint16_t window_mask_none_buf[256] ALIGNED;

void ssnes_ppu_init(void)
{
   ALLOCATE(ppu_buffer, 1024, 1024 * 256 * sizeof(uint16_t));
   ALLOCATE(xbgr2rgb_lut, 1024, 1 << 20);

   init_xbgr_lut();

   memset(window_mask_none_buf, 0xff, sizeof(window_mask_none_buf));
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


#include "window.h"
#include "sprite.h"
#include "bg.h"
#include "mode0.h"
#include "mode1.h"


static void blit_scanline_lo(uint16_t * restrict output, const uint16_t * restrict input)
{
   unsigned bright = ((unsigned)PPU.inidisp & 0xf) << 15;
   for (unsigned i = 0; i < 256; i++)
      *output++ = COLOR_LUT_S(*input++, bright);
}


// Just a background color for now :D
void ssnes_ppu_scanline(unsigned scanline)
{
   dprintf("BG1 vhflip: %s\n", PPU.bg1sc & 0x03 ? "yes" : "no");
   dprintf("BG2 vhflip: %s\n", PPU.bg2sc & 0x03 ? "yes" : "no");
   dprintf("BG3 vhflip: %s\n", PPU.bg3sc & 0x03 ? "yes" : "no");
   dprintf("BG4 vhflip: %s\n", PPU.bg4sc & 0x03 ? "yes" : "no");

   uint16_t out_buf[1024] ALIGNED;
   ppu_render_bg(out_buf, scanline);

   switch (PPU.bgmode & 7)
   {
      case 0:
         dprintf("Mode 0\n");
         ppu_render_mode0(out_buf, scanline);
         break;

      case 1:
         dprintf("Mode 1\n");
         ppu_render_mode1(out_buf, scanline);
         break;

      case 2:
         dprintf("Mode 2: Unimplemented ...\n");
         //ppu_render_mode2(out_buf, scanline);
         break;

      case 3:
         dprintf("Mode 3: Unimplemented ...\n");
         //ppu_render_mode3(out_buf, scanline);
         break;

      case 4:
         dprintf("Mode 4: Unimplemented ...\n");
         //ppu_render_mode4(out_buf, scanline);
         break;

      case 5:
         dprintf("Mode 5: Unimplemented ...\n");
         //ppu_render_mode5(out_buf, scanline);
         break;

      case 6: // ...
         dprintf("Mode 6: Unimplemented ...\n");
         //ppu_render_mode6(out_buf, scanline);
         break;

      case 7: // O.O
         dprintf("Mode 7: Unimplemented ...\n");
         //ppu_render_mode7(out_buf, scanline);
         break;
   }

   blit_scanline_lo(ppu_buffer + 1024 * scanline, out_buf);
}
