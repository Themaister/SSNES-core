#ifndef __PPU_MODE1_H
#define __PPU_MODE1_H

static inline void ppu_render_bg_mode1(uint16_t *pixels, unsigned scanline, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data)
{
   hofs &= 0xff;
   tilemap_addr += (((scanline & 0xff) >> 3) << 5);
   unsigned y = scanline & 7;

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i);
      unsigned pal = (tile & 0x1c00) >> 6;

      unsigned index = tile & 0x3ff;

      unsigned tile_y = isel_if(tile & 0x8000, 7 - y, y);
      unsigned base_addr = character_data + (index << 4) + tile_y;

      uint16_t plane0 = READ_VRAMW(base_addr);
      uint16_t plane1 = READ_VRAMW(base_addr + 8);

      if (tile & 0x4000)
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane0 >> i) & 1;
            color |= ((plane0 >> (i + 8)) & 1) << 1;
            color |= ((plane1 >> i) & 1) << 2;
            color |= ((plane1 >> (i + 8)) & 1) << 3;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }
      }
      else
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (plane0 >> (7 - i)) & 1;
            color |= ((plane0 >> (15 - i)) & 1) << 1;
            color |= ((plane1 >> (7 - i)) & 1) << 2;
            color |= ((plane1 >> (15 - i)) & 1) << 3;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }
      }
   }
}

// TODO: Not exactly optimized in any way ... ;) It renders too much.
// 512x512 BG. Cannot render it all. Render half of the scanline.
static inline void ppu_render_bg_mode1_16(uint16_t *pixels, unsigned scanline, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data)
{
   hofs &= 0x1ff;
   tilemap_addr += ((scanline & 0x1ff) >> 4) << 5; // Where is address of first tile?
   unsigned y = scanline & 15; // Find line in our sprite.

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i);
      unsigned pal = (tile & 0x1c00) >> 6;

      unsigned index = tile & 0x3ff;
      unsigned tile_y = isel_if(tile & 0x8000, 15 - y, y); // Vertical flip.

      // 16x16 tiles consist of 4 8x8 tiles, with offsets {0, 1, 15, 16}. Flipping flips everything.
      unsigned index0 = index + ((tile_y & 8) << 1);

      unsigned base_addr = character_data + (index0 << 4) + (tile_y & 7);
      uint16_t tile0_plane0 = READ_VRAMW(base_addr);
      uint16_t tile0_plane1 = READ_VRAMW(base_addr + 8);
      uint16_t tile1_plane0 = READ_VRAMW(base_addr + 16);
      uint16_t tile1_plane1 = READ_VRAMW(base_addr + 24);

      if (tile & 0x4000)
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (tile1_plane0 >> i) & 1;
            color |= ((tile1_plane0 >> (i + 8)) & 1) << 1;
            color |= ((tile1_plane1 >> i) & 1) << 2;
            color |= ((tile1_plane1 >> (i + 8)) & 1) << 3;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }

         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (tile0_plane0 >> i) & 1;
            color |= ((tile0_plane0 >> (i + 8)) & 1) << 1;
            color |= ((tile0_plane1 >> i) & 1) << 2;
            color |= ((tile0_plane1 >> (i + 8)) & 1) << 3;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }
      }
      else
      {
         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (tile0_plane0 >> (7 - i)) & 1;
            color |= ((tile0_plane0 >> (15 - i)) & 1) << 1;
            color |= ((tile0_plane1 >> (7 - i)) & 1) << 2;
            color |= ((tile0_plane1 >> (15 - i)) & 1) << 3;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }

         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (tile1_plane0 >> (7 - i)) & 1;
            color |= ((tile1_plane0 >> (15 - i)) & 1) << 1;
            color |= ((tile1_plane1 >> (7 - i)) & 1) << 2;
            color |= ((tile1_plane1 >> (15 - i)) & 1) << 3;
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }
      }
   }
}

static void ppu_render_mode1(uint16_t *out_buf, unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   if (PPU.tm & 0x04) // BG3
   {
      unsigned vofs = PPU.bg3vofs;
      unsigned line = (scanline + vofs) & 0xff;
      unsigned hofs = 256 - PPU.bg3hofs;
      unsigned tilemap_addr = (PPU.bg3sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0x0f) << 12;

      if (PPU.bgmode & 0x40)
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

   if (PPU.tm & 0x02) // BG2
   {
      unsigned vofs = PPU.bg2vofs;
      unsigned line = (scanline + vofs) & 0xff;
      unsigned hofs = 256 - PPU.bg2hofs;
      unsigned tilemap_addr = (PPU.bg2sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf0) << 8;

      if (PPU.bgmode & 0x20)
      {
         ppu_render_bg_mode1_16(out_buf, line,
               hofs, tilemap_addr, character_data);
      }
      else
      {
         ppu_render_bg_mode1(out_buf, line,
               hofs, tilemap_addr, character_data);
      }
   }

   if (PPU.tm & 0x01) // BG1
   {
      unsigned vofs = PPU.bg1vofs;
      unsigned line = (scanline + vofs) & 0xff;
      unsigned hofs = 256 - PPU.bg1hofs;
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf) << 12;

      if (PPU.bgmode & 0x10)
      {
         ppu_render_bg_mode1_16(out_buf, line,
               hofs, tilemap_addr, character_data);
      }
      else
      {
         ppu_render_bg_mode1(out_buf, line,
               hofs, tilemap_addr, character_data);
      }
   }

   if (PPU.tm & 0x10)
   {
      const uint8_t *oam_hi = &MEM.oam.b[512];
      ppu_render_sprites(out_buf, oam_hi, scanline);
   }
}



#endif
