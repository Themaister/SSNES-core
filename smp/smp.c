#include "smp.h"
#include <string.h>
#include <stdint.h>
#include "system/state.h"
#include "system/macros.h"
#include "iplrom.h"

#include "table.h"
#include "memory.h"

static void init_smp_registers(void)
{
   SMP.pc = 0xffc0; // Reset vector
   SMP.x = 0;
   SMP.sp = 0xef;
   SMP.ya.w = 0;
   memset(&SMP.p, 0, sizeof(SMP.p));
   SMP.p.z = 1;
}

static void init_ipl_rom(void)
{
   memcpy(&MEM.apuram[0xffc0], smp_iplrom, sizeof(smp_iplrom));
}

void ssnes_smp_init(void)
{
   init_smp_registers();
   init_ipl_rom();
}

void ssnes_smp_deinit(void)
{}

void ssnes_smp_run(unsigned cycles)
{
   unsigned ran_cycles = 0;
   while (ran_cycles < cycles)
   {
      uint8_t opcode = smp_read_pc();
      ssnes_smp_optable[opcode]();
      ran_cycles += ssnes_smp_cycle_table[opcode];
   }
}
