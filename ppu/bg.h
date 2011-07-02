#ifndef __SSNES_PPU_BG_H
#define __SSNES_PPU_BG_H

#include <stdbool.h>
#include <stdint.h>

#include <assert.h>

#if 1
static void ppu_merge_bg(uint16_t * restrict dst, const uint16_t * restrict src, uint16_t * restrict z_dst, const uint16_t * restrict z_src)
{
   for (unsigned i = 0; i < 256; i++)
   {
      if (z_src[i] > z_dst[i])
      {
         dst[i] = src[i];
         z_dst[i] = z_src[i];
      }
   }
}

static inline void ppu_apply_window_mask(uint16_t * restrict z_buf, const uint16_t * restrict window_buf)
{
   for (unsigned i = 0; i < 256; i++)
      z_buf[i] &= window_buf[i];
}
#else
#include <emmintrin.h>
// SSE2 line merger.
static void ppu_merge_bg(uint16_t * restrict dst, const uint16_t * restrict src, uint16_t * restrict z_dst, const uint16_t * restrict z_src)
{
   for (unsigned i = 0; i < 32; i += 2)
   {
      __m128i z_dst_0 = _mm_load_si128((__m128i*)z_dst + i + 0);
      __m128i z_dst_1 = _mm_load_si128((__m128i*)z_dst + i + 1);

      const __m128i z_src_0 = _mm_load_si128((const __m128i*)z_src + i + 0);
      const __m128i z_src_1 = _mm_load_si128((const __m128i*)z_src + i + 1);

      __m128i dst_0 = _mm_load_si128((__m128i*)dst + i + 0);
      __m128i dst_1 = _mm_load_si128((__m128i*)dst + i + 1);

      const __m128i src_0 = _mm_load_si128((const __m128i*)src + i + 0);
      const __m128i src_1 = _mm_load_si128((const __m128i*)src + i + 1);

      __m128i gt_mask0 = _mm_cmpgt_epi16(z_src_0, z_dst_0);
      __m128i gt_mask1 = _mm_cmpgt_epi16(z_src_1, z_dst_1);

      dst_0 = _mm_or_si128(_mm_and_si128(gt_mask0, src_0), _mm_andnot_si128(gt_mask0, dst_0));
      dst_1 = _mm_or_si128(_mm_and_si128(gt_mask1, src_1), _mm_andnot_si128(gt_mask1, dst_1));

      _mm_store_si128((__m128i*)dst + i + 0, dst_0);
      _mm_store_si128((__m128i*)dst + i + 1, dst_1);

      _mm_store_si128((__m128i*)z_dst + i + 0, _mm_max_epi16(z_dst_0, z_src_0));
      _mm_store_si128((__m128i*)z_dst + i + 1, _mm_max_epi16(z_dst_1, z_src_1));
   }
}

// Apply window mask. Set Z-buf to 0 if we mask away. :)
static inline void ppu_apply_window_mask(uint16_t * restrict z_buf, const uint16_t * restrict window_buf)
{
   for (unsigned i = 0; i < 32; i += 2)
   {
      __m128i z0 = _mm_load_si128((__m128i*)z_buf + i + 0);
      __m128i z1 = _mm_load_si128((__m128i*)z_buf + i + 1);

      const __m128i win0 = _mm_load_si128((const __m128i*)window_buf + i + 0);
      const __m128i win1 = _mm_load_si128((const __m128i*)window_buf + i + 1);

      __m128i res0 = _mm_and_si128(z0, win0);
      __m128i res1 = _mm_and_si128(z1, win1);

      _mm_store_si128((__m128i*)z_buf + i + 0, res0);
      _mm_store_si128((__m128i*)z_buf + i + 1, res1);
   }
}
#endif

// This should be very SIMD friendly :) :)
static void ppu_merge_bgs(uint16_t (* restrict linebuf)[1024], uint16_t (* restrict z_buf)[1024], uint16_t * restrict out_buf, const unsigned *merge_list, unsigned bgs)
{
   uint16_t * restrict line_dst = NULL;
   const uint16_t * restrict line_src = NULL;
   uint16_t * restrict z_dst = NULL;
   const uint16_t * restrict z_src = NULL;

   for (unsigned j = 0; j < bgs - 1; j++)
   {
      unsigned dst = merge_list[j + 1];
      unsigned src = merge_list[j];

      line_dst = linebuf[dst];
      line_src = linebuf[src];
      z_dst = z_buf[dst];
      z_src = z_buf[src];

      ppu_merge_bg(line_dst, line_src, z_dst, z_src);
   }
}



