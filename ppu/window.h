#ifndef __SSNES_WINDOW_H
#define __SSNES_WINDOW_H

#include <stdint.h>

static inline void generate_window_mask(uint8_t *mask_buf, unsigned left_mask, unsigned right_mask, bool inverse)
{
   uint16_t mask = inverse ? 0xff : 0;
   uint16_t unmask = mask ^ 0xff;

   for (unsigned i = 0; i < left_mask; i++)
      mask_buf[i] = unmask;
   for (unsigned i = right_mask; i < 256; i++)
      mask_buf[i] = unmask;
   for (unsigned i = left_mask; i < right_mask; i++)
      mask_buf[i] = mask;
}

static inline void generate_window_dual_mask(uint8_t *mask_buf, unsigned left_mask0, unsigned right_mask0, bool inverse0, unsigned left_mask1, unsigned right_mask1, bool inverse1, unsigned op)
{
   uint16_t mask0 = inverse0 ? 0xff : 0;
   uint16_t unmask0 = mask0 ^ 0xff;
   uint16_t mask1 = inverse1 ? 0xff : 0;
   uint16_t unmask1 = mask1 ^ 0xff;

   for (unsigned i = 0; i < left_mask0; i++)
      mask_buf[i] = unmask0;
   for (unsigned i = right_mask0; i < 256; i++)
      mask_buf[i] = unmask0;
   for (unsigned i = left_mask0; i < right_mask0; i++)
      mask_buf[i] = mask0;

   // Window logic when we have two windows.
   switch (op)
   {
      case 0: // OR
         for (unsigned i = 0; i < left_mask1; i++)
            mask_buf[i] |= unmask1;
         for (unsigned i = right_mask1; right_mask1 < 256; i++)
            mask_buf[i] |= unmask1;
         for (unsigned i = left_mask1; i < right_mask1; i++)
            mask_buf[i] |= mask1;
         break;

      case 1: // AND
         for (unsigned i = 0; i < left_mask1; i++)
            mask_buf[i] &= unmask1;
         for (unsigned i = right_mask1; right_mask1 < 256; i++)
            mask_buf[i] &= unmask1;
         for (unsigned i = left_mask1; i < right_mask1; i++)
            mask_buf[i] &= mask1;
         break;

      case 2: // XOR
         for (unsigned i = 0; i < left_mask1; i++)
            mask_buf[i] ^= unmask1;
         for (unsigned i = right_mask1; right_mask1 < 256; i++)
            mask_buf[i] ^= unmask1;
         for (unsigned i = left_mask1; i < right_mask1; i++)
            mask_buf[i] ^= mask1;
         break;

      case 3: // XNOR
         for (unsigned i = 0; i < left_mask1; i++)
            mask_buf[i] ^= ~unmask1;
         for (unsigned i = right_mask1; right_mask1 < 256; i++)
            mask_buf[i] ^= ~unmask1;
         for (unsigned i = left_mask1; i < right_mask1; i++)
            mask_buf[i] ^= ~mask1;
         break;
   }
}

// Generate a masking buffer for windows. Returns a generic all-0xffff mask buffer if we don't use any windows to avoid branching in core rendering routine ... :)
static inline const uint8_t* ppu_window_generate_mask_bg1(uint8_t *mask_buf)
{
   unsigned window_mode = PPU.w12sel & 0x0a;
   if (!window_mode) // No windows for BG, return a static buffer that doesn't mask at all. :)
      return window_mask_none_buf;
   
   switch (window_mode)
   {
      case 2:
         generate_window_mask(mask_buf, PPU.wh0, PPU.wh1, PPU.w12sel & 0x01);
         break;

      case 8:
         generate_window_mask(mask_buf, PPU.wh2, PPU.wh3, PPU.w12sel & 0x10);
         break;

      case 0xa:
         generate_window_dual_mask(mask_buf, PPU.wh0, PPU.wh1, PPU.w12sel & 0x01, PPU.wh2, PPU.wh3, PPU.w12sel & 0x10, PPU.wbglog & 3);
         break;
   }

   return mask_buf;
}

static inline const uint8_t* ppu_window_generate_mask_bg2(uint8_t *mask_buf)
{
   unsigned window_mode = PPU.w12sel & 0xa0;
   if (!window_mode) // No windows for BG, return a static buffer that doesn't mask at all. :)
      return window_mask_none_buf;

   return mask_buf;
}

static inline const uint8_t* ppu_window_generate_mask_bg3(uint8_t *mask_buf)
{
   unsigned window_mode = PPU.w34sel & 0x0a;
   if (!window_mode) // No windows for BG, return a static buffer that doesn't mask at all. :)
      return window_mask_none_buf;


   return mask_buf;
}

static inline const uint8_t* ppu_window_generate_mask_bg4(uint8_t *mask_buf)
{
   unsigned window_mode = PPU.w34sel & 0xa0;
   if (!window_mode) // No windows for BG, return a static buffer that doesn't mask at all. :)
      return window_mask_none_buf;

   return mask_buf;
}

static inline const uint8_t* ppu_window_generate_mask_obj(uint8_t *mask_buf)
{
   unsigned window_mode = PPU.wobjsel & 0x0a;
   if (!window_mode) // No windows for BG, return a static buffer that doesn't mask at all. :)
      return window_mask_none_buf;

   return mask_buf;
}

#endif
