#ifndef __PPU_SPRITE_H
#define __PPU_SPRITE_H

// Hardcode shit for 8x8 :D
static inline void ppu_render_sprite(uint16_t *pixels, uint32_t oam, unsigned scanline, unsigned offset)
{
   unsigned y = (oam >> 8) & 0xff;

   int line = (int)scanline - (int)y;
   if (line >= 8 || line < 0)
      return;

   unsigned x = oam & 0xff;
   unsigned index = (oam >> 16) & 0xff;
   unsigned attr = oam >> 24;

   //fprintf(stderr, "OAM data: x = %u, y = %u, sprite = %u, attr = $%02x\n", x, y, index, attr);

   unsigned addr = offset + (index << 4);
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

static inline void ppu_render_sprites(uint16_t *line, const uint8_t *oam_hi, unsigned scanline)
{
   unsigned offset = ((unsigned)PPU.obsel & 7) << 13;
   for (int i = 31; i >= 0; i--)
   {
      // Any sprites to render?
      if (~oam_hi[i] & 0x55)
      {
         if (~oam_hi[i] & 0x40)
         {
            //fprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 3);
            ppu_render_sprite(line, READ_OAML((i << 2) + 3), scanline, offset);
         }
         if (~oam_hi[i] & 0x10)
         {
            //fprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 2);
            ppu_render_sprite(line, READ_OAML((i << 2) + 2), scanline, offset);
         }
         if (~oam_hi[i] & 0x04)
         {
            //fprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 1);
            ppu_render_sprite(line, READ_OAML((i << 2) + 1), scanline, offset);
         }
         if (~oam_hi[i] & 0x01)
         {
            //fprintf(stderr, "Rendering sprite: %d\n", (i << 2) + 0);
            ppu_render_sprite(line, READ_OAML((i << 2) + 0), scanline, offset);
         }
      }
   }
}

#endif
