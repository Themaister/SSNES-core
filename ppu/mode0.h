#ifndef __PPU_MODE0_H
#define __PPU_MODE0_H

static inline void ppu_render_bg_mode0(uint16_t *pixels, unsigned scanline, unsigned scanline_mask, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data,
      unsigned base_palette)
{
   tilemap_addr += ((scanline >> 3) << 5);
   unsigned y = scanline & 7;

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i);
      unsigned pal = (tile & 0x1c) >> 8;
      pal += base_palette;
      
      unsigned index = tile & 0x3ff;

      unsigned tile_y = isel_if(tile & 0x8000, 7 - y, y);
      unsigned base_addr = character_data + (index << 3) + tile_y;

      uint16_t plane = READ_VRAMW(base_addr);

      if (tile & 0x4000)
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane >> i) & 1;
            color |= ((plane >> (i + 8)) & 1) << 1;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }
      }
      else
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane >> (7 - i)) & 1;
            color |= ((plane >> (15 - i)) & 1) << 1;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }
      }
   }
}

#endif
