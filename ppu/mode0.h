#ifndef __PPU_MODE0_H
#define __PPU_MODE0_H

// TODO: Mirroring and proper priority handling ... ;) And lots more stuff ...

// 256x256 BG. Render the whole scanline, but shift it.
static inline void ppu_render_bg_mode0(uint16_t *pixels, unsigned scanline, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data,
      unsigned base_palette)
{
   hofs &= 0xff;
   tilemap_addr += ((scanline & 0xff) >> 3) << 5;
   unsigned y = scanline & 7;

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i);
      unsigned pal = (tile & 0x1c00) >> 8;
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

// TODO: Not exactly optimized in any way ... ;) It renders too much.
// 512x512 BG. Cannot render it all. Render half of the scanline.
static inline void ppu_render_bg_mode0_16(uint16_t *pixels, unsigned scanline, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data,
      unsigned base_palette)
{
   hofs &= 0x1ff;
   tilemap_addr += ((scanline & 0x1ff) >> 4) << 5; // Where is address of first tile?
   unsigned y = scanline & 15; // Find line in our sprite.

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i);
      unsigned pal = (tile & 0x1c00) >> 8;
      pal += base_palette;

      unsigned index = tile & 0x3ff;
      unsigned tile_y = isel_if(tile & 0x8000, 15 - y, y); // Vertical flip.

      // 16x16 tiles consist of 4 8x8 tiles, with offsets {0, 1, 15, 16}. Flipping flips everything.
      unsigned index0 = index + ((tile_y & 8) << 1);

      unsigned base_addr = character_data + (index0 << 3) + (tile_y & 7);
      uint16_t plane0 = READ_VRAMW(base_addr);
      uint16_t plane1 = READ_VRAMW(base_addr + 8);

      if (tile & 0x4000)
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane1 >> i) & 1;
            color |= ((plane1 >> (i + 8)) & 1) << 1;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }

         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane0 >> i) & 1;
            color |= ((plane0 >> (i + 8)) & 1) << 1;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }
      }
      else
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane0 >> (7 - i)) & 1;
            color |= ((plane0 >> (15 - i)) & 1) << 1;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }

         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane1 >> (7 - i)) & 1;
            color |= ((plane1 >> (15 - i)) & 1) << 1;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }
      }
   }
}

static void ppu_render_mode0(uint16_t *out_buf, unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   // Priority bits aren't handled yet :(
   if (PPU.tm & 0x08) // BG4
   {
      unsigned vofs = PPU.bg4vofs;
      unsigned line = (scanline + vofs + 1);
      unsigned hofs = 1024 - (PPU.bg4hofs & 0x3ff);
      unsigned tilemap_addr = (PPU.bg4sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg34nba & 0xf0) << 8;

      // 16x16 tiles.
      if (PPU.bgmode & 0x80)
      {
         ppu_render_bg_mode0_16(out_buf, line,
               hofs, tilemap_addr, character_data, 96);
      }
      else
      {
         ppu_render_bg_mode0(out_buf, line, 
               hofs, tilemap_addr, character_data, 96);
      }
   }

   if (PPU.tm & 0x04) // BG3
   {
      unsigned vofs = PPU.bg3vofs;
      unsigned line = (scanline + vofs + 1);
      unsigned hofs = 1024 - (PPU.bg3hofs & 0x3ff);
      unsigned tilemap_addr = (PPU.bg3sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0x0f) << 12;

      if (PPU.bgmode & 0x40)
      {
         ppu_render_bg_mode0_16(out_buf, line,
               hofs, tilemap_addr, character_data, 64);
      }
      else
      {
         ppu_render_bg_mode0(out_buf, line,
               hofs, tilemap_addr, character_data, 64);
      }
   }

   if (PPU.tm & 0x02) // BG2
   {
      unsigned vofs = PPU.bg2vofs;
      unsigned line = (scanline + vofs + 1);
      unsigned hofs = 1024 - (PPU.bg2hofs & 0x3ff);
      unsigned tilemap_addr = (PPU.bg2sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf0) << 8;

      if (PPU.bgmode & 0x20)
      {
         ppu_render_bg_mode0_16(out_buf, line, 
               hofs, tilemap_addr, character_data, 32);
      }
      else
      {
         ppu_render_bg_mode0(out_buf, line, 
               hofs, tilemap_addr, character_data, 32);
      }
   }

   if (PPU.tm & 0x01) // BG1
   {
      unsigned vofs = PPU.bg1vofs;
      unsigned line = (scanline + vofs + 1);
      unsigned hofs = 1024 - (PPU.bg1hofs & 0x3ff);
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf) << 12;

      if (PPU.bgmode & 0x10)
      {
         ppu_render_bg_mode0_16(out_buf, line, 
               hofs, tilemap_addr, character_data, 0);
      }
      else
      {
         ppu_render_bg_mode0(out_buf, line, 
               hofs, tilemap_addr, character_data, 0);
      }
   }
   
   if (PPU.tm & 0x10)
   {
      const uint8_t *oam_hi = &MEM.oam.b[512];
      ppu_render_sprites(out_buf, oam_hi, scanline);
   }
}

#endif