// A fuckton of code incoming! :D

static inline void ppu_render_tile_2bpp_8x8(
      uint16_t * restrict pixels, 
      uint16_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
{
   unsigned z_value = z_prio[prio];
   if (hflip)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> i) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 8)) & 1) << 1;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }
   }
   else
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (7 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (15 - i)) & 1) << 1;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }
   }
}

static inline void ppu_render_tile_2bpp_16x16(
      uint16_t * restrict pixels, 
      uint32_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
{
   unsigned z_value = z_prio[prio];
   if (hflip)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> i) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 8)) & 1) << 1;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (i + 16)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 24)) & 1) << 1;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }
   }
   else
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (7 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (15 - i)) & 1) << 1;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (23 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (31 - i)) & 1) << 1;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }
   }
}

static inline void ppu_render_tile_4bpp_8x8(
      uint16_t * restrict pixels, 
      uint32_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
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

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
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

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }
   }
}

static inline void ppu_render_tile_4bpp_16x16(
      uint16_t * restrict pixels, 
      uint64_t plane, unsigned x, unsigned x_mask,
      unsigned palette, bool hflip, bool prio,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
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

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (i + 32)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (i + 40)) & 1) << 1;
         color |= ((plane >> (i + 48)) & 1) << 2;
         color |= ((plane >> (i + 56)) & 1) << 3;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
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

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane >> (39 - i)) & 1; // Pull out the two bitplane values.
         color |= ((plane >> (47 - i)) & 1) << 1;
         color |= ((plane >> (55 - i)) & 1) << 2;
         color |= ((plane >> (63 - i)) & 1) << 3;

         z_buffer[x] = z_value & isel_if(color, 0xff, 0); // Mask transparency
         pixels[x] = READ_CGRAMW(palette + color);
         x = (x + 1) & x_mask;
      }
   }
}


static inline void ppu_render_bg_2bpp_8x8 (
      uint16_t * restrict pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
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

      unsigned start_x = ((map_index & 0x1f) + (map_index >> 5)) << 3;
      start_x -= hofs;
      start_x &= x_mask;

      ppu_render_tile_2bpp_8x8(pixels, plane, start_x, x_mask, pal, hflip, prio,
            z_prio, z_buffer);
   }
}

static inline void ppu_render_bg_4bpp_8x8 (
      uint16_t * restrict pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
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
      unsigned pal = (tile_attr & 0x1c00) >> 6;
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

      ppu_render_tile_4bpp_8x8(pixels, plane, start_x, x_mask, pal, hflip, prio,
            z_prio, z_buffer);
   }
}


static inline void ppu_render_bg_2bpp_16x16 (
      uint16_t * restrict pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
{
   scanline += vofs + 1;
   scanline &= isel_if(vmirror, 0x3ff, 0x1ff);
   
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
            z_prio, z_buffer);
   }
}

static inline void ppu_render_bg_4bpp_16x16 (
      uint16_t * restrict pixels,
      unsigned scanline, unsigned vofs, bool vmirror,
      unsigned hofs, bool hmirror,
      unsigned tilemap_addr, unsigned character_data, unsigned base_palette,
      const uint16_t * restrict z_prio, uint16_t * restrict z_buffer)
{
   scanline += vofs + 1;
   scanline &= isel_if(vmirror, 0x3ff, 0x1ff);
   
   tilemap_addr += ((scanline & 0x1ff) >> 4) << 5;
   tilemap_addr += (scanline & 0x200) << 2;

   unsigned y = scanline & 15;

   unsigned x_mask = isel_if(hmirror, 0x3ff, 0x1ff);
   for (unsigned i = 0; i <= 256; i += 16)
   {
      unsigned x = (i + hofs) & x_mask;
      unsigned map_index = ((x & 0x1ff) >> 4) + ((x & 0x200) << 1);
      unsigned tile_attr = READ_VRAMW(tilemap_addr + map_index);
      unsigned pal = (tile_attr & 0x1c00) >> 6;
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
            z_prio, z_buffer);
   }
}




#endif
