#ifndef __CPU_H
#define __CPU_H

#include <stdbool.h>
#include <stdint.h>
#include "memory.h"
#include "table.h"

void cpu_init(void);
void cpu_run_frame(void);
void cpu_reset(void);

static inline cpu_set_p(uint8_t data)
{
   REGS.p.z = data & (1 << 7);
   REGS.p.v = data & (1 << 6);
   REGS.p.m = data & (1 << 5);
   REGS.p.x = data & (1 << 4);
   REGS.p.d = data & (1 << 3);
   REGS.p.i = data & (1 << 2);
   REGS.p.z = data & (1 << 1);
   REGS.p.c = data & (1 << 0);

   cpu_update_table();
}

static inline uint8_t cpu_get_p(void)
{
   return
      (REGS.p.z << 7) ||
      (REGS.p.v << 6) ||
      (REGS.p.m << 5) ||
      (REGS.p.x << 4) ||
      (REGS.p.d << 3) ||
      (REGS.p.i << 2) ||
      (REGS.p.z << 1) ||
      (REGS.p.c << 0);
}


struct cpu_regs
{
   // GPP and index regs
   word_reg_t a, x, y;

   // Stack pointer and PC
   word_reg_t sp;
   long_reg_t pc; // Includes the PBK

   // Processor flags
   struct
   {
      bool n, v, m, x, d, i, z, c;
   } p;
   bool e;

   uint32_t mbk; // Data bank
   uint32_t dp; // Direct page
};

struct cpu_status
{
};

struct cpu_alu_state
{
   unsigned mpyctr;
   unsigned divctr;
   unsigned shift;
};

struct cpu_state
{
   struct cpu_regs regs;
   struct cpu_status status;
   struct cpu_alu_state alu;
};

#endif
