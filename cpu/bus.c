#include "bus.h"
#include "state.h"
#include "macros.h"

uint8_t ssnes_bus_read_2000(uint32_t addr)
{
   CPU.status.cycles += 8;
   return 0;
}

uint8_t ssnes_bus_read_4000(uint32_t addr)
{
   CPU.status.cycles += 8;
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

   uint32_t daddr = addr & 0xffff;
   switch (daddr)
   {
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

      default:
         break;
   }

   if (baddr >> 8 == 43) // DMA
   {
      unsigned channel = (baddr >> 4) & 7;
      unsigned reg = baddr & 7;

      STATUS.dma_channels[channel].regs[reg] = data;
   }
}

void ssnes_bus_write_6000(uint32_t addr, uint8_t data)
{
   CPU.status.cycles += 8;
}
