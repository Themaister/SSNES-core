#ifndef __PPU_SPRITE_H
#define __PPU_SPRITE_H

// Hardcode shit for 8x8 :D
static void ppu_render_sprite(uint16_t *pixels, uint32_t oam, unsigned scanline, unsigned offset, unsigned name)
{
   unsigned y = (oam >> 8) & 0xff;

   int line = (int)scanline - (int)y;
   if (line >= 8 || line < 0)
      return;

   unsigned x = oam & 0xff;
   unsigned index = (oam >> 16) & 0xff;
   unsigned attr = oam >> 24;

   offset += isel_if(attr & 0x01, name, 0);

   //dprintf(stderr, "OAM data: x = %u, y = %u, sprite = %u, attr = $%02x\n", x, y, index, attr);

   unsigned addr = (offset + (index << 4)) & 0x7fff;
   unsigned pal = 128 + ((attr & 0xe) << 3);
   unsigned real_line = isel_if(attr & 0x80, 7 - line, line);

   uint16_t plane0 = READ_VRAMW(addr + real_line);
   uint16_t plane1 = READ_VRAMW(addr + real_line + 8);

   if (attr & 0x40)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (plane0 >> i) & 1;
         color |= ((plane0 >> (i + 8)) & 1) << 1;
         color |= ((plane1 >> i) & 1) << 2;
         color |= ((plane1 >> (i + 8)) & 1) << 3;

         iup_if(pixels[x & 0xff], color, READ_CGRAMW(pal + color));
         x++;
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

         iup_if(pixels[x & 0xff], color, READ_CGRAMW(pal + color));
         x++;
      }
   }
}

static void ppu_render_sprite_big(uint16_t *pixels, uint32_t oam, unsigned scanline, unsigned offset, unsigned name)
{
   unsigned y = (oam >> 8) & 0xff;
   int line = (int)scanline - (int)y;
   if (line >= 16 || line < 0)
      return;

   unsigned x = oam & 0xff;
   unsigned index = (oam >> 16) & 0xff;
   unsigned attr = oam >> 24;

   offset += isel_if(attr & 0x01, name, 0);

   unsigned pal = 128 + ((attr & 0xe) << 3);
   unsigned real_line = isel_if(attr & 0x80, 15 - line, line);

   // Sprite tiles "wrap" in terms of a 16x16 grid.
   const uint16_t indices[4] = {
      index,                     (index & 0xf0) | ((index + 1) & 0x0f),
      (index + 0x10) & 0xff,     ((index + 0x10) & 0xf0) | ((index + 1) & 0x0f)
   };

   unsigned index0 = indices[isel_if(real_line & 8, 2, 0)];
   unsigned index1 = indices[isel_if(real_line & 8, 3, 1)];
   unsigned addr0 = (offset + (index0 << 4)) & 0x7fff;
   unsigned addr1 = (offset + (index1 << 4)) & 0x7fff;

   unsigned sprite_line = real_line & 7;
   uint16_t tile0_plane0 = READ_VRAMW(addr0 + sprite_line);
   uint16_t tile0_plane1 = READ_VRAMW(addr0 + sprite_line + 8);
   uint16_t tile1_plane0 = READ_VRAMW(addr1 + sprite_line);
   uint16_t tile1_plane1 = READ_VRAMW(addr1 + sprite_line + 8);

   if (attr & 0x40)
   {
      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (tile1_plane0 >> i) & 1;
         color |= ((tile1_plane0 >> (i + 8)) & 1) << 1;
         color |= ((tile1_plane1 >> i) & 1) << 2;
         color |= ((tile1_plane1 >> (i + 8)) & 1) << 3;

         iup_if(pixels[x & 0xff], color, READ_CGRAMW(pal + color));
         x++;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (tile0_plane0 >> i) & 1;
         color |= ((tile0_plane0 >> (i + 8)) & 1) << 1;
         color |= ((tile0_plane1 >> i) & 1) << 2;
         color |= ((tile0_plane1 >> (i + 8)) & 1) << 3;

         iup_if(pixels[x & 0xff], color, READ_CGRAMW(pal + color));
         x++;
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

         iup_if(pixels[x & 0xff], color, READ_CGRAMW(pal + color));
         x++;
      }

      for (unsigned i = 0; i < 8; i++)
      {
         unsigned color = (tile1_plane0 >> (7 - i)) & 1;
         color |= ((tile1_plane0 >> (15 - i)) & 1) << 1;
         color |= ((tile1_plane1 >> (7 - i)) & 1) << 2;
         color |= ((tile1_plane1 >> (15 - i)) & 1) << 3;

         iup_if(pixels[x & 0xff], color, READ_CGRAMW(pal + color));
         x++;
      }
   }
}

static inline void ppu_render_sprites(uint16_t *line, const uint8_t *oam_hi, unsigned scanline)
{
   unsigned offset = ((unsigned)PPU.obsel & 7) << 13;
   unsigned name = (((unsigned)PPU.obsel & 0x18) + 1) << 9;
   for (int i = 31; i >= 0; i--)
   {
      // Any sprites to render?
      if (~oam_hi[i] & 0x55)
      {
         if (~oam_hi[i] & 0x40)
         {
            //dprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 3);
            if (oam_hi[i] & 0x80)
               ppu_render_sprite_big(line, READ_OAML((i << 2) + 3), scanline, offset, name);
            else
               ppu_render_sprite(line, READ_OAML((i << 2) + 3), scanline, offset, name);
         }
         if (~oam_hi[i] & 0x10)
         {
            //dprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 2);
            if (oam_hi[i] & 0x20)
               ppu_render_sprite_big(line, READ_OAML((i << 2) + 2), scanline, offset, name);
            else
               ppu_render_sprite(line, READ_OAML((i << 2) + 2), scanline, offset, name);
         }
         if (~oam_hi[i] & 0x04)
         {
            //dprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 1);
            if (oam_hi[i] & 0x08)
               ppu_render_sprite_big(line, READ_OAML((i << 2) + 1), scanline, offset, name);
            else
               ppu_render_sprite(line, READ_OAML((i << 2) + 1), scanline, offset, name);
         }
         if (~oam_hi[i] & 0x01)
         {
            //dprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 0);
            if (oam_hi[i] & 0x02)
               ppu_render_sprite_big(line, READ_OAML((i << 2) + 0), scanline, offset, name);
            else
               ppu_render_sprite(line, READ_OAML((i << 2) + 0), scanline, offset, name);
         }
      }
   }
}

#endif
