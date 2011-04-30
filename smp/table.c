#include "table.h"

#include "op.h"

#define NOP smp_op_nop
#define TCALL(x) SMP_OP_TCALL_DECL(x)
#define SET1(x) SMP_OP_SET_DECL(x)
#define CLR1(x) SMP_OP_CLEAR_DECL(x)
#define BBC(x) SMP_OP_BBC_DECL(x)
#define BBS(x) SMP_OP_BBS_DECL(x)

#define ALU(read, op) SMP_OP_ALU_##read##_DECL(op)
#define ALUXY(reg, read) SMP_OP_ALUXY_CMP_DECL(reg, read)
#define RMW(type, op) SMP_OP_RMW_##type##_DECL(op)
#define RMWW(op) SMP_OP_RMWW_DP_DECL(op)

#define BRANCH(flag) SMP_OP_BRANCH_FLAG_DECL(flag)
#define BRANCH_N(flag) SMP_OP_BRANCH_N_FLAG_DECL(flag)

smp_op_t ssnes_smp_optable[256] = {
   NOP,                    // 0x00
   TCALL(0),               // 0x01
   SET1(0),                // 0x02
   BBS(0),                 // 0x03
   ALU(DP, or),            // 0x04
   ALU(ADDR, or),          // 0x05
   ALU(DPX, or),           // 0x06
   ALU(IDPX, or),          // 0x07
   ALU(IMM, or),           // 0x08
   ALU(DP_DP, or),         // 0x09
   ALU(BIT, or1),          // 0x0a
   RMW(DP, asl),           // 0x0b
   RMW(ADDR, asl),         // 0x0c
   smp_op_push_p,          // 0x0d
   RMW(A, inc),            // 0x0e
   smp_op_brk,             // 0x0f
   BRANCH_N(n),            // 0x10
   TCALL(1),               // 0x11
   CLR1(0),                // 0x12
   BBC(0),                 // 0x13
   ALU(DPX, or),           // 0x14
   ALU(ADDRX, or),         // 0x15
   ALU(ADDRY, or),         // 0x16
   ALU(IDPY, or),          // 0x17
   ALU(IMM, or),           // 0x18
   ALU(DPX_DPY, or),       // 0x19
   RMWW(decw),             // 0x1a
   RMW(DPX, asl),          // 0x1b
   RMW(A, asl),            // 0x1c
   RMW(X, dec),            // 0x1d
   ALUXY(x, addr),         // 0x1e
   smp_op_jmpix,           // 0x1f
   smp_op_clrp,            // 0x20
   TCALL(2),               // 0x21
   SET1(1),                // 0x22







};

uint8_t ssnes_smp_cycle_table[256];
