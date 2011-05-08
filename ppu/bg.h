#ifndef __SSNES_PPU_BG_H
#define __SSNES_PPU_BG_H

#include <stdbool.h>
#include <stdint.h>

// A fuckton of code incoming! :D

static inline void ppu_render_tile_2bpp_8x8(
      uint16_t *pixels, 
      uint16_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint8_t *mask_buf, const uint8_t *z_prio, uint8_t *z_buffer)
{
   unsigned z_value = z_prio[prio];
   if (hflip)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> i) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 8)) & 1) << 1;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color) // If prio is higher and color is not 0 (transparent), update pixel and z-buffer.
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
   else
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (7 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (15 - i)) & 1) << 1;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color)
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
}

static inline void ppu_render_tile_2bpp_16x16(
      uint16_t *pixels, 
      uint16_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint8_t *mask_buf, const uint8_t *z_prio, uint8_t *z_buffer)
{
   unsigned z_value = z_prio[prio];
   if (hflip)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> i) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 8)) & 1) << 1;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color) // If prio is higher and color is not 0 (transparent), update pixel and z-buffer.
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (i + 16)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 24)) & 1) << 1;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color) // If prio is higher and color is not 0 (transparent), update pixel and z-buffer.
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
   else
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (7 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (15 - i)) & 1) << 1;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color)
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (23 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (31 - i)) & 1) << 1;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color)
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
}

static inline void ppu_render_tile_4bpp_8x8(
      uint16_t *pixels, 
      uint32_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint8_t *mask_buf, const uint8_t *z_prio, uint8_t *z_buffer)
{
   unsigned z_value = z_prio[prio];
   if (hflip)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> i) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 8)) & 1) << 1;
         color |= ((plane >> (i + 16)) & 1) << 2;
         color |= ((plane >> (i + 24)) & 1) << 3;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color) // If prio is higher and color is not 0 (transparent), update pixel and z-buffer.
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
   else
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (7 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (15 - i)) & 1) << 1;
         color |= ((plane >> (23 - i)) & 1) << 2;
         color |= ((plane >> (31 - i)) & 1) << 3;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color)
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
}

static inline void ppu_render_tile_4bpp_16x16(
      uint16_t *pixels, 
      uint64_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint8_t *mask_buf, const uint8_t *z_prio, uint8_t *z_buffer)
{
   unsigned z_value = z_prio[prio];
   if (hflip)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> i) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 8)) & 1) << 1;
         color |= ((plane >> (i + 16)) & 1) << 2;
         color |= ((plane >> (i + 24)) & 1) << 3;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color) // If prio is higher and color is not 0 (transparent), update pixel and z-buffer.
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (i + 32)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 40)) & 1) << 1;
         color |= ((plane >> (i + 48)) & 1) << 2;
         color |= ((plane >> (i + 56)) & 1) << 3;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color) // If prio is higher and color is not 0 (transparent), update pixel and z-buffer.
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
   else
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (7 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (15 - i)) & 1) << 1;
         color |= ((plane >> (23 - i)) & 1) << 2;
         color |= ((plane >> (31 - i)) & 1) << 3;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color)
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (39 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (47 - i)) & 1) << 1;
         color |= ((plane >> (55 - i)) & 1) << 2;
         color |= ((plane >> (63 - i)) & 1) << 3;
         color &= mask_buf[x & 0xff]; // Window mask.
         if (z_value > z_buffer[x] && color)
         {
            z_buffer[x] = z_value;
            pixels[x] = READ_CGRAMW(palette + color);
         }
         x = (x + 1) & x_mask;
      }
   }
}


static inline void ppu_render_bg_2bpp_8x8 (
      uint16_t *pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint8_t *mask_buf, const uint8_t* z_prio, uint8_t *z_buffer)
{
   scanline += vofs + 1;
   scanline &= isel_if(vmirror, 0x1ff, 0xff);

   tilemap_addr += ((scanline & 0xff) >> 3) << 5;
   tilemap_addr += (scanline & 0x100) << 3;

   unsigned y = scanline & 7;

   unsigned x_mask = isel_if(hmirror, 0x1ff, 0xff);
   for (unsigned i = 0; i <= 256; i += 8)
   {
      unsigned x = (i + hofs) & x_mask;
      unsigned map_index = ((x & 0xff) >> 3) + ((x & 0x100) << 2);
      unsigned tile_attr = READ_VRAMW(tilemap_addr + map_index);
      unsigned pal = (tile_attr & 0x1c00) >> 8;
      pal += base_palette;

      unsigned index = tile_attr & 0x3ff;
      bool prio = tile_attr & 0x2000;
      bool hflip = tile_attr & 0x4000;

      unsigned tile_y = isel_if(tile_attr & 0x8000, 7 - y, y);
      unsigned base_addr = character_data + (index << 3) + tile_y;
      uint16_t plane = READ_VRAMW(base_addr);

      unsigned start_x = (map_index & 0x1f) + (map_index >> 5);
      start_x -= hofs;
      start_x &= x_mask;

      ppu_render_tile_2bpp_8x8(pixels, plane, start_x, x_mask, pal, hflip, prio,
            mask_buf, z_prio, z_buffer);
   }
}

