#include "table.h"
#include "op.h"
#include <stddef.h>

#define ALUOP(op, alu) CPU_OP_READ_##op##_DECL(alu)
#define RMWOP(mode, op) CPU_OP_RMW_##mode##_DECL(op)
#define NOP cpu_op_nop

const cpu_op_t op_table_EMUL[256] = {
   CPU_OP_INTERRUPT_E_DECL(brk),    // 00 
   ALUOP(IDPX_B, ora),              // 01
   NOP,                             // 02
   NOP,                             // 03
   NOP,                             // 04
   ALUOP(DP_B, ora),                // 05
   RMWOP(DP_B, asl),                // 06
   NOP,                             // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_B, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_B, tsb),              // 0c
   ALUOP(ADDR_B, ora),              // 0d
   RMWOP(ADDR_B, asl),              // 0e
   NOP,                             // 0f



};

const cpu_op_t op_table_mx[256];
const cpu_op_t op_table_mX[256];
const cpu_op_t op_table_Mx[256];
const cpu_op_t op_table_MX[256];

const uint8_t cycle_table_EMUL[256];
const uint8_t cycle_table_mx[256];
const uint8_t cycle_table_mX[256];
const uint8_t cycle_table_Mx[256];
const uint8_t cycle_table_MX[256];

const cpu_op_t *op_table = op_table_EMUL;
const uint8_t *cycle_table = cycle_table_EMUL;

const cpu_op_t* op_table_index[] = {
   op_table_mx,
   op_table_mX,
   op_table_Mx,
   op_table_MX,
   op_table_EMUL,
   op_table_EMUL,
   op_table_EMUL,
   op_table_EMUL
};

const uint8_t* cycle_table_index[] = {
   cycle_table_mx,
   cycle_table_mX,
   cycle_table_Mx,
   cycle_table_MX,
   cycle_table_EMUL,
   cycle_table_EMUL,
   cycle_table_EMUL,
   cycle_table_EMUL
};

void cpu_init_tables(void)
{
   CPU_OP_RMW_REG_B_DECL(a, inc) ();


}
