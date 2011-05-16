#ifndef __SMP_H
#define __SMP_H

#include <stdint.h>
#include <stdbool.h>
#include "system/util.h"

#define SMP_FREQUENCY (1024000)

struct smp_state
{
   uint16_t pc;
   uint8_t x;
   word_reg_t ya;
   uint16_t sp;
   struct
   {
      bool n, v, p, b, h, i, z, c;
   } p;

   
   // SMP registers
   // We don't care about register $f0
   uint8_t control; // $f1
   uint8_t dsp_addr; // $f2
   uint8_t dsp_data; // $f3

   uint8_t apuio[4]; // $f4-$f7 S-CPU <-> S-SMP communication. These are write only, we read from CPU-side bus.

   unsigned base_timer_8;
   unsigned base_timer_64;

   uint8_t t0_tick;
   uint8_t t1_tick;
   uint8_t t2_tick;

   // $fa - $fc
   uint8_t t0_target;
   uint8_t t1_target;
   uint8_t t2_target;

   uint8_t t0_out;
   uint8_t t1_out;
   uint8_t t2_out;

   unsigned dsp_cycles;
   bool dsp_write;
   bool dsp_read; // Current unused as we don't sync up for DSP reads.
};

unsigned ssnes_smp_run(unsigned cycles);

void ssnes_smp_init(void);
void ssnes_smp_deinit(void);

#endif
