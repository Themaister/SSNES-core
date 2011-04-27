#ifndef __PPU_MODE0_H
#define __PPU_MODE0_H

static inline void ppu_render_bg_mode0(uint16_t *pixel, unsigned scanline, unsigned scanline_mask, unsigned x, uint16_t bright,
      unsigned tilemap_addr, unsigned character_data,
      unsigned base_palette)
{
   unsigned tile_no = ((scanline >> 3) << 5) + (x >> 3);
   uint16_t tile = READ_VRAMW(tilemap_addr + tile_no);

   unsigned x_mask = x & 7;
   //unsigned tile_x = isel_if(tile & 0x4000, 7 - x_mask, x_mask);
   //unsigned tile_y = isel_if(tile & 0x8000, 7 - scanline_mask, scanline_mask);
   unsigned tile_x = tile & 0x4000 ? 7 - x_mask : x_mask;
   unsigned tile_y = tile & 0x8000 ? 7 - scanline_mask : scanline_mask;

   unsigned pal = (tile & 0x1c) >> 8;
   pal += base_palette;

   unsigned index = tile & 0x3ff;

   unsigned base_addr = character_data + (index << 3) + tile_y;
   uint16_t plane = READ_VRAMW(base_addr);

   unsigned color = ((plane >> (7 - tile_x)) & 1) << 1;
   color |= (plane >> (15 - tile_x)) & 1;

   //iup_if(*pixel, color, COLOR_LUT(READ_CGRAMW(color + pal), bright));
   if (color)
      *pixel = COLOR_LUT(READ_CGRAMW(color + pal), bright);
}

#endif
