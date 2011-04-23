#ifndef __CPU_OP_H
#define __CPU_OP_H

#include "state.h"
#include "macros.h"
#include "table.h"

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

#include "util.h"
#include "op_alu.h"
#include "op_pc.h"
#include "op_read.h"
#include "op_misc.h"
#include "op_rmw.h"
#include "op_write.h"

#endif
