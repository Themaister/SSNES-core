#include "smp.h"
#include <string.h>
#include <stdint.h>
#include "system/state.h"
#include "system/macros.h"

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

   SMP.control = 0xb0;
   SMP.t0_out = 0xf;
   SMP.t1_out = 0xf;
   SMP.t2_out = 0xf;
}

void ssnes_smp_init(void)
{
   init_smp_registers();

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
   fprintf(stderr, "A: %02x X: %02x Y: %02x YA: %04x ", 
         (unsigned)SMP.ya.b.l, (unsigned)SMP.x, (unsigned)SMP.ya.b.h, (unsigned)SMP.ya.w);
   fprintf(stderr, "S: %02x ", (unsigned)SMP.sp);
   print_flags();
}

#define SMP_TICK_TIMER(timer) \
   static inline void smp_tick_timer_t##timer (void) \
   { \
      if (++SMP.t##timer##_tick == SMP.t##timer##_target) \
      { \
         SMP.t##timer##_tick = 0; \
         SMP.t##timer##_out = (SMP.t##timer##_out + 1) & 0xf; \
      } \
   }

SMP_TICK_TIMER(0)
SMP_TICK_TIMER(1)
SMP_TICK_TIMER(2)

static inline void smp_update_timers(unsigned cycles)
{
   unsigned old_8 = SMP.base_timer_8;
   unsigned old_64 = SMP.base_timer_64;

   // Stage 1 timers. These always run.
   SMP.base_timer_8 = (old_8 + cycles) & 127;
   SMP.base_timer_64 = (old_64 + cycles) & 15;

   // Contitionally tick timers.
   unsigned ctrl = SMP.control;
   if (SMP.base_timer_8 < old_8) // Check edge
   {
      if (ctrl & 0x01)
      {
         //fprintf(stderr, "Ticking timer 0\n");
         smp_tick_timer_t0();
      }
      if (ctrl & 0x02)
      {
         //fprintf(stderr, "Ticking timer 1\n");
         smp_tick_timer_t1();
      }
   }
   if (SMP.base_timer_64 < old_64)
   {
      if (ctrl & 0x04)
      {
         //fprintf(stderr, "Ticking timer 2\n");
         smp_tick_timer_t2();
      }
   }
}

unsigned ssnes_smp_run(unsigned cycles)
{
   // We try to scale master cycles from CPU down to SMP cycles (1.024MHz).
   cycles /= 10;
   unsigned ran_cycles = 0;
   while (ran_cycles < cycles)
   {
      //fprintf(stderr, "  %04x ", (unsigned)SMP.pc);
      uint8_t opcode = smp_read_pc();
      //fprintf(stderr, "%-14s", ssnes_smp_opname[opcode]);
      //print_registers();
      ssnes_smp_optable[opcode]();

      unsigned cycles = ssnes_smp_cycle_table[opcode];
      ran_cycles += cycles;
      smp_update_timers(ran_cycles);
   }

   return ran_cycles * 10;
}
