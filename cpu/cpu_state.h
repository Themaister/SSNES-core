#ifndef __CPU_STATE_H
#define __CPU_STATE_H

#include "util.h"
#include <stdbool.h>

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

   uint32_t db; // Data bank
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
};

#endif
