#include "bus.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdio.h>

uint8_t ssnes_bus_read_2000(uint32_t addr)
{
   CPU.status.cycles += 8;
   return 0;
}

uint8_t ssnes_bus_read_4000(uint32_t addr)
{
   CPU.status.cycles += 8;

   uint16_t daddr = addr & 0xffff;
   switch (daddr)
   {
      case 0x4212: // HVBJOY
         return PPU.hvbjoy;

      case 0x4218: // JOY1L
         return STATUS.input[0].data1.b.l;
      case 0x4219: // JOY1H
         return STATUS.input[0].data1.b.h;
      case 0x421a: // JOY2L
         return STATUS.input[1].data1.b.l;
      case 0x421b: // JOY2H
         return STATUS.input[1].data1.b.h;
      case 0x421c: // JOY3L
         return STATUS.input[0].data2.b.l;
      case 0x421d: // JOY3H
         return STATUS.input[0].data2.b.h;
      case 0x421e: // JOY4L
         return STATUS.input[1].data2.b.l;
      case 0x421f: // JOY4H
         return STATUS.input[1].data2.b.h;
   }

   return 0;
}

uint8_t ssnes_bus_read_6000(uint32_t addr)
{
   CPU.status.cycles += 8;
   return 0;
}

void ssnes_bus_write_2000(uint32_t addr, uint8_t data)
{
   static const unsigned vram_addr_inc[] = {1, 32, 128, 128};
   CPU.status.cycles += 8;

   uint16_t daddr = addr & 0xffff;
   switch (daddr)
   {
      /////////////// PPU

      case 0x2100: // INIDISP
         PPU.inidisp = data;
         return;

      case 0x2105: // BGMODE
         PPU.bgmode = data;
         return;

      case 0x2107: // BG1SC
         PPU.bg1sc = data;
         return;
      case 0x2108:
         PPU.bg2sc = data;
         return;
      case 0x2109:
         PPU.bg3sc = data;
         return;
      case 0x210a:
         PPU.bg4sc = data;
         return;

      case 0x210b: // BG12NBA
         PPU.bg12nba = data;
         return;
      case 0x210c:
         PPU.bg34nba = data;
         return;

      case 0x210d: // BG1HOFS
         // Probably not accurate at all :)
         PPU.bg1hofs = (PPU.bg1hofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x210e:
         PPU.bg1vofs = (PPU.bg1vofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x210f:
         PPU.bg2hofs = (PPU.bg2hofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x2110:
         PPU.bg2vofs = (PPU.bg2vofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x2111:
         PPU.bg3hofs = (PPU.bg3hofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x2112:
         PPU.bg3vofs = (PPU.bg3vofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x2113:
         PPU.bg4hofs = (PPU.bg4hofs >> 8) | ((uint16_t)data << 8);
         return;
      case 0x2114:
         PPU.bg4vofs = (PPU.bg4vofs >> 8) | ((uint16_t)data << 8);
         return;

      case 0x212c:
         PPU.tm = data;
         return;


      /////////////// VRAM
      case 0x2115: // VMAIN
         STATUS.regs.vmain = data;
         return;

      case 0x2116: // VMADDL
         STATUS.regs.vram_addr.b.l = data;
         return;

      case 0x2117: // VMADDH
         STATUS.regs.vram_addr.b.h = data;
         return;

      // Missing address remapping.
      // Block VRAM access unless we're in force blank or vblank.
      case 0x2118: // VMDATAL
         fprintf(stderr, "Write VRAM word $%x -> $%x (low)\n", (unsigned)data, (unsigned)STATUS.regs.vram_addr.w);
         iup_if(MEM.vram[(STATUS.regs.vram_addr.w & 0x7fff) << 1], PPU.vsync | (PPU.inidisp & 0x80), data);
         STATUS.regs.vram_addr.w += isel_if(STATUS.regs.vmain & 0x80, 0, vram_addr_inc[STATUS.regs.vmain & 3]);
         return;

      case 0x2119: // VMDATAH
         fprintf(stderr, "Write VRAM word $%x -> $%x (hi)\n", (unsigned)data, (unsigned)STATUS.regs.vram_addr.w);
         iup_if(MEM.vram[((STATUS.regs.vram_addr.w & 0x7fff) << 1) + 1], PPU.vsync | (PPU.inidisp & 0x80), data);
         STATUS.regs.vram_addr.w += isel_if(STATUS.regs.vmain & 0x80, vram_addr_inc[STATUS.regs.vmain & 3], 0);
         return;
      //////////////


      /////////////// CGRAM
      case 0x2121: // CGADD
         STATUS.regs.cgadd = data;
         return;

      case 0x2122: // CGDATA
         STATUS.regs.cg_odd ^= true;
         if (!STATUS.regs.cg_odd)
         {
            MEM.cgram[STATUS.regs.cgadd << 1] = STATUS.regs.cgbuf;
            MEM.cgram[((STATUS.regs.cgadd++) << 1) + 1] = data;
         }
         else
         {
            STATUS.regs.cgbuf = data;
         }
         return;

      /////////////////////

      // WRAM
      case 0x2180: // WMDATA
         MEM.wram[STATUS.regs.wram_addr.l++] = data;
         STATUS.regs.wram_addr.l &= 0x1ffff;
         return;

      case 0x2181: // WMADDL
         STATUS.regs.wram_addr.b.ll = data;
         return;

      case 0x2182: // WMADDM
         STATUS.regs.wram_addr.b.lh = data;
         return;

      case 0x2183: // WMADDH
         STATUS.regs.wram_addr.b.hl = data;
         return;
         
      
      default:
         break;
   }

   // APUIO
   if (daddr >= 0x2140 && daddr < 0x2180)
      STATUS.regs.apuio[(daddr - 0x2140) & 3] = data;

}

void ssnes_bus_write_4000(uint32_t addr, uint8_t data)
{
   CPU.status.cycles += 8;
   uint16_t baddr = addr & 0xffff;

   switch (baddr)
   {
      case 0x4200: // NMITIMEN
         STATUS.regs.nmitimen = data;
         return;

      case 0x420b: // MDMAEN
         STATUS.dma_enable = data;
         for (unsigned i = 0; i < 8; i++)
            STATUS.dma_channels[i].trans_cnt = 0;
         return;

      default:
         break;
   }

   if ((baddr >> 8) == 0x43) // DMA
   {
      unsigned channel = (baddr >> 4) & 7;
      unsigned reg = baddr & 0xf;

      switch (reg)
      {
         case 0: // CTRL
            STATUS.dma_channels[channel].ctrl = data;
            return;

         case 1: // B-Bus addr
            STATUS.dma_channels[channel].dest = data;
            return;

         case 2: // A1TxL
            STATUS.dma_channels[channel].src.b.ll = data;
            return;

         case 3:
            STATUS.dma_channels[channel].src.b.lh = data;
            return;

         case 4:
            STATUS.dma_channels[channel].src.b.hl = data;
            return;

         case 5:
            STATUS.dma_channels[channel].size.b.l = data;
            return;

         case 6:
            STATUS.dma_channels[channel].size.b.h = data;
            return;

         default:
            break;
      }
   }
}

void ssnes_bus_write_6000(uint32_t addr, uint8_t data)
{
   CPU.status.cycles += 8;
}
