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

static void ppu_render_mode0(uint16_t *out_buf, unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   // Priority bits aren't handled yet :(
   if (PPU.tm & 0x08) // BG4
   {
      unsigned vofs = PPU.bg4vofs;
      unsigned line = (scanline + vofs + 1) & 0xff;
      unsigned hofs = 256 - PPU.bg4hofs;
      hofs &= 0xff;
      unsigned tilemap_addr = (PPU.bg4sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg34nba & 0xf0) << 8;
      unsigned scanline_mask = scanline & 7;

      ppu_render_bg_mode0(out_buf, line, scanline_mask, 
            hofs, tilemap_addr, character_data, 96);
   }

   if (PPU.tm & 0x04) // BG3
   {
      unsigned vofs = PPU.bg3vofs;
      unsigned line = (scanline + vofs + 1) & 0xff;
      unsigned hofs = 256 - PPU.bg3hofs;
      hofs &= 0xff;
      unsigned tilemap_addr = (PPU.bg3sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0x0f) << 12;
      unsigned scanline_mask = scanline & 7;

      ppu_render_bg_mode0(out_buf, line, scanline_mask, 
            hofs, tilemap_addr, character_data, 64);
   }

   if (PPU.tm & 0x02) // BG2
   {
      unsigned vofs = PPU.bg2vofs;
      unsigned line = (scanline + vofs + 1) & 0xff;
      unsigned hofs = 256 - PPU.bg2hofs;
      hofs &= 0xff;
      unsigned tilemap_addr = (PPU.bg2sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf0) << 8;
      unsigned scanline_mask = scanline & 7;

      ppu_render_bg_mode0(out_buf, line, scanline_mask, 
            hofs, tilemap_addr, character_data, 32);
   }

   if (PPU.tm & 0x01) // BG1
   {
      unsigned vofs = PPU.bg1vofs;
      unsigned line = (scanline + vofs + 1) & 0xff;
      unsigned hofs = 256 - PPU.bg1hofs;
      hofs &= 0xff;
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf) << 12;
      unsigned scanline_mask = scanline & 7;

      ppu_render_bg_mode0(out_buf, line, scanline_mask, 
            hofs, tilemap_addr, character_data, 0);
   }
   
   if (PPU.tm & 0x10)
   {
      const uint8_t *oam_hi = &MEM.oam.b[512];
      ppu_render_sprites(out_buf, oam_hi, scanline);
   }
}

#endif
