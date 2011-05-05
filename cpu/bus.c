#include "bus.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdio.h>

static inline uint16_t vram_translation(uint16_t addr)
{
   unsigned trans = (STATUS.regs.vmain & 0x0c) >> 2;
   uint16_t tmp;
   switch (trans)
   {
      // No remapping.
      case 0:
         return addr;

      // aaaaaaaaBBBccccc => aaaaaaaacccccBBB
      case 1:
         tmp = addr & 0xff;
         tmp = (tmp << 3) | (tmp >> 5);
         return (addr & 0xff00) | tmp;

      // aaaaaaaBBBcccccc => aaaaaaaccccccBBB
      case 2:
         tmp = addr & 0x1ff;
         tmp = (tmp << 3) | (tmp >> 6);
         return (addr & 0xfe00) | tmp;

      // aaaaaaBBBccccccc => aaaaaacccccccBBB
      case 3:
         tmp = addr & 0x3ff;
         tmp = (tmp << 3) | (tmp >> 7);
         return (addr & 0xfa00) | tmp;
   }

   return 0;
}

uint8_t ssnes_bus_read_2000(uint32_t addr)
{
   CPU.status.cycles += 8;

   uint16_t daddr = addr & 0xffff;
   uint8_t res;
   static const unsigned vram_addr_inc[] = {1, 32, 128, 128};

   switch (daddr)
   {
      // OAMDATAREAD
      case 0x2138:
         STATUS.regs.oam_odd ^= true;
         if (!STATUS.regs.cg_odd)
         {
            res = MEM.oam.b[((uint16_t)STATUS.regs.oam_addr.w++ << 1) + 1];
            STATUS.regs.oam_addr.w &= 0x3ff;
         }
         else
            res = MEM.oam.b[(uint16_t)STATUS.regs.oam_addr.w << 1];
         return res;

      // VRAM reads have a buffering mechanism.
      // VMDATALREAD
      case 0x2139:
         res = STATUS.regs.vram_rd_buf.b.l;
         iup_if(STATUS.regs.vram_rd_buf.w, ~STATUS.regs.vmain & 0x80, READ_VRAMW(vram_translation(STATUS.regs.vram_addr.w)));
         STATUS.regs.vram_addr.w += isel_if(STATUS.regs.vmain & 0x80, 0, vram_addr_inc[STATUS.regs.vmain & 3]);
         return res;

      // VMDATAHREAD
      case 0x213a:
         res = STATUS.regs.vram_rd_buf.b.h;
         iup_if(STATUS.regs.vram_rd_buf.w, STATUS.regs.vmain & 0x80, READ_VRAMW(vram_translation(STATUS.regs.vram_addr.w)));
         STATUS.regs.vram_addr.w += isel_if(STATUS.regs.vmain & 0x80, vram_addr_inc[STATUS.regs.vmain & 3], 0);
         return res;


      // CGDATAREAD
      case 0x213b:
         STATUS.regs.cg_odd ^= true;
         if (!STATUS.regs.cg_odd)
            res = MEM.cgram.b[((uint16_t)STATUS.regs.cgadd++ << 1) + 1];
         else
            res = MEM.cgram.b[(uint16_t)STATUS.regs.cgadd << 1];
         return res;

      // WMDATA
      case 0x2180:
         res = MEM.wram[STATUS.regs.wram_addr.l++];
         STATUS.regs.wram_addr.l &= 0x1ffff;
         return res;
   }

   if (daddr >= 0x2140 && daddr < 0x2180)
   {
      STATUS.smp_state = true;
      uint8_t ret = SMP.apuio[daddr & 3];
      //fprintf(stderr, "CPU: APUIO%d = $%x\n", (int)daddr & 3, (unsigned)ret);
      return ret;
   }

   return 0;
}

