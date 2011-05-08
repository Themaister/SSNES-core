#ifndef __PPU_MODE0_H
#define __PPU_MODE0_H


static void ppu_render_mode0(uint16_t *out_buf, unsigned scanline)
{
   if (PPU.inidisp & 0x80)
      return;

   unsigned merge_list[4];
   unsigned merge_ptr = 0;

   uint8_t window_mask[256] ALIGNED;
   uint16_t z_buf[4][1024] ALIGNED;
   uint16_t empty_z[1024] ALIGNED;
   uint16_t line_buf[4][1024] ALIGNED;
   const uint8_t *use_win_mask;

   // Priority bits aren't handled yet :(
   if (PPU.tm & 0x08) // BG4
   {
      merge_list[merge_ptr++] = 3;
      unsigned hofs = PPU.bg4hofs;
      unsigned vofs = PPU.bg4vofs;
      unsigned tilemap_addr = (PPU.bg4sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg34nba & 0xf0) << 8;
      bool hmirror = PPU.bg4sc & 0x01;
      bool vmirror = PPU.bg4sc & 0x02;

      const uint16_t z_prio[] = {1, 4};

      use_win_mask = ppu_window_generate_mask_bg4(window_mask);

      if (PPU.bgmode & 0x80)
      {
         ppu_render_bg_2bpp_16x16(
               line_buf[3],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 96,
               use_win_mask, z_prio, z_buf[3]);
      }
      else
      {
         ppu_render_bg_2bpp_8x8(
               line_buf[3],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 96,
               use_win_mask, z_prio, z_buf[3]);
      }
   }

   if (PPU.tm & 0x04) // BG3
   {
      merge_list[merge_ptr++] = 2;
      unsigned hofs = PPU.bg3hofs;
      unsigned vofs = PPU.bg3vofs;
      unsigned tilemap_addr = (PPU.bg3sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg34nba & 0x0f) << 12;
      bool hmirror = PPU.bg3sc & 0x01;
      bool vmirror = PPU.bg3sc & 0x02;

      const uint16_t z_prio[] = {2, 5};

      use_win_mask = ppu_window_generate_mask_bg3(window_mask);

      if (PPU.bgmode & 0x40)
      {
         ppu_render_bg_2bpp_16x16(
               line_buf[2],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 64,
               use_win_mask, z_prio, z_buf[2]);
      }
      else
      {
         ppu_render_bg_2bpp_8x8(
               line_buf[2],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 64,
               use_win_mask, z_prio, z_buf[2]);
      }
   }

   if (PPU.tm & 0x02) // BG2
   {
      merge_list[merge_ptr++] = 1;
      unsigned hofs = PPU.bg2hofs;
      unsigned vofs = PPU.bg2vofs;
      unsigned tilemap_addr = (PPU.bg2sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0xf0) << 8;
      bool hmirror = PPU.bg2sc & 0x01;
      bool vmirror = PPU.bg2sc & 0x02;

      const uint16_t z_prio[] = {7, 10};

      use_win_mask = ppu_window_generate_mask_bg2(window_mask);

      if (PPU.bgmode & 0x20)
      {
         ppu_render_bg_2bpp_16x16(
               line_buf[1],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 32,
               use_win_mask, z_prio, z_buf[1]);
      }
      else
      {
         ppu_render_bg_2bpp_8x8(
               line_buf[1],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 32,
               use_win_mask, z_prio, z_buf[1]);
      }
   }

   if (PPU.tm & 0x01) // BG1
   {
      merge_list[merge_ptr++] = 0;
      unsigned hofs = PPU.bg1hofs;
      unsigned vofs = PPU.bg1vofs;
      unsigned tilemap_addr = (PPU.bg1sc & 0xfc) << 8;
      unsigned character_data = (PPU.bg12nba & 0x0f) << 12;
      bool hmirror = PPU.bg1sc & 0x01;
      bool vmirror = PPU.bg1sc & 0x02;

      const uint16_t z_prio[] = {8, 11};

      use_win_mask = ppu_window_generate_mask_bg1(window_mask);

      if (PPU.bgmode & 0x10)
      {
         ppu_render_bg_2bpp_16x16(
               line_buf[0],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 0,
               use_win_mask, z_prio, z_buf[0]);
      }
      else
      {
         ppu_render_bg_2bpp_8x8(
               line_buf[0],
               scanline, vofs, vmirror,
               hofs, hmirror,
               tilemap_addr, character_data, 0,
               use_win_mask, z_prio, z_buf[0]);
      }
   }

   memset(empty_z, 0, sizeof(empty_z));

   if (merge_ptr > 1)
   {
      ppu_merge_bgs(line_buf, z_buf, out_buf, merge_list, merge_ptr);
      ppu_merge_bg(out_buf, line_buf[merge_list[merge_ptr - 1]], empty_z, z_buf[merge_list[merge_ptr - 1]]);
   }
   else if (merge_ptr == 1)
   {
      ppu_merge_bg(out_buf, line_buf[merge_list[0]], empty_z, z_buf[merge_list[0]]);
   }
   
   if (PPU.tm & 0x10)
   {
      const uint8_t *oam_hi = &MEM.oam.b[512];
      const uint16_t z_prio[] = {3, 6, 9, 12};
      use_win_mask = ppu_window_generate_mask_obj(window_mask);
      ppu_render_sprites(out_buf, oam_hi, scanline, use_win_mask, z_prio, empty_z);
   }
}

#endif
