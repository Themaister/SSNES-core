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
      case 0x4128: // JOY1L
         return STATUS.input[0].data1.b.l;
      case 0x4129: // JOY1H
         return STATUS.input[0].data1.b.h;
      case 0x421a: // JOY2L
         return STATUS.input[1].data1.b.l;
      case 0x421b: // JOY2H
         return STATUS.input[1].data1.b.h;
      case 0x412c: // JOY3L
         return STATUS.input[0].data2.b.l;
      case 0x412d: // JOY3H
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
      case 0x2100: // INIDISP
         PPU.inidisp = data;
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
      case 0x2118: // VMDATAL
         MEM.vram[(STATUS.regs.vram_addr.w & 0x7fff) << 1] = data;
         if (~(STATUS.regs.vmain & 0x80)) STATUS.regs.vram_addr.w += vram_addr_inc[STATUS.regs.vmain & 3];
         return;

      case 0x2119: // VMDATAH
         MEM.vram[((STATUS.regs.vram_addr.w & 0x7fff) << 1) + 1] = data;
         if (STATUS.regs.vmain & 0x80) STATUS.regs.vram_addr.w += vram_addr_inc[STATUS.regs.vmain & 3];
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
