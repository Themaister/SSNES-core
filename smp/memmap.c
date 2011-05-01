#include "memmap.h"
#include "system/state.h"
#include "system/macros.h"

#include <stdio.h>

smp_write_t ssnes_smp_memmap_write[1 << 12];
smp_read_t ssnes_smp_memmap_read[1 << 12];

static void smp_write_ram(uint16_t addr, uint8_t data, uint8_t mask)
{
   uint8_t orig = MEM.apuram[addr];
   uint8_t delta = orig ^ data;
   delta &= mask;
   MEM.apuram[addr] = orig ^ delta;
}

static uint8_t smp_read_ram(uint16_t addr)
{
   return MEM.apuram[addr];
}

static void smp_write_regs(uint16_t addr, uint8_t data, uint8_t mask)
{
   addr &= 0xff;
   switch (addr)
   {
      case 0xf0: // Testing stuff, we don't care.
         break;

      case 0xf1: // Timer control, TODO.
         break;

      case 0xf2:
         SMP.dsp_addr ^= (SMP.dsp_addr ^ data) & mask;
         break;

      case 0xf3:
         SMP.dsp_data ^= (SMP.dsp_data ^ data) & mask;
         break;

      case 0xf4:
         SMP.apuio[0] ^= (SMP.apuio[0] ^ data) & mask;
         //fprintf(stderr, "SMP: APUIO0 = $%x\n", (unsigned)SMP.apuio[0]);
         break;
      case 0xf5:
         SMP.apuio[1] ^= (SMP.apuio[1] ^ data) & mask;
         //fprintf(stderr, "SMP: APUIO1 = $%x\n", (unsigned)SMP.apuio[1]);
         break;
      case 0xf6:
         SMP.apuio[2] ^= (SMP.apuio[2] ^ data) & mask;
         //fprintf(stderr, "SMP: APUIO2 = $%x\n", (unsigned)SMP.apuio[2]);
         break;
      case 0xf7:
         SMP.apuio[3] ^= (SMP.apuio[3] ^ data) & mask;
         //fprintf(stderr, "SMP: APUIO3 = $%x\n", (unsigned)SMP.apuio[3]);
         break;

      case 0xf8:
      case 0xf9:
         MEM.apuram[addr] ^= (MEM.apuram[addr] ^ data) & mask;
         break;

      // Timing stuff.
      case 0xfa:
      case 0xfb:
      case 0xfc:
      case 0xfd:
      case 0xfe:
      case 0xff:
         break;
   }
}

static uint8_t smp_read_regs(uint16_t addr)
{
   addr &= 0xff;
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
}
