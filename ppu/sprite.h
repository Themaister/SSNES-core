#ifndef __PPU_SPRITE_H
#define __PPU_SPRITE_H

// Hardcode shit for 8x8 :D
static inline void ppu_render_sprite(uint16_t *pixels, uint32_t oam, unsigned scanline)
{
   unsigned y = (oam >> 8) & 0xff;

   int line = (int)scanline - (int)y;
   if (line >= 8 || line < 0)
      return;

   unsigned offset = ((unsigned)PPU.obsel & 7) << 13;
   unsigned index = (oam >> 16) & 0xff;
   unsigned addr = offset + (index << 4);

   unsigned x = oam & 0xff;
   unsigned attr = oam >> 24;
   unsigned pal = 128 + ((attr & 0xe) << 3);

   uint16_t plane0 = READ_VRAMW(addr + line);
   uint16_t plane1 = READ_VRAMW(addr + line + 8);
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

static inline void ppu_render_sprites(uint16_t *line, const uint8_t *oam_hi, unsigned scanline)
{
   for (int i = 31; i >= 0; i--)
   {
      // Any sprites to render?
      if (~oam_hi[i] & 0x55)
      {
         if (~oam_hi[i] & 0x40)
            ppu_render_sprite(line, READ_OAML((i << 4) + 3), scanline);
         if (~oam_hi[i] & 0x10)
            ppu_render_sprite(line, READ_OAML((i << 4) + 2), scanline);
         if (~oam_hi[i] & 0x04)
            ppu_render_sprite(line, READ_OAML((i << 4) + 1), scanline);
         if (~oam_hi[i] & 0x01)
            ppu_render_sprite(line, READ_OAML((i << 4) + 0), scanline);
      }
   }
}

#endif
