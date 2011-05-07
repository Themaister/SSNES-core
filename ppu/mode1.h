#ifndef __PPU_MODE1_H
#define __PPU_MODE1_H

static inline void ppu_render_bg_mode1(uint16_t *pixels, unsigned scanline, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data, const uint8_t *mask_win)
{
   tilemap_addr += (((scanline & 0xff) >> 3) << 5);
   tilemap_addr += (scanline & 0x100) << 5;
   unsigned y = scanline & 7;

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i + ((hofs & 0x100) << 4));
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
            color &= mask_win[hofs];
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
            color &= mask_win[hofs];
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }
      }
   }
}

// TODO: Not exactly optimized in any way ... ;) It renders too much.
// 512x512 BG. Cannot render it all. Render half of the scanline.
static inline void ppu_render_bg_mode1_16(uint16_t *pixels, unsigned scanline, unsigned hofs,
      unsigned tilemap_addr, unsigned character_data, const uint8_t *mask_win)
{
   tilemap_addr += ((scanline & 0x1ff) >> 4) << 5; // Where is address of first tile?
   tilemap_addr += (scanline & 0x200) << 4;
   unsigned y = scanline & 15; // Find line in our sprite.

   for (unsigned i = 0; i < 32; i++)
   {
      uint16_t tile = READ_VRAMW(tilemap_addr + i + ((hofs & 0x200) << 3));
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
            color &= mask_win[hofs & 0xff];
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0x1ff;
         }

         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (tile0_plane0 >> i) & 1;
            color |= ((tile0_plane0 >> (i + 8)) & 1) << 1;
            color |= ((tile0_plane1 >> i) & 1) << 2;
            color |= ((tile0_plane1 >> (i + 8)) & 1) << 3;
            color &= mask_win[hofs & 0xff];
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
            color &= mask_win[hofs & 0xff];
            iup_if(pixels[hofs], color, READ_CGRAMW(color + pal));
            hofs = (hofs + 1) & 0xff;
         }

         for (unsigned i = 0; i < 8; i++)
         {
            unsigned color = (tile1_plane0 >> (7 - i)) & 1;
            color |= ((tile1_plane0 >> (15 - i)) & 1) << 1;
            color |= ((tile1_plane1 >> (7 - i)) & 1) << 2;
            color |= ((tile1_plane1 >> (15 - i)) & 1) << 3;
            color &= mask_win[hofs & 0xff];
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

   uint8_t window_mask[256];
   const uint8_t *use_win_mask;

   if (PPU.tm & 0x04) // BG3
   {
      unsigned vofs = PPU.bg3vofs;
      unsigned line = (scanline + vofs) & 0xff;
      unsigned hofs = 1024 - PPU.bg3hofs;
      unsigned tilemap_addr = (PPU.bg3sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0x0f) << 12;

      use_win_mask = ppu_window_generate_mask_bg3(window_mask);

      if (PPU.bgmode & 0x40)
      {
         hofs &= isel_if(PPU.bg3sc & 0x01, 0x3ff, 0x1ff);
         line &= isel_if(PPU.bg3sc & 0x02, 0x3ff, 0x1ff);
         ppu_render_bg_mode0_16(out_buf, line,
               hofs, tilemap_addr, character_data, 0, use_win_mask);
      }
      else
      {
         hofs &= isel_if(PPU.bg3sc & 0x01, 0x1ff, 0xff);
         line &= isel_if(PPU.bg3sc & 0x02, 0x1ff, 0xff);
         ppu_render_bg_mode0(out_buf, line,
               hofs, tilemap_addr, character_data, 0, use_win_mask);
      }
   }

   if (PPU.tm & 0x02) // BG2
   {
      unsigned vofs = PPU.bg2vofs;
      unsigned line = (scanline + vofs) & 0xff;
      unsigned hofs = 1024 - PPU.bg2hofs;
      unsigned tilemap_addr = (PPU.bg2sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf0) << 8;

      use_win_mask = ppu_window_generate_mask_bg2(window_mask);

      if (PPU.bgmode & 0x20)
      {
         hofs &= isel_if(PPU.bg2sc & 0x01, 0x3ff, 0x1ff);
         line &= isel_if(PPU.bg2sc & 0x02, 0x3ff, 0x1ff);
         ppu_render_bg_mode1_16(out_buf, line,
               hofs, tilemap_addr, character_data, use_win_mask);
      }
      else
      {
         hofs &= isel_if(PPU.bg2sc & 0x01, 0x1ff, 0xff);
         line &= isel_if(PPU.bg2sc & 0x02, 0x1ff, 0xff);
         ppu_render_bg_mode1(out_buf, line,
               hofs, tilemap_addr, character_data, use_win_mask);
      }
   }

   if (PPU.tm & 0x01) // BG1
   {
      unsigned vofs = PPU.bg1vofs;
      unsigned line = (scanline + vofs) & 0xff;
      unsigned hofs = 1024 - PPU.bg1hofs;
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf) << 12;

      use_win_mask = ppu_window_generate_mask_bg1(window_mask);

      if (PPU.bgmode & 0x10)
      {
         hofs &= isel_if(PPU.bg1sc & 0x01, 0x3ff, 0x1ff);
         line &= isel_if(PPU.bg1sc & 0x02, 0x3ff, 0x1ff);
         ppu_render_bg_mode1_16(out_buf, line,
               hofs, tilemap_addr, character_data, use_win_mask);
      }
      else
      {
         hofs &= isel_if(PPU.bg1sc & 0x01, 0x1ff, 0xff);
         line &= isel_if(PPU.bg1sc & 0x02, 0x1ff, 0xff);
         ppu_render_bg_mode1(out_buf, line,
               hofs, tilemap_addr, character_data, use_win_mask);
      }
   }

   if (PPU.tm & 0x10)
   {
      const uint8_t *oam_hi = &MEM.oam.b[512];
      use_win_mask = ppu_window_generate_mask_obj(window_mask);
      ppu_render_sprites(out_buf, oam_hi, scanline, use_win_mask);
   }
}



#endif
