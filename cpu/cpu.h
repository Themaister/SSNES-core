#ifndef __SSNES_CPU_H
#define __SSNES_CPU_H

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "cpu_state.h"
#include "state.h"
#include "memory.h"
#include "table.h"

void cpu_init(void);
void cpu_run_frame(void);
void cpu_reset(void);

static inline void cpu_set_p(uint8_t data)
{
   REGS.p.n = (data & (1 << 7)) >> 7;
   REGS.p.v = (data & (1 << 6)) >> 6;
   REGS.p.m = (data & (1 << 5)) >> 5;
   REGS.p.x = (data & (1 << 4)) >> 4;
   REGS.p.d = (data & (1 << 3)) >> 3;
   REGS.p.i = (data & (1 << 2)) >> 2;
   REGS.p.z = (data & (1 << 1)) >> 1;
   REGS.p.c = (data & (1 << 0)) >> 0;

   cpu_update_table();
}

static inline uint8_t cpu_get_p(void)
{
   return
      ((uint8_t)REGS.p.n << 7) |
      ((uint8_t)REGS.p.v << 6) |
      ((uint8_t)REGS.p.m << 5) |
      ((uint8_t)REGS.p.x << 4) |
      ((uint8_t)REGS.p.d << 3) |
      ((uint8_t)REGS.p.i << 2) |
      ((uint8_t)REGS.p.z << 1) |
      ((uint8_t)REGS.p.c << 0);
}


#endif