uint8_t ssnes_bus_read_4000(uint32_t addr)
{
   CPU.status.cycles += 8;

   uint16_t daddr = addr & 0xffff;
   switch (daddr)
   {
      case 0x4210: // RDNMI
         return ((uint8_t)PPU.vsync << 7) | 2;

      case 0x4212: // HVBJOY
         return PPU.hvbjoy;

      case 0x4214: // RDDIVL
         return CPU.alu.div_quot.b.l;
      case 0x4215: // RDDIVH
         return CPU.alu.div_quot.b.h;
      case 0x4216: // RDMPYL
         return CPU.alu.mul_rem_res.b.l;
      case 0x4217: // RDMPYH
         return CPU.alu.mul_rem_res.b.h;

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

   // Read DMA registers.
   if ((daddr >> 8) == 0x43) // DMA
   {
      unsigned channel = (daddr >> 4) & 7;
      unsigned reg = daddr & 0xf;

      switch (reg)
      {
         case 0: // CTRL
            return STATUS.dma_channels[channel].ctrl;

         case 1: // B-Bus addr
            return STATUS.dma_channels[channel].dest;

         case 2: // A1TxL
            return STATUS.dma_channels[channel].src.b.ll;

         case 3:
            return STATUS.dma_channels[channel].src.b.lh;

         case 4:
            return STATUS.dma_channels[channel].src.b.hl;

         case 5:
            return STATUS.dma_channels[channel].size.b.l;

         case 6:
            return STATUS.dma_channels[channel].size.b.h;

         default:
            break;
      }
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

      case 0x2101: // OBSEL
         PPU.obsel = data;
         return;

      // "OAM reset" isn't in yet. But the buffering stuff should work.
      case 0x2102: // OAMADDL
         STATUS.regs.oam_addr_buf.b.l = data;
         STATUS.regs.oam_addr = STATUS.regs.oam_addr_buf;
         STATUS.regs.oam_odd = false;
         return;
      case 0x2103: // OAMADDH
         STATUS.regs.oam_addr_buf.b.h = data;
         STATUS.regs.oam_addr = STATUS.regs.oam_addr_buf;
         STATUS.regs.oam_odd = false;
         return;

      case 0x2104: // OAMDATA
         STATUS.regs.oam_odd ^= true;
         if (!STATUS.regs.oam_odd)
         {
            uint16_t oam_data = STATUS.regs.oam_buf | ((uint16_t)data << 8);
            //fprintf(stderr, "\tWriting OAM $%04x => $%x (word)\n", (unsigned)oam_data, (unsigned)STATUS.regs.oam_addr.w);
            WRITE_OAMW(STATUS.regs.oam_addr.w++ & 0x1ff, oam_data);
         }
         else
         {
            //fprintf(stderr, "\tWriting OAM buf <= $%02x\n", (unsigned)data);
            STATUS.regs.oam_buf = data;
            // Writes to high table takes place immediately.
            iup_if(MEM.oam.b[(uint16_t)STATUS.regs.oam_addr.w << 1], STATUS.regs.oam_addr.w & 0x100, data);
         }
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

      // Block VRAM access unless we're in force blank or vblank.
      case 0x2118: // VMDATAL
         //fprintf(stderr, "Write VRAM word $%x -> $%x (low)\n", (unsigned)data, (unsigned)STATUS.regs.vram_addr.w);
         iup_if(MEM.vram.b[(vram_translation(STATUS.regs.vram_addr.w) & 0x7fff) << 1], PPU.vsync | (PPU.inidisp & 0x80), data);
         STATUS.regs.vram_addr.w += isel_if(STATUS.regs.vmain & 0x80, 0, vram_addr_inc[STATUS.regs.vmain & 3]);
         return;

      case 0x2119: // VMDATAH
         //fprintf(stderr, "Write VRAM word $%x -> $%x (hi)\n", (unsigned)data, (unsigned)STATUS.regs.vram_addr.w);
         iup_if(MEM.vram.b[((vram_translation(STATUS.regs.vram_addr.w) & 0x7fff) << 1) + 1], PPU.vsync | (PPU.inidisp & 0x80), data);
         STATUS.regs.vram_addr.w += isel_if(STATUS.regs.vmain & 0x80, vram_addr_inc[STATUS.regs.vmain & 3], 0);
         return;
      //////////////


      /////////////// CGRAM
      case 0x2121: // CGADD
         STATUS.regs.cgadd = data;
         STATUS.regs.cg_odd = false;
         return;

      case 0x2122: // CGDATA
         STATUS.regs.cg_odd ^= true;
         if (!STATUS.regs.cg_odd)
         {
            uint16_t res = STATUS.regs.cgbuf | ((uint16_t)data << 8);
            //fprintf(stderr, "Writing $%04x to CGRAM $%02x.\n", (unsigned)res, (unsigned)STATUS.regs.cgadd);
            WRITE_CGRAMW(STATUS.regs.cgadd, res);
            STATUS.regs.cgadd++;
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
         STATUS.regs.wram_addr.b.hl = data & 0x1;
         return;
         
      
      default:
         break;
   }

   // APUIO
   if (daddr >= 0x2140 && daddr < 0x2180)
   {
      STATUS.smp_state = true;
      STATUS.regs.apuio[daddr & 3] = data;
   }
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

      // Normally, there is a CPU 16-cycle delay before the proper values can be read from ALU registers, but for speed we don't bother emulating this.
      // WRMPYA
      case 0x4202:
         CPU.alu.wrmpya = data;
         return;

      // WRMPYB
      case 0x4203:
         CPU.alu.wrmpyb = data;
         CPU.alu.mul_rem_res.w = (uint16_t)CPU.alu.wrmpya * (uint16_t)CPU.alu.wrmpyb;
         return;

      // WRDIVL
      case 0x4204:
         CPU.alu.wrdiv.b.l = data;
         return;

      case 0x4205:
         CPU.alu.wrdiv.b.h = data;
         return;

      case 0x4206:
         CPU.alu.wrdivb = data;
         if (CPU.alu.wrdivb == 0) // Handle special case.
         {
            CPU.alu.div_quot.w = 0xffff;
            CPU.alu.mul_rem_res.w = CPU.alu.wrdiv.w;
         }
         else
         {
            CPU.alu.div_quot.w = CPU.alu.wrdiv.w / CPU.alu.wrdivb;
            CPU.alu.mul_rem_res.w = CPU.alu.wrdiv.w % CPU.alu.wrdivb;
         }
         return;

      // H/V IRQ
      case 0x4207: // HTIMEL
         STATUS.irq.htime.b.l = data;
         STATUS.irq.vhtrig = STATUS.irq.vtrig + (STATUS.irq.htime.w << 2);
         STATUS.irq.htrig = STATUS.irq.htime.w << 2;
         STATUS.pending_irq.irq_fired = false;
         return;
      case 0x4208: // HTIMEH
         STATUS.irq.htime.b.h = data;
         STATUS.irq.vhtrig = STATUS.irq.vtrig + (STATUS.irq.htime.w << 2);
         STATUS.irq.htrig = STATUS.irq.htime.w << 2;
         STATUS.pending_irq.irq_fired = false;
         return;
      case 0x4209: // VTIMEL
         STATUS.irq.vtime.b.l = data;
         STATUS.irq.vtrig = STATUS.irq.vtime.w * 1364;
         STATUS.irq.vhtrig = STATUS.irq.vtrig + (STATUS.irq.htime.w << 2);
         STATUS.pending_irq.irq_fired = false;
         return;
      case 0x420a: // VTIMEH
         STATUS.irq.vtime.b.h = data;
         STATUS.irq.vtrig = STATUS.irq.vtime.w * 1364;
         STATUS.irq.vhtrig = STATUS.irq.vtrig + (STATUS.irq.htime.w << 2);
         STATUS.pending_irq.irq_fired = false;
         return;

      case 0x420b: // MDMAEN
         STATUS.dma_enable = data;
         for (unsigned i = 0; i < 8; i++)
            STATUS.dma_channels[i].trans_cnt = 0;
         return;

      // MEMSEL
      case 0x420d:
         STATUS.regs.memsel = data & 1;
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
