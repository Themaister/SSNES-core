#ifndef __SMP_H
#define __SMP_H

#include <stdint.h>
#include <stdbool.h>
#include "system/util.h"

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

   // $fa - $fc
   uint8_t t0_target;
   uint8_t t1_target;
   uint8_t t2_target;

   uint8_t t0_out;
   uint8_t t1_out;
   uint8_t t2_out;
};

unsigned ssnes_smp_run(unsigned cycles);

void ssnes_smp_init(void);
void ssnes_smp_deinit(void);

#endif
