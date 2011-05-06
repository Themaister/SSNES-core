#include "memmap.h"
#include "system/state.h"
#include "system/macros.h"
#include "iplrom.h"

#include <stdio.h>

smp_write_t ssnes_smp_memmap_write[1 << 12];
smp_read_t ssnes_smp_memmap_read[1 << 12];

static const uint8_t *smp_hiram_data;


static void smp_write_ram(uint16_t addr, uint8_t data, uint8_t mask)
{
   //dprintf(stderr, "Write RAM: $%x => $%x\n", (unsigned)addr, (unsigned)data);
   uint8_t orig = MEM.apuram[addr];
   uint8_t delta = orig ^ data;
   delta &= mask;
   MEM.apuram[addr] = orig ^ delta;
}

static uint8_t smp_read_ram(uint16_t addr)
{
   return MEM.apuram[addr];
}

static uint8_t smp_read_hiram(uint16_t addr)
{
   return smp_hiram_data[addr - 0xffc0];
}

static inline void smp_write_control(uint8_t data, uint8_t mask)
{
   SMP.control ^= (SMP.control ^ data) & mask;

   if (data & mask & 0x10)
   {
      STATUS.regs.apuio[0] = 0;
      STATUS.regs.apuio[1] = 0;
   }
   if (data & mask & 0x20)
   {
      STATUS.regs.apuio[2] = 0;
      STATUS.regs.apuio[3] = 0;
   }

   smp_hiram_data = (SMP.control & 0x80) ? smp_iplrom : &MEM.apuram[0xffc0];
}

static void smp_write_regs(uint16_t addr, uint8_t data, uint8_t mask)
{
   addr &= 0xff;
   switch (addr)
   {
      case 0xf0: // Testing stuff, we don't care.
         dprintf("SMP $f0 is being touched inappropriately with $%02x (mask $%02x), call da police!\n", (unsigned)data, (unsigned)mask);
         return;

      case 0xf1: // Timer control, TODO.
         smp_write_control(data, mask);
         SMP.t0_out = 0;
         SMP.t1_out = 0;
         SMP.t2_out = 0;
         return;

      // DSP communication. Dummy for now.
      case 0xf2:
         SMP.dsp_addr ^= (SMP.dsp_addr ^ data) & mask;
         return;

      case 0xf3:
         SMP.dsp_data ^= (SMP.dsp_data ^ data) & mask;
         return;

      case 0xf4:
         SMP.apuio[0] ^= (SMP.apuio[0] ^ data) & mask;
         //dprintf(stderr, "SMP: APUIO0 = $%x\n", (unsigned)SMP.apuio[0]);
         return;
      case 0xf5:
         SMP.apuio[1] ^= (SMP.apuio[1] ^ data) & mask;
         //dprintf(stderr, "SMP: APUIO1 = $%x\n", (unsigned)SMP.apuio[1]);
         return;
      case 0xf6:
         SMP.apuio[2] ^= (SMP.apuio[2] ^ data) & mask;
         //dprintf(stderr, "SMP: APUIO2 = $%x\n", (unsigned)SMP.apuio[2]);
         return;
      case 0xf7:
         SMP.apuio[3] ^= (SMP.apuio[3] ^ data) & mask;
         //dprintf(stderr, "SMP: APUIO3 = $%x\n", (unsigned)SMP.apuio[3]);
         return;

      // Normal ram.
      case 0xf8:
      case 0xf9:
         MEM.apuram[addr] ^= (MEM.apuram[addr] ^ data) & mask;
         return;

      // Timing stuff.
      case 0xfa:
         SMP.t0_target ^= (SMP.t0_target ^ data) & mask;
         //dprintf(stderr, "Timer 0xfa target = $%02x\n", (unsigned)SMP.t0_target);
         return;
      case 0xfb:
         SMP.t1_target ^= (SMP.t1_target ^ data) & mask;
         //dprintf(stderr, "Timer 0xfb target = $%02x\n", (unsigned)SMP.t1_target);
         return;
      case 0xfc:
         SMP.t2_target ^= (SMP.t2_target ^ data) & mask;
         //dprintf(stderr, "Timer 0xfc target = $%02x\n", (unsigned)SMP.t2_target);
         return;
   }
}

static uint8_t smp_read_regs(uint16_t addr)
{
   uint8_t ret;
   switch (addr)
   {
      case 0xf4:
         return STATUS.regs.apuio[0];
      case 0xf5:
         return STATUS.regs.apuio[1];
      case 0xf6:
         return STATUS.regs.apuio[2];
      case 0xf7:
         return STATUS.regs.apuio[3];

      case 0xf8:
      case 0xf9:
         return MEM.apuram[addr];

      // Timing stuff
      case 0xfd:
         ret = SMP.t0_out;
         //dprintf(stderr, "Returned timer0 = %u\n", (unsigned)ret);
         SMP.t0_out = 0;
         return ret;
      case 0xfe:
         ret = SMP.t1_out;
         //dprintf(stderr, "Returned timer1 = %u\n", (unsigned)ret);
         SMP.t1_out = 0;
         return ret;
      case 0xff:
         ret = SMP.t2_out;
         //dprintf(stderr, "Returned timer2 = %u\n", (unsigned)ret);
         SMP.t2_out = 0;
         return ret;
   }

   return 0;
}

void ssnes_smp_init_memmap(void)
{
   for (unsigned i = 0; i < 1 << 12; i++)
   {
      ssnes_smp_memmap_write[i] = smp_write_ram;
      ssnes_smp_memmap_read[i] = smp_read_ram;
   }

   ssnes_smp_memmap_write[0xf0 >> 4] = smp_write_regs;
   ssnes_smp_memmap_read[0xf0 >> 4] = smp_read_regs;
   smp_hiram_data = smp_iplrom;

   // Reads 0xffc0 - 0xffff might be mapped to either IPLROM or regular APURAM.
   for (unsigned i = 0; i < 4; i++)
      ssnes_smp_memmap_read[(0xffc0 >> 4) + i] = smp_read_hiram;
}
