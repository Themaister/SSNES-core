#ifndef __SMP_REGS_H
#define __SMP_REGS_H

#include "system/state.h"
#include "system/util.h"
#include "system/macros.h"

static inline uint8_t smp_get_p(void)
{
   return
      ((uint8_t)SMP.p.n << 7) |
      ((uint8_t)SMP.p.v << 6) |
      ((uint8_t)SMP.p.p << 5) |
      ((uint8_t)SMP.p.b << 4) |
      ((uint8_t)SMP.p.h << 3) |
      ((uint8_t)SMP.p.i << 2) |
      ((uint8_t)SMP.p.z << 1) |
      ((uint8_t)SMP.p.c << 0);
}

static inline void smp_set_p(uint8_t p)
{
   SMP.p.n = p & 0x80;
   SMP.p.v = p & 0x40;
   SMP.p.p = p & 0x20;
   SMP.p.b = p & 0x10;
   SMP.p.h = p & 0x08;
   SMP.p.i = p & 0x04;
   SMP.p.z = p & 0x02;
   SMP.p.c = p & 0x01;
}

#endif