static inline void ppu_render_bg_4bpp_8x8 (
      uint16_t *pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint8_t *mask_buf, const uint8_t* z_prio, uint8_t *z_buffer)
{
   scanline += vofs + 1;
   scanline &= isel_if(vmirror, 0x1ff, 0xff);

   tilemap_addr += ((scanline & 0xff) >> 3) << 5;
   tilemap_addr += (scanline & 0x100) << 3;

   unsigned y = scanline & 7;

   unsigned x_mask = isel_if(hmirror, 0x1ff, 0xff);
   for (unsigned i = 0; i <= 256; i += 8)
   {
      unsigned x = (i + hofs) & x_mask;
      unsigned map_index = ((x & 0xff) >> 3) + ((x & 0x100) << 2);
      unsigned tile_attr = READ_VRAMW(tilemap_addr + map_index);
      unsigned pal = (tile_attr & 0x1c00) >> 8;
      pal += base_palette;

      unsigned index = tile_attr & 0x3ff;
      bool prio = tile_attr & 0x2000;
      bool hflip = tile_attr & 0x4000;

      unsigned tile_y = isel_if(tile_attr & 0x8000, 7 - y, y);
      unsigned base_addr = character_data + (index << 4) + tile_y;
      uint32_t plane = (uint32_t)READ_VRAMW(base_addr) | ((uint32_t)READ_VRAMW(base_addr + 8) << 16);

      unsigned start_x = ((map_index & 0x1f) + (map_index >> 5)) << 3;
      start_x -= hofs;
      start_x &= x_mask;

      ppu_render_tile_2bpp_8x8(pixels, plane, start_x, x_mask, pal, hflip, prio,
            mask_buf, z_prio, z_buffer);
   }
}


static inline void ppu_render_bg_2bpp_16x16 (
      uint16_t *pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint8_t *mask_buf, const uint8_t* z_prio, uint8_t *z_buffer)
{
   scanline += vofs + 1;
   scanline &= isel_if(vmirror, 0x1ff, 0xff);
   
   tilemap_addr += ((scanline & 0x1ff) >> 4) << 5;
   tilemap_addr += (scanline & 0x200) << 2;

   unsigned y = scanline & 15;

   unsigned x_mask = isel_if(hmirror, 0x3ff, 0x1ff);
   for (unsigned i = 0; i <= 256; i += 16)
   {
      unsigned x = (i + hofs) & x_mask;
      unsigned map_index = ((x & 0x1ff) >> 4) + ((x & 0x200) << 1);
      unsigned tile_attr = READ_VRAMW(tilemap_addr + map_index);
      unsigned pal = (tile_attr & 0x1c00) >> 8;
      pal += base_palette;

      unsigned index = tile_attr & 0x3ff;
      bool prio = tile_attr & 0x2000;
      bool hflip = tile_attr & 0x4000;

      unsigned tile_y = isel_if(tile_attr & 0x8000, 15 - y, y);
      index += (tile_y & 8) << 1;
      unsigned base_addr = character_data + (index << 3) + (tile_y & 7);
      uint32_t plane = (uint32_t)READ_VRAMW(base_addr) | ((uint32_t)READ_VRAMW(base_addr + 8) << 16);

      unsigned start_x = ((map_index & 0x1f) + (map_index >> 5)) << 4;
      start_x -= hofs;
      start_x &= x_mask;

      ppu_render_tile_2bpp_16x16(pixels, plane, start_x, x_mask, pal, hflip, prio,
            mask_buf, z_prio, z_buffer);
   }
}

static inline void ppu_render_bg_4bpp_16x16 (
      uint16_t *pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint8_t *mask_buf, const uint8_t* z_prio, uint8_t *z_buffer)
{
   scanline += vofs + 1;
   scanline &= isel_if(vmirror, 0x1ff, 0xff);
   
   tilemap_addr += ((scanline & 0x1ff) >> 4) << 5;
   tilemap_addr += (scanline & 0x200) << 2;

   unsigned y = scanline & 15;

   unsigned x_mask = isel_if(hmirror, 0x3ff, 0x1ff);
   for (unsigned i = 0; i <= 256; i += 16)
   {
      unsigned x = (i + hofs) & x_mask;
      unsigned map_index = ((x & 0x1ff) >> 4) + ((x & 0x200) << 1);
      unsigned tile_attr = READ_VRAMW(tilemap_addr + map_index);
      unsigned pal = (tile_attr & 0x1c00) >> 8;
      pal += base_palette;

      unsigned index = tile_attr & 0x3ff;
      bool prio = tile_attr & 0x2000;
      bool hflip = tile_attr & 0x4000;

      unsigned tile_y = isel_if(tile_attr & 0x8000, 15 - y, y);
      index += (tile_y & 8) << 1;
      unsigned base_addr = character_data + (index << 4) + (tile_y & 7);
      uint64_t plane = 
         (uint64_t)READ_VRAMW(base_addr) | 
         ((uint64_t)READ_VRAMW(base_addr + 8) << 16) |
         ((uint64_t)READ_VRAMW(base_addr + 16) << 32) |
         ((uint64_t)READ_VRAMW(base_addr + 24) << 48);

      unsigned start_x = ((map_index & 0x1f) + (map_index >> 5)) << 4;
      start_x -= hofs;
      start_x &= x_mask;

      ppu_render_tile_4bpp_16x16(pixels, plane, start_x, x_mask, pal, hflip, prio,
            mask_buf, z_prio, z_buffer);
   }
}




#endif
