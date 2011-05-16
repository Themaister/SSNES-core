#include "smp.h"
#include "dsp/dsp.h"
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
   dputc(SMP.p.n ? 'N' : 'n');
   dputc(SMP.p.v ? 'V' : 'v');
   dputc(SMP.p.p ? 'P' : 'p');
   dputc(SMP.p.b ? 'B' : 'b');
   dputc(SMP.p.h ? 'H' : 'h');
   dputc(SMP.p.i ? 'I' : 'i');
   dputc(SMP.p.z ? 'Z' : 'z');
   dputc(SMP.p.c ? 'C' : 'c');
   dprintf("\n");
}

static inline void print_registers(void)
{
   dprintf("A: %02x X: %02x Y: %02x YA: %04x ", 
         (unsigned)SMP.ya.b.l, (unsigned)SMP.x, (unsigned)SMP.ya.b.h, (unsigned)SMP.ya.w);
   dprintf("S: %02x ", (unsigned)SMP.sp);
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
         //dprintf(stderr, "Ticking timer 0\n");
         smp_tick_timer_t0();
      }
      if (ctrl & 0x02)
      {
         //dprintf(stderr, "Ticking timer 1\n");
         smp_tick_timer_t1();
      }
   }
   if (SMP.base_timer_64 < old_64)
   {
      if (ctrl & 0x04)
      {
         //dprintf(stderr, "Ticking timer 2\n");
         smp_tick_timer_t2();
      }
   }
}

static inline void check_dsp(unsigned cycles)
{
   SMP.dsp_cycles += cycles;

   if (SMP.dsp_write)
   {
      ssnes_dsp_write(SMP.dsp_addr, SMP.dsp_data);
      ssnes_dsp_run(SMP.dsp_cycles);
      SMP.dsp_write = false;
      SMP.dsp_cycles = 0;
   }
   else if (SMP.dsp_cycles > 1000) // 1 ms maximal difference on reads.
   {
      ssnes_dsp_run(SMP.dsp_cycles);
      SMP.dsp_cycles = 0;
   }
}

unsigned ssnes_smp_run(unsigned cycles)
{
   // We try to scale master cycles from CPU down to SMP cycles (1.024MHz).
   cycles /= 21;
   cycles++;
   unsigned ran_cycles = 0;
   while (ran_cycles < cycles)
   {
      dprintf("  %04x ", (unsigned)SMP.pc);
      uint8_t opcode = smp_read_pc();
      dprintf("%-14s", ssnes_smp_opname[opcode]);
      print_registers();
      ssnes_smp_optable[opcode]();

      unsigned cycles = ssnes_smp_cycle_table[opcode];
      ran_cycles += cycles;
      smp_update_timers(cycles);
      check_dsp(cycles);
   }

   return ran_cycles * 21;
}
