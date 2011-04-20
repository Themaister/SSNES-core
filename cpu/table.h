#ifndef __TABLE_H
#define __TABLE_H

#include <stdint.h>
#include "state.h"

typedef void (*cpu_op_t)(void);
extern const cpu_op_t *op_table;
extern const uint8_t *cycle_table;

extern cpu_op_t op_table_EMUL[256];
extern cpu_op_t op_table_mx[256];
extern cpu_op_t op_table_mX[256];
extern cpu_op_t op_table_Mx[256];
extern cpu_op_t op_table_MX[256];
extern const cpu_op_t *op_table_index[];

extern uint8_t cycle_table_EMUL[256];
extern uint8_t cycle_table_mx[256];
extern uint8_t cycle_table_mX[256];
extern uint8_t cycle_table_Mx[256];
extern uint8_t cycle_table_MX[256];
extern const uint8_t* cycle_table_index[];

static inline void cpu_update_table(void)
{
   unsigned reg_val = (REGS.e << 2) | (REGS.p.m << 1) | REGS.p.x;
   op_table = op_table_index[reg_val];
   cycle_table = cycle_table_index[reg_val];
}

void cpu_init_tables(void);

#endif
