#include "smp.h"
#include <string.h>
#include <stdint.h>
#include "system/state.h"
#include "system/macros.h"
#include "iplrom.h"

#include "table.h"
#include "memory.h"
#include "memmap.h"

#include <stdio.h>
#include <assert.h>

static void init_smp_registers(void)
{
   SMP.pc = 0xffc0; // Reset vector
   SMP.x = 0;
   SMP.sp = 0x00;
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

   ssnes_smp_init_memmap();

   assert(ssnes_smp_optable[255]);
   assert(ssnes_smp_cycle_table[255]);
   assert(ssnes_smp_opname[255]);
}

void ssnes_smp_deinit(void)
{}

static inline void print_flags(void)
{
   fputc(SMP.p.n ? 'N' : 'n', stderr);
   fputc(SMP.p.v ? 'V' : 'v', stderr);
   fputc(SMP.p.p ? 'P' : 'p', stderr);
   fputc('-', stderr);
   fputc(SMP.p.h ? 'H' : 'h', stderr);
   fputc(SMP.p.i ? 'I' : 'i', stderr);
   fputc(SMP.p.z ? 'Z' : 'z', stderr);
   fputc(SMP.p.c ? 'C' : 'c', stderr);
   fputc('\n', stderr);
}

static inline void print_registers(void)
{
   fprintf(stderr, "  A:  %02x   Y: %02x   X: %02x   YA: %04x\n", 
         (unsigned)SMP.ya.b.l, (unsigned)SMP.ya.b.h, (unsigned)SMP.x, (unsigned)SMP.ya.w);
   fprintf(stderr, "  SP: %02x   ", (unsigned)SMP.sp);
   print_flags();
}

unsigned ssnes_smp_run(unsigned cycles)
{
   unsigned ran_cycles = 0;
   while (ran_cycles < cycles)
   {
      uint8_t opcode = smp_read_pc();
      //fprintf(stderr, "==== SMP ===========================\n");
      //fprintf(stderr, "PC = $%04x, opcode = $%x (%s)\n", (unsigned)SMP.pc - 1, (unsigned)opcode, ssnes_smp_opname[opcode]);
      ssnes_smp_optable[opcode]();

      //print_registers();
      //fprintf(stderr, "====================================\n");
      ran_cycles += ssnes_smp_cycle_table[opcode];
   }
   return ran_cycles;
}
