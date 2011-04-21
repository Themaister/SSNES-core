#include "table.h"
#include "op.h"
#include <stddef.h>

#define ALUOP(op, alu) CPU_OP_READ_##op##_DECL(alu)
#define RMWOP(mode, op) CPU_OP_RMW_##mode##_DECL(op)
#define NOP cpu_op_nop

const cpu_op_t op_table_EMUL[256];

const cpu_op_t op_table_MX[256] = {
   CPU_OP_INTERRUPT_N_DECL(brk),    // 00 
   ALUOP(IDPX_B, ora),              // 01
   NOP,                             // 02 ?!?!
   ALUOP(SR_B, ora),                // 03
   RMWOP(DP_B, tsb),                // 04
   ALUOP(DP_B, ora),                // 05
   RMWOP(DP_B, asl),                // 06
   ALUOP(ILDP_B, ora),              // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_B, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_B, tsb),              // 0c
   ALUOP(ADDR_B, ora),              // 0d
   RMWOP(ADDR_B, asl),              // 0e
   ALUOP(LONG_B, ora),              // 0f
   CPU_OP_BRANCH_REG_N_DECL(n),     // 10 ?!?!
   ALUOP(IDPY_B, ora),              // 11
   ALUOP(IDP_B, ora),               // 12
   ALUOP(ISRY_B, ora),              // 13
   RMWOP(DP_B, trb),                // 14
   CPU_OP_READ_DPR_B_DECL(ora, x),  // 15
   RMWOP(DPX_B, asl),               // 16
   ALUOP(IDPY_B, ora),              // 17
   CPU_OP_RESET_FLAG_DECL(c),       // 18
   ALUOP(ADDRY_B, ora),             // 19
   CPU_OP_RMW_REG_B_DECL(a, inc),   // 1a
   cpu_op_tcs_n,                    // 1b
   RMWOP(ADDR_B, trb),              // 1c
   ALUOP(ADDRX_B, ora),             // 1d
   RMWOP(ADDRX_B, asl),             // 1e
   ALUOP(LONGX_B, ora),             // 1f
   cpu_op_jsr_addr,                 // 20
   ALUOP(IDPX_B, and),              // 21
   cpu_op_jsr_long_n,               // 22
   ALUOP(SR_B, and),                // 23
   ALUOP(DP_B, bit),                // 24
   ALUOP(DP_B, and),                // 25
   RMWOP(DP_B, rol),                // 26
   ALUOP(ILDP_B, and),              // 27
   cpu_op_plp_n,                    // 28
   ALUOP(CONST_B, and),             // 29
   CPU_OP_RMW_REG_B_DECL(a, rol),   // 2a
   cpu_op_pld,                      // 2b
   ALUOP(ADDR_B, bit),              // 2c
   ALUOP(ADDR_B, and),              // 2d
   


};

const cpu_op_t op_table_mx[256];
const cpu_op_t op_table_mX[256];
const cpu_op_t op_table_Mx[256];

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
