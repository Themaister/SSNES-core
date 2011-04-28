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
};

void ssnes_smp_run(unsigned cycles);

void ssnes_smp_init(void);
void ssnes_smp_deinit(void);

#endif
