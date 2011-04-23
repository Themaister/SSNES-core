#ifndef __TABLE_H
#define __TABLE_H

#include <stdint.h>
#include "state.h"
#include "macros.h"
#include <stdio.h>

typedef void (*cpu_op_t)(void);
extern const cpu_op_t *ssnes_cpu_op_table;
extern const uint8_t *ssnes_cpu_cycle_table;

extern const cpu_op_t ssnes_cpu_op_table_EMUL[256];
extern const cpu_op_t ssnes_cpu_op_table_mx[256];
extern const cpu_op_t ssnes_cpu_op_table_mX[256];
extern const cpu_op_t ssnes_cpu_op_table_Mx[256];
extern const cpu_op_t ssnes_cpu_op_table_MX[256];
extern const cpu_op_t *ssnes_cpu_op_table_index[];

extern const uint8_t ssnes_cpu_cycle_table_EMUL[256];
extern const uint8_t ssnes_cpu_cycle_table_mx[256];
extern const uint8_t ssnes_cpu_cycle_table_mX[256];
extern const uint8_t ssnes_cpu_cycle_table_Mx[256];
extern const uint8_t ssnes_cpu_cycle_table_MX[256];
extern const uint8_t* ssnes_cpu_cycle_table_index[];

extern const char* ssnes_cpu_opcode_names[];

static inline void cpu_update_table(void)
{
   unsigned reg_val = (REGS.e << 2) | (REGS.p.m << 1) | REGS.p.x;
   ssnes_cpu_op_table = ssnes_cpu_op_table_index[reg_val];
   ssnes_cpu_cycle_table = ssnes_cpu_cycle_table_index[reg_val];
   fprintf(stderr, "Opcode table updated! Value: %u\n", reg_val);
}

#endif
