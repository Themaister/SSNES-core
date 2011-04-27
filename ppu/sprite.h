#ifndef __PPU_SPRITE_H
#define __PPU_SPRITE_H

static inline void ppu_render_sprites(uint16_t *pixel, const uint8_t *oam_hi, unsigned scanline)
{
   /*
   for (int i = 31; i >= 0; i--)
   {
      // Any sprites to render?
      if (~oam_hi[i] & 0x55)
      {
         if (oam_hi[i] & 0x40)
            ppu_render_sprite(pixel, 


      }
   }
   */
}

#endif
